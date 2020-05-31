import stormpy
import stormpy.logic
from helpers.helper import get_example_path
import pytest


class TestSparseModel:
    def test_init_default(self):
        components = stormpy.SparseModelComponents()

        assert components.state_labeling.get_labels() == set()
        assert components.reward_models == {}
        assert components.transition_matrix.nr_rows == 0
        assert components.transition_matrix.nr_columns == 0
        assert components.markovian_states is None
        assert components.player1_matrix is None
        assert not components.rate_transitions

    # def test_init(self):
    # todo Build simple transition matrix etc
    # transition_matrix =


    def test_dtmc_modelcomponents(self):
        program = stormpy.parse_prism_program(get_example_path("dtmc", "die.pm"))
        model = stormpy.build_model(program)

        components = stormpy.SparseModelComponents(transition_matrix=model.transition_matrix,
                                                   state_labeling=model.labeling,
                                                   reward_models=model.reward_models)

        dtmc = stormpy.storage.SparseDtmc(components)

        assert dtmc.model_type == stormpy.ModelType.DTMC
        assert dtmc.initial_states == [0]
        assert dtmc.nr_states == 13
        for state in dtmc.states:
            assert len(state.actions) <= 1
        assert dtmc.labeling.get_labels() == {'init', 'deadlock', 'done', 'one', 'two', 'three', 'four', 'five', 'six'}
        assert dtmc.nr_transitions == 20
        assert len(dtmc.reward_models) == 1
        assert not dtmc.reward_models["coin_flips"].has_state_rewards
        assert dtmc.reward_models["coin_flips"].has_state_action_rewards
        for reward in dtmc.reward_models["coin_flips"].state_action_rewards:
            assert reward == 1.0 or reward == 0.0
        assert not dtmc.reward_models["coin_flips"].has_transition_rewards
        assert not dtmc.supports_parameters


    def test_pmdp_modelcomponents(self):
        program = stormpy.parse_prism_program(get_example_path("pmdp", "two_dice.nm"))
        model = stormpy.build_parametric_model(program)

 
    def test_ma_modelcomponents(self):
        program = stormpy.parse_prism_program(get_example_path("ma", "simple.ma"), False, True)
        formulas = stormpy.parse_properties_for_prism_program("Pmax=? [ F<=2 s=2 ]", program)
        model = stormpy.build_model(program, formulas)
        #todo create mc

        assert model.nr_states == 4
        assert model.nr_transitions == 7
        assert model.model_type == stormpy.ModelType.MA
        assert not model.supports_parameters
        assert type(model) is stormpy.SparseMA
