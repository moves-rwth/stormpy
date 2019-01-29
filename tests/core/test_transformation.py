import stormpy
import stormpy.logic
from helpers.helper import get_example_path

import math


class TestTransformation:
    def test_transform_symbolic_dtmc_to_sparse(self):
        program = stormpy.parse_prism_program(get_example_path("dtmc", "crowds5_5.pm"))
        symbolic_model = stormpy.build_symbolic_model(program)
        assert symbolic_model.nr_states == 8607
        assert symbolic_model.nr_transitions == 15113
        assert symbolic_model.model_type == stormpy.ModelType.DTMC
        assert not symbolic_model.supports_parameters
        assert type(symbolic_model) is stormpy.SymbolicSylvanDtmc
        sparse_model = stormpy.transform_to_sparse_model(symbolic_model)
        assert sparse_model.nr_states == 8607
        assert sparse_model.nr_transitions == 15113
        assert sparse_model.model_type == stormpy.ModelType.DTMC
        assert not sparse_model.supports_parameters
        assert type(sparse_model) is stormpy.SparseDtmc

    def test_transform_symbolic_parametric_dtmc_to_sparse(self):
        program = stormpy.parse_prism_program(get_example_path("pdtmc", "parametric_die.pm"))
        model = stormpy.build_symbolic_parametric_model(program)
        assert model.nr_states == 13
        assert model.nr_transitions == 20
        assert model.model_type == stormpy.ModelType.DTMC
        assert model.supports_parameters
        assert type(model) is stormpy.SymbolicSylvanParametricDtmc
        symbolic_model = stormpy.transform_to_sparse_model(model)
        assert symbolic_model.nr_states == 13
        assert symbolic_model.nr_transitions == 20
        assert symbolic_model.model_type == stormpy.ModelType.DTMC
        assert symbolic_model.supports_parameters
        assert type(symbolic_model) is stormpy.SparseParametricDtmc

    def test_transform_continuous_to_discrete_time_model(self):
        ctmc = stormpy.build_model_from_drn(get_example_path("ctmc", "dft.drn"))
        formulas = stormpy.parse_properties("T=? [ F \"failed\" ]")
        print(formulas)
        assert ctmc.nr_states == 16
        assert ctmc.nr_transitions == 33
        assert len(ctmc.initial_states) == 1
        initial_state = ctmc.initial_states[0]
        assert initial_state == 1
        result = stormpy.model_checking(ctmc, formulas[0])
        assert math.isclose(result.at(initial_state), 4.166666667)

        dtmc, dtmc_formulas = stormpy.transform_to_discrete_time_model(ctmc, formulas)
        print(dtmc_formulas)
        assert dtmc.nr_states == 16
        assert dtmc.nr_transitions == 33
        assert len(dtmc.initial_states) == 1
        initial_state = dtmc.initial_states[0]
        assert initial_state == 1
        result = stormpy.model_checking(dtmc, dtmc_formulas[0])
        assert math.isclose(result.at(initial_state), 4.166666667)
