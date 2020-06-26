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

    # todo: ctmc
    # todo: ma
    # todo mdp
    # todo pomdp?

    def test_build_dtmc_from_model_components(self):
        nr_states = 13
        nr_choices = 13

        # TransitionMatrix
        builder = stormpy.SparseMatrixBuilder(rows=0, columns=0, entries=0, force_dimensions=False,
                                              has_custom_row_grouping=False, row_groups=0)

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
        for s in range(7, 13):
            builder.add_next_value(s, s, 1)
        # Build transition matrix, update number of rows and columns
        transition_matrix = builder.build(nr_states, nr_states)

        # StateLabeling
        state_labeling = stormpy.storage.StateLabeling(nr_states)
        state_labels = {'init', 'one', 'two', 'three', 'four', 'five', 'six', 'done', 'deadlock'}
        for label in state_labels:
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
        reward_models['coin_flips'] = stormpy.SparseRewardModel(optional_state_action_reward_vector=action_reward)

        # StateValuations
        manager = stormpy.ExpressionManager()
        var_s = manager.create_integer_variable(name='s')
        var_d = manager.create_integer_variable(name='d')
        v_builder = stormpy.StateValuationsBuilder()

        v_builder.add_variable(var_s)
        v_builder.add_variable(var_d)

        for s in range(7):
            v_builder.add_state(state=s, integer_values=[s, 0])
        for s in range(7, 13):
            v_builder.add_state(state=s, integer_values=[7, s - 6])

        state_valuations = v_builder.build(13)

        # todo choice origins:
        prism_program = stormpy.parse_prism_program(get_example_path("dtmc", "die.pm"))
        index_to_identifier_mapping = [1, 2, 3, 4, 5, 6, 7, 8, 8, 8, 8, 8, 8]
        id_to_command_set_mapping = [stormpy.FlatSet() for _ in range(9)]
        for i in range(1, 8):  # 0: no origin
            id_to_command_set_mapping[i].insert(i - 1)
        id_to_command_set_mapping[8].insert(7)
        #
        choice_origins = stormpy.PrismChoiceOrigins(prism_program, index_to_identifier_mapping,
                                                    id_to_command_set_mapping)

        components = stormpy.SparseModelComponents(transition_matrix=transition_matrix, state_labeling=state_labeling,
                                                   reward_models=reward_models)
        components.choice_origins = choice_origins

        components.state_valuations = state_valuations

        dtmc = stormpy.storage.SparseDtmc(components)

        assert type(dtmc) is stormpy.SparseDtmc
        assert not dtmc.supports_parameters

        # test transition matrix
        assert dtmc.nr_choices == nr_choices
        assert dtmc.nr_states == nr_states
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

        # test choice_labeling
        assert not dtmc.has_choice_labeling()

        # test state_valuations
        assert dtmc.has_state_valuations()
        assert dtmc.state_valuations
        value_s = [None] * nr_states
        value_d = [None] * nr_states
        for s in range(0, dtmc.nr_states):
            value_s[s] = dtmc.state_valuations.get_integer_value(s, var_s)
            value_d[s] = dtmc.state_valuations.get_integer_value(s, var_d)
        assert value_s == [0, 1, 2, 3, 4, 5, 6, 7, 7, 7, 7, 7, 7]
        assert value_d == [0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6]

        # todo choice_origins more tests
        assert dtmc.has_choice_origins()
        assert dtmc.choice_origins is components.choice_origins
        assert dtmc.choice_origins.get_number_of_identifiers() == 9
