#include "simulator.h"
#include "src/helpers.h"

#include "storm-dft/simulator/DFTTraceSimulator.h"
#include "storm-dft/api/storm-dft.h"
#include "storm-dft/generator/DftNextStateGenerator.h"
#include "storm-dft/storage/SymmetricUnits.h"
#include "storm-dft/storage/DFTIsomorphism.h"


template<typename ValueType> using Simulator = storm::dft::simulator::DFTTraceSimulator<ValueType>;
typedef storm::dft::storage::DFTStateGenerationInfo DFTStateInfo;
typedef boost::mt19937 RandomGenerator;


void define_simulator(py::module& m) {

    // Simulation result
    py::enum_<storm::dft::simulator::SimulationResult>(m, "SimulationResult")
        .value("SUCCESSFUL", storm::dft::simulator::SimulationResult::SUCCESSFUL)
        .value("UNSUCCESSFUL", storm::dft::simulator::SimulationResult::UNSUCCESSFUL)
        .value("INVAlID", storm::dft::simulator::SimulationResult::INVALID)
    ;
    py::class_<DFTStateInfo, std::shared_ptr<DFTStateInfo>>(m, "DFTStateInfo", "State Generation Info for DFT")
    ;

    py::class_<RandomGenerator, std::shared_ptr<RandomGenerator>>(m, "RandomGenerator", "Random number generator")
        .def_static("create", [](unsigned int seed) -> RandomGenerator {
            return RandomGenerator(seed);
        }, py::arg("seed"), "Initialize random number generator")
    ;
}


template<typename ValueType>
void define_simulator_typed(py::module& m, std::string const& vt_suffix) {

    // Simulator for DFTs
    py::class_<Simulator<ValueType>, std::shared_ptr<Simulator<ValueType>>>(m, ("DFTSimulator"+vt_suffix).c_str(), "Simulator for DFT traces")
        .def(py::init<storm::dft::storage::DFT<ValueType> const&, DFTStateInfo const&, RandomGenerator&>(), py::keep_alive<1,2>(), py::keep_alive<1, 3>(), py::keep_alive<1,4>(), py::arg("dft"), py::arg("state_generation_info"), py::arg("generator"), "Create Simulator")
        .def("reset", &Simulator<ValueType>::resetToInitial, "Reset to initial state")
        .def("current", &Simulator<ValueType>::getCurrentState, "Get current state")
        .def("step", &Simulator<ValueType>::step, py::arg("next_failure"), py::arg("dependency_success") = true, "Perform simulation step according to next_failure. For PDEPs, dependency_success determines whether the PDEP was successful or not.")
        .def("random_step", &Simulator<ValueType>::randomStep, "Perform random simulation step. Returns a tuple of the simulation result and the time which progressed during this step.")
        .def("simulate_trace", &Simulator<ValueType>::simulateCompleteTrace, py::arg("timebound"), "Simulate complete trace for given timebound")
    ;
}


template void define_simulator_typed<double>(py::module& m, std::string const& vt_suffix);
template void define_simulator_typed<storm::RationalFunction>(py::module& m, std::string const& vt_suffix);
