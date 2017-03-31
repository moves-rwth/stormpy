import stormpy
import stormpy.logic
from helpers.helper import get_example_path

import math


class TestModelChecking:
    def test_model_checking_dtmc(self):
        program = stormpy.parse_prism_program(get_example_path("dtmc", "die.pm"))
        formulas = stormpy.parse_properties_for_prism_program("P=? [ F \"one\" ]", program)
        model = stormpy.build_model(program, formulas)
        assert model.nr_states == 13
        assert model.nr_transitions == 20
        assert len(model.initial_states) == 1
        initial_state = model.initial_states[0]
        assert initial_state == 0
        result = stormpy.model_checking(model, formulas[0])
        assert math.isclose(result.at(initial_state), 0.16666666666666663)
    
    def test_model_checking_all_dtmc(self):
        program = stormpy.parse_prism_program(get_example_path("dtmc", "die.pm"))
        formulas = stormpy.parse_properties_for_prism_program("P=? [ F \"one\" ]", program)
        model = stormpy.build_model(program, formulas)
        assert model.nr_states == 13
        assert model.nr_transitions == 20
        result = stormpy.model_checking(model, formulas[0])
        assert result.result_for_all_states
        reference = [0.16666666666666663, 0.3333333333333333, 0, 0.6666666666666666, 0, 0, 0, 1, 0, 0, 0, 0, 0]
        assert all(map(math.isclose, result.get_values(), reference))
    
    def test_parametric_state_elimination(self):
        import pycarl
        import pycarl.formula
        program = stormpy.parse_prism_program(get_example_path("pdtmc", "brp16_2.pm"))
        prop = "P=? [F s=5]"
        formulas = stormpy.parse_properties_for_prism_program(prop, program)
        model = stormpy.build_parametric_model(program, formulas)
        assert model.nr_states == 613
        assert model.nr_transitions == 803
        assert model.model_type == stormpy.ModelType.DTMC
        assert model.has_parameters
        initial_state = model.initial_states[0]
        assert initial_state == 0
        result = stormpy.model_checking(model, formulas[0])
        func = result.result.at(initial_state)
        one = pycarl.FactorizedPolynomial(pycarl.Rational(1))
        assert func.denominator == one
        constraints_well_formed = result.constraints_well_formed
        for constraint in constraints_well_formed:
            assert constraint.rel() == pycarl.formula.Relation.GEQ or constraint.rel() == pycarl.formula.Relation.LEQ
        constraints_graph_preserving = result.constraints_graph_preserving
        for constraint in constraints_graph_preserving:
            assert constraint.rel() == pycarl.formula.Relation.GREATER

    def test_model_checking_prob01(self):
        program = stormpy.parse_prism_program(get_example_path("dtmc", "die.pm"))
        formulaPhi = stormpy.parse_properties("true")[0]
        formulaPsi = stormpy.parse_properties("\"six\"")[0]
        model = stormpy.build_model(program, [formulaPsi])
        phiResult = stormpy.model_checking(model, formulaPhi)
        phiStates = phiResult.get_truth_values()
        assert phiStates.number_of_set_bits() == model.nr_states
        psiResult = stormpy.model_checking(model, formulaPsi)
        psiStates = psiResult.get_truth_values()
        assert psiStates.number_of_set_bits() == 1
        (prob0, prob1) = stormpy.compute_prob01_states(model, phiStates, psiStates)
        assert prob0.number_of_set_bits() == 9
        assert prob1.number_of_set_bits() == 1
        (prob0, prob1) = stormpy.compute_prob01min_states(model, phiStates, psiStates)
        assert prob0.number_of_set_bits() == 9
        assert prob1.number_of_set_bits() == 1
        (prob0, prob1) = stormpy.compute_prob01max_states(model, phiStates, psiStates)
        assert prob0.number_of_set_bits() == 9
        assert prob1.number_of_set_bits() == 1
        labelprop = stormpy.core.Property("cora", formulaPsi.raw_formula)
        result = stormpy.model_checking(model, labelprop)
        assert result.get_truth_values().number_of_set_bits() == 1
