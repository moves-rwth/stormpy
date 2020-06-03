import stormpy
import stormpy.logic
from helpers.helper import get_example_path
import pytest


class TestSparseModelComponents:
    def test_init_default(self):
        components = stormpy.SparseModelComponents()

        assert components.state_labeling.get_labels() == set()
        assert components.reward_models == {}
        assert components.transition_matrix.nr_rows == 0
        assert components.transition_matrix.nr_columns == 0
        assert components.markovian_states is None
        assert components.player1_matrix is None
        assert not components.rate_transitions

    def test_build_dtmc_from_model_components(self):
        program = stormpy.parse_prism_program(get_example_path("dtmc", "die.pm"))
        options = stormpy.BuilderOptions(True, True)
        options.set_build_with_choice_origins()
        options.set_build_state_valuations()
        options.set_build_all_labels()
        model = stormpy.build_sparse_model_with_options(program, options)

        components = stormpy.SparseModelComponents(transition_matrix=model.transition_matrix,
                                                   state_labeling=model.labeling,
                                                   reward_models=model.reward_models)
        components.choice_origins = model.choice_origins
        components.state_valuations = model.state_valuations

        dtmc = stormpy.storage.SparseDtmc(components)

        assert type(dtmc) is stormpy.SparseDtmc
        assert not dtmc.supports_parameters

        # test transition matrix
        assert dtmc.nr_choices == 13
        assert dtmc.nr_states == 13
        assert dtmc.nr_transitions == 20
        assert dtmc.transition_matrix.nr_entries == 20
        assert dtmc.transition_matrix.nr_entries == model.nr_transitions
        for e in dtmc.transition_matrix:
            assert e.value() == 0.5 or e.value() == 0 or (e.value() == 1 and e.column > 6)
        for state in dtmc.states:
            assert len(state.actions) <= 1

        # test state_labeling
        assert dtmc.labeling.get_labels() == {'init', 'deadlock', 'done', 'one', 'two', 'three', 'four', 'five', 'six'}

        # test reward_models
        assert len(model.reward_models) == 1
        assert not model.reward_models["coin_flips"].has_state_rewards
        assert model.reward_models["coin_flips"].has_state_action_rewards
        for reward in model.reward_models["coin_flips"].state_action_rewards:
            assert reward == 1.0 or reward == 0.0
        assert not model.reward_models["coin_flips"].has_transition_rewards

        # choice_labeling
        assert not dtmc.has_choice_labeling()
        # state_valuations
        assert dtmc.has_state_valuations()
        # choice_origins
        assert dtmc.has_choice_origins()
        assert dtmc.choice_origins is components.choice_origins  # todo correct?
