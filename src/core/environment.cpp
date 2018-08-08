#include "environment.h"
#include "src/helpers.h"
#include "storm/environment/Environment.h"
#include "storm/environment/solver/SolverEnvironment.h"
#include "storm/environment/solver/EigenSolverEnvironment.h"
#include "storm/environment/solver/GmmxxSolverEnvironment.h"
#include "storm/environment/solver/GameSolverEnvironment.h"
#include "storm/environment/solver/NativeSolverEnvironment.h"
#include "storm/environment/solver/TopologicalSolverEnvironment.h"
#include "storm/environment/solver/MultiplierEnvironment.h"
#include "storm/environment/solver/MinMaxSolverEnvironment.h"

void define_environment(py::module& m) {
    py::enum_<storm::solver::EquationSolverType>(m, "EquationSolverType", "Solver type for equation systems")
        .value("native", storm::solver::EquationSolverType::Native)
        .value("eigen", storm::solver::EquationSolverType::Eigen)
        .value("elimination", storm::solver::EquationSolverType::Elimination)
        .value("gmmxx", storm::solver::EquationSolverType::Gmmxx)
        .value("topological", storm::solver::EquationSolverType::Topological)
    ;

    py::class_<storm::Environment>(m, "Environment", "Environment")
        .def(py::init<>(), "Construct default environment")
        .def_property_readonly("solver_environment", [](storm::Environment& env) {return env.solver();}, "solver part of environment")
    ;

    py::class_<storm::SolverEnvironment>(m, "SolverEnvironment", "Environment for solvers")
        .def("set_force_sound", &storm::SolverEnvironment::setForceSoundness, "force soundness", py::arg("new_value") = true)
        .def("set_linear_equation_solver_type", &storm::SolverEnvironment::setLinearEquationSolverType, "set solver type to use", py::arg("new_value"), py::arg("set_from_default") = false)
    ;

}

