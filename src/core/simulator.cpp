#include "simulator.h"
#include <storm/simulator/DiscreteTimeSparseModelSimulator.h>

void define_sparse_model_simulator(py::module& m) {
    py::class_<storm::simulator::DiscreteTimeSparseModelSimulator<double>> dtsmsd(m, "_DiscreteTimeSparseModelSimulatorDouble", "Simulator for sparse discrete-time models in memory (ValueType = double)");
    dtsmsd.def(py::init<storm::models::sparse::Model<double> const&>());
    dtsmsd.def("set_seed", &storm::simulator::DiscreteTimeSparseModelSimulator<double>::setSeed, py::arg("seed"));
    dtsmsd.def("step", &storm::simulator::DiscreteTimeSparseModelSimulator<double>::step, py::arg("action"));
    dtsmsd.def("get_current_state", &storm::simulator::DiscreteTimeSparseModelSimulator<double>::getCurrentState);
    dtsmsd.def("reset_to_initial_state", &storm::simulator::DiscreteTimeSparseModelSimulator<double>::resetToInitial);

}