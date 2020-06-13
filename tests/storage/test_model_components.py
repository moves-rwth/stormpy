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
        nr_states = 13

        # TransitionMatrix
        builder = stormpy.SparseMatrixBuilder(rows = 0, columns = 0, entries = 0, force_dimensions = False, has_custom_row_grouping = False, row_groups = 0)

        # Add transitions
        builder.add_next_value(0, 1, 0.5)
        builder.add_next_value(0, 2, 0.5)
        builder.add_next_value(1, 3, 0.5)
        builder.add_next_value(1, 4, 0.5)
        builder.add_next_value(2, 5, 0.5)
        builder.add_next_value(2, 6, 0.5)
        builder.add_next_value(3, 7, 0.5)
        builder.add_next_value(3, 1, 0.5)
        builder.add_next_value(4, 8, 0.5)
        builder.add_next_value(4, 9, 0.5)
        builder.add_next_value(5, 10, 0.5)
        builder.add_next_value(5, 11, 0.5)
        builder.add_next_value(6, 2, 0.5)
        builder.add_next_value(6, 12, 0.5)
        for s in range(7,13):
            builder.add_next_value(s, s, 1)

        # Build transition matrix, update number of rows and columns
        transition_matrix = builder.build(nr_states, nr_states)


        # StateLabeling
        state_labeling = stormpy.storage.StateLabeling(nr_states)
        labels = {'init','one', 'two', 'three', 'four', 'five', 'six', 'done', 'deadlock'}
        for label in labels:
            state_labeling.add_label(label)
        # Add label to one state
        state_labeling.add_label_to_state('init', 0)
        state_labeling.add_label_to_state('one', 7)
        state_labeling.add_label_to_state('two', 8)
        state_labeling.add_label_to_state('three', 9)
        state_labeling.add_label_to_state('four', 10)
        state_labeling.add_label_to_state('five', 11)
        state_labeling.add_label_to_state('six', 12)

        # Set the labeling of states given in a bit vector, where length = nr_states
        state_labeling.set_states('done', stormpy.BitVector(nr_states, [7, 8, 9, 10, 11, 12]))

        # RewardModels
        reward_models = {}
        # Create a vector representing the state-action rewards
        action_reward = [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
        reward_models['coin_flips'] = stormpy.SparseRewardModel(optional_state_action_reward_vector = action_reward)


        components = stormpy.SparseModelComponents(transition_matrix=transition_matrix,
                                                   state_labeling=state_labeling,
                                                   reward_models=reward_models)

        # todo components.choice_origins = choice_origins
        # todo components.state_valuations = state_valuations

        dtmc = stormpy.storage.SparseDtmc(components)

        assert type(dtmc) is stormpy.SparseDtmc
        assert not dtmc.supports_parameters

        # test transition matrix
        assert dtmc.nr_choices == 13
        assert dtmc.nr_states == 13
        assert dtmc.nr_transitions == 20
        assert dtmc.transition_matrix.nr_entries == dtmc.nr_transitions
        for e in dtmc.transition_matrix:
            assert e.value() == 0.5 or e.value() == 0 or (e.value() == 1 and e.column > 6)
        for state in dtmc.states:
            assert len(state.actions) <= 1

        # test state_labeling
        assert dtmc.labeling.get_labels() == {'init', 'deadlock', 'done', 'one', 'two', 'three', 'four', 'five', 'six'}

        # test reward_models
        assert len(dtmc.reward_models) == 1
        assert not dtmc.reward_models["coin_flips"].has_state_rewards
        assert dtmc.reward_models["coin_flips"].has_state_action_rewards
        for reward in dtmc.reward_models["coin_flips"].state_action_rewards:
            assert reward == 1.0 or reward == 0.0
        assert not dtmc.reward_models["coin_flips"].has_transition_rewards

        # choice_labeling
        assert not dtmc.has_choice_labeling()
        # todo state_valuations
        #  assert dtmc.has_state_valuations()
        # todo choice_origins
        #  assert dtmc.has_choice_origins()
        # assert dtmc.choice_origins is components.choice_origins  # todo
