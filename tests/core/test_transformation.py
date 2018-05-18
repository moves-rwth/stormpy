import stormpy
import stormpy.logic
from helpers.helper import get_example_path


class TestTransformation:
    def test_transform_symbolic_dtmc_to_sparse(self):
        program = stormpy.parse_prism_program(get_example_path("dtmc", "crowds5_5.pm"))
        model = stormpy.build_symbolic_model(program)
        assert model.nr_states == 8607
        assert model.nr_transitions == 15113
        assert model.model_type == stormpy.ModelType.DTMC
        assert not model.supports_parameters
        assert type(model) is stormpy.SymbolicSylvanDtmc
        symbolic_model = stormpy.transform_to_sparse_model(model)
        assert symbolic_model.nr_states == 8607
        assert symbolic_model.nr_transitions == 15113
        assert symbolic_model.model_type == stormpy.ModelType.DTMC
        assert not symbolic_model.supports_parameters
        assert type(symbolic_model) is stormpy.SparseDtmc

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
