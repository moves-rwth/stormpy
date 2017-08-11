import stormpy
import stormpy.logic
from helpers.helper import get_example_path
import pytest


class TestModel:
    def test_build_dtmc_from_prism_program(self):
        program = stormpy.parse_prism_program(get_example_path("dtmc", "die.pm"))
        model = stormpy.build_model(program)
        assert model.nr_states == 13
        assert model.nr_transitions == 20
        assert model.model_type == stormpy.ModelType.DTMC
        assert not model.supports_parameters
        assert type(model) is stormpy.SparseDtmc

    def test_build_dtmc_from_prism_program_formulas(self):
        program = stormpy.parse_prism_program(get_example_path("dtmc", "die.pm"))
        prop = "P=? [F \"one\"]"
        properties = stormpy.parse_properties_for_prism_program(prop, program, None)
        model = stormpy.build_model(program, properties)
        assert model.nr_states == 13
        assert model.nr_transitions == 20
        assert model.model_type == stormpy.ModelType.DTMC
        assert not model.supports_parameters
        assert type(model) is stormpy.SparseDtmc

    def test_build_parametric_dtmc_from_prism_program(self):
        program = stormpy.parse_prism_program(get_example_path("pdtmc", "brp16_2.pm"))
        prop = "P=? [F s=5]"
        formulas = stormpy.parse_properties_for_prism_program(prop, program)
        model = stormpy.build_parametric_model(program, formulas)
        assert model.nr_states == 613
        assert model.nr_transitions == 803
        assert model.model_type == stormpy.ModelType.DTMC
        assert model.supports_parameters
        assert model.has_parameters
        assert type(model) is stormpy.SparseParametricDtmc

    def test_build_dtmc_from_jani_model(self):
        jani_model, properties = stormpy.parse_jani_model(get_example_path("dtmc", "die.jani"))
        model = stormpy.build_model(jani_model)
        assert model.nr_states == 13
        assert model.nr_transitions == 20
        assert model.model_type == stormpy.ModelType.DTMC
        assert not model.supports_parameters
        assert type(model) is stormpy.SparseDtmc

    def test_build_dtmc(self):
        program = stormpy.parse_prism_program(get_example_path("dtmc", "die.pm"))
        formulas = stormpy.parse_properties_for_prism_program("P=? [ F \"one\" ]", program)
        model = stormpy.build_model(program, formulas)
        assert model.nr_states == 13
        assert model.nr_transitions == 20
        assert model.model_type == stormpy.ModelType.DTMC
        assert not model.supports_parameters
        assert type(model) is stormpy.SparseDtmc

    def test_build_dtmc_with_undefined_constants(self):
        jani_model, properties = stormpy.parse_jani_model(get_example_path("dtmc", "brp.jani"))
        assert jani_model.has_undefined_constants
        assert not jani_model.undefined_constants_are_graph_preserving
        with pytest.raises(stormpy.StormError):
            model = stormpy.build_model(jani_model)

    def test_build_instantiated_dtmc(self):
        jani_model, properties = stormpy.parse_jani_model(get_example_path("dtmc", "brp.jani"))
        assert jani_model.has_undefined_constants
        assert not jani_model.undefined_constants_are_graph_preserving
        description = stormpy.SymbolicModelDescription(jani_model)
        constant_definitions = description.parse_constant_definitions("N=16, MAX=2")
        instantiated_jani_model = description.instantiate_constants(constant_definitions).as_jani_model()
        model = stormpy.build_model(instantiated_jani_model)
        assert model.nr_states == 677
        assert model.nr_transitions == 867
        assert model.model_type == stormpy.ModelType.DTMC
        assert not model.supports_parameters
        assert type(model) is stormpy.SparseDtmc

    def test_build_parametric_dtmc(self):
        program = stormpy.parse_prism_program(get_example_path("pdtmc", "brp16_2.pm"))
        formulas = stormpy.parse_properties_for_prism_program("P=? [ F s=5 ]", program)
        model = stormpy.build_parametric_model(program, formulas)
        assert model.nr_states == 613
        assert model.nr_transitions == 803
        assert model.model_type == stormpy.ModelType.DTMC
        assert model.supports_parameters
        assert model.has_parameters
        assert type(model) is stormpy.SparseParametricDtmc

    def test_build_dtmc_supporting_parameters(self):
        program = stormpy.parse_prism_program(get_example_path("dtmc", "die.pm"))
        formulas = stormpy.parse_properties_for_prism_program("P=? [ F \"one\" ]", program)
        model = stormpy.build_parametric_model(program, formulas)
        assert model.nr_states == 13
        assert model.nr_transitions == 20
        assert model.model_type == stormpy.ModelType.DTMC
        assert model.supports_parameters
        assert not model.has_parameters
        assert type(model) is stormpy.SparseParametricDtmc

    def test_build_mdp(self):
        program = stormpy.parse_prism_program(get_example_path("mdp", "two_dice.nm"))
        formulas = stormpy.parse_properties_for_prism_program("P=? [ F \"two\" ]", program)
        model = stormpy.build_model(program, formulas)
        assert model.nr_states == 169
        assert model.nr_transitions == 435
        assert model.model_type == stormpy.ModelType.MDP
        assert not model.supports_parameters
        assert type(model) is stormpy.SparseMdp

    def test_initial_states(self):
        program = stormpy.parse_prism_program(get_example_path("dtmc", "die.pm"))
        formulas = stormpy.parse_properties_for_prism_program("P=? [ F \"one\" ]", program)
        model = stormpy.build_model(program, formulas)
        initial_states = model.initial_states
        assert len(initial_states) == 1
        assert 0 in initial_states
