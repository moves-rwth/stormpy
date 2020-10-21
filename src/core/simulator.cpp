#include "simulator.h"
#include <storm/simulator/DiscreteTimeSparseModelSimulator.h>

template<typename ValueType>
void define_sparse_model_simulator(py::module& m, std::string const& vtSuffix) {
    py::class_<storm::simulator::DiscreteTimeSparseModelSimulator<ValueType>> dtsmsd(m, ("_DiscreteTimeSparseModelSimulator" + vtSuffix).c_str(), "Simulator for sparse discrete-time models in memory (ValueType = double)");
    dtsmsd.def(py::init<storm::models::sparse::Model<ValueType> const&>());
    dtsmsd.def("set_seed", &storm::simulator::DiscreteTimeSparseModelSimulator<ValueType>::setSeed, py::arg("seed"));
    dtsmsd.def("step", &storm::simulator::DiscreteTimeSparseModelSimulator<ValueType>::step, py::arg("action"));
    dtsmsd.def("random_step", &storm::simulator::DiscreteTimeSparseModelSimulator<ValueType>::randomStep);
    dtsmsd.def("get_last_reward", &storm::simulator::DiscreteTimeSparseModelSimulator<ValueType>::getLastRewards);
    dtsmsd.def("get_current_state", &storm::simulator::DiscreteTimeSparseModelSimulator<ValueType>::getCurrentState);
    dtsmsd.def("reset_to_initial_state", &storm::simulator::DiscreteTimeSparseModelSimulator<ValueType>::resetToInitial);
}

template void define_sparse_model_simulator<double>(py::module& m, std::string const& vtSuffix);
template void define_sparse_model_simulator<storm::RationalNumber>(py::module& m, std::string const& vtSuffix);
