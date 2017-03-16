import stormpy
from helpers.helper import get_example_path

class TestState:
    def test_states_dtmc(self):
        model = stormpy.parse_explicit_model(get_example_path("dtmc", "die.tra"), get_example_path("dtmc", "die.lab"))
        i = 0
        states = model.states
        for state in states:
            assert state.id == i
            i += 1
        assert i == model.nr_states
        state = model.states[2]
        assert state.id == 2
        state = states[5]
        assert state.id == 5
    
    def test_states_mdp(self):
        model = stormpy.parse_explicit_model(get_example_path("mdp", "two_dice.tra"), get_example_path("mdp", "two_dice.lab"))
        i = 0
        for state in model.states:
            assert state.id == i
            i += 1
        assert i == model.nr_states
        states = model.states
        state = model.states[6]
        assert state.id == 6
        state = states[1]
        assert state.id == 1
    
    def test_labels_dtmc(self):
        model = stormpy.parse_explicit_model(get_example_path("dtmc", "die.tra"), get_example_path("dtmc", "die.lab"))
        labelsOrig = [["init"], [], [], [], [], [], [], ["one", "done"], ["two", "done"], ["three", "done"], ["four", "done"], ["five", "done"], ["six", "done"]]
        i = 0
        for state in model.states:
            labels = state.labels
            for label in labelsOrig[i]:
                assert label in labels
            i += 1
    
    def test_actions_dtmc(self):
        model = stormpy.parse_explicit_model(get_example_path("dtmc", "die.tra"), get_example_path("dtmc", "die.lab"))
        for state in model.states:
            for action in state.actions:
                assert action.id == 0
    
    def test_actions_mdp(self):
        model = stormpy.parse_explicit_model(get_example_path("mdp", "two_dice.tra"), get_example_path("mdp", "two_dice.lab"))
        for state in model.states:
            for action in state.actions:
                assert action.id == 0 or action.id == 1
    
    def test_transitions_dtmc(self):
        transitions_orig = [(0, 0, 0), (0, 1, 0.5), (0, 2, 0.5), (1, 1, 0), (1, 3, 0.5), (1, 4, 0.5), 
                (2, 2, 0), (2, 5, 0.5), (2, 6, 0.5), (3, 1, 0.5), (3, 3, 0), (3, 7, 0.5),
                (4, 4, 0), (4, 8, 0.5), (4, 9, 0.5), (5, 5, 0), (5, 10, 0.5), (5, 11, 0.5),
                (6, 2, 0.5), (6, 6, 0), (6, 12, 0.5), (7, 7, 1), (8, 8, 1),
                (9, 9, 1), (10, 10, 1), (11, 11, 1), (12, 12, 1)
            ]
        model = stormpy.parse_explicit_model(get_example_path("dtmc", "die.tra"), get_example_path("dtmc", "die.lab"))
        i = 0
        for state in model.states:
            for action in state.actions:
                for transition in action.transitions:
                    transition_orig = transitions_orig[i]
                    i += 1
                    assert state.id == transition_orig[0]
                    assert transition.column == transition_orig[1]
                    assert transition.value() == transition_orig[2]
    
    def test_transitions_mdp(self):
        model = stormpy.parse_explicit_model(get_example_path("mdp", "two_dice.tra"), get_example_path("mdp", "two_dice.lab"))
        assert model.states[1].id == 1
        for state in model.states:
            i = 0
            for action in state.actions:
                i += 1
                for transition in action.transitions:
                    assert transition.value() == 0.5 or transition.value() == 1
            assert i == 1 or i == 2

    def test_row_iterator(self):
        transitions_orig = [(0, 0, 0), (0, 1, 0.5), (0, 2, 0.5), (1, 1, 0), (1, 3, 0.5), (1, 4, 0.5),
                (2, 2, 0), (2, 5, 0.5), (2, 6, 0.5), (3, 1, 0.5), (3, 3, 0), (3, 7, 0.5),
                (4, 4, 0), (4, 8, 0.5), (4, 9, 0.5), (5, 5, 0), (5, 10, 0.5), (5, 11, 0.5),
                (6, 2, 0.5), (6, 6, 0), (6, 12, 0.5), (7, 7, 1), (8, 8, 1),
                (9, 9, 1), (10, 10, 1), (11, 11, 1), (12, 12, 1)
            ]
        model = stormpy.parse_explicit_model(get_example_path("dtmc", "die.tra"), get_example_path("dtmc", "die.lab"))
        i = 0
        for state in model.states:
            for transition in model.transition_matrix.row_iter(state.id, state.id):
                transition_orig = transitions_orig[i]
                i += 1
                assert state.id == transition_orig[0]
                assert transition.column == transition_orig[1]
                assert transition.value() == transition_orig[2]
    
    def test_parametric_transitions(self):
        import pycarl
        program = stormpy.parse_prism_program(get_example_path("pmdp", "two_dice.nm"))
        model = stormpy.build_parametric_model(program)
        assert model.states[1].id == 1
        one = pycarl.FactorizedPolynomial(pycarl.Rational(1))
        i = 0
        for state in model.states:
            assert state.id == i
            i += 1
            j = 0
            for action in state.actions:
                assert j == 0 or j == 1
                j += 1
                for transition in action.transitions:
                    assert transition.value().denominator == one
