import stormpy
import stormpy.logic
from helpers.helper import get_example_path
from configurations import numpy_avail


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

    # todo mdp
    # todo pomdp?

    def test_build_dtmc_from_model_components(self):
        nr_states = 13
        nr_choices = 13

        # transition_matrix
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

        # state_labeling
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

        state_labeling.set_states('done', stormpy.BitVector(nr_states, [7, 8, 9, 10, 11, 12]))

        # reward_models
        reward_models = {}
        # Create a vector representing the state-action rewards
        action_reward = [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
        reward_models['coin_flips'] = stormpy.SparseRewardModel(optional_state_action_reward_vector=action_reward)

        # state_valuations
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

        # choice origins
        prism_program = stormpy.parse_prism_program(get_example_path("dtmc", "die.pm"))
        index_to_identifier_mapping = [1, 2, 3, 4, 5, 6, 7, 8, 8, 8, 8, 8, 8]
        id_to_command_set_mapping = [stormpy.FlatSet() for _ in range(9)]
        for i in range(1, 8):  # 0: no origin
            id_to_command_set_mapping[i].insert(i - 1)
        id_to_command_set_mapping[8].insert(7)
        choice_origins = stormpy.PrismChoiceOrigins(prism_program, index_to_identifier_mapping,
                                                    id_to_command_set_mapping)

        # Construct Components
        components = stormpy.SparseModelComponents(transition_matrix=transition_matrix, state_labeling=state_labeling,
                                                   reward_models=reward_models)
        components.choice_origins = choice_origins
        components.state_valuations = state_valuations

        # Build DTMC
        dtmc = stormpy.storage.SparseDtmc(components)

        assert type(dtmc) is stormpy.SparseDtmc
        assert not dtmc.supports_parameters

        # Test transition matrix
        assert dtmc.nr_choices == nr_choices
        assert dtmc.nr_states == nr_states
        assert dtmc.nr_transitions == 20
        assert dtmc.transition_matrix.nr_entries == dtmc.nr_transitions
        for e in dtmc.transition_matrix:
            assert e.value() == 0.5 or e.value() == 0 or (e.value() == 1 and e.column > 6)
        for state in dtmc.states:
            assert len(state.actions) <= 1

        # Test state_labeling
        assert dtmc.labeling.get_labels() == {'init', 'deadlock', 'done', 'one', 'two', 'three', 'four', 'five', 'six'}

        # Test reward_models
        assert len(dtmc.reward_models) == 1
        assert not dtmc.reward_models["coin_flips"].has_state_rewards
        assert dtmc.reward_models["coin_flips"].has_state_action_rewards
        for reward in dtmc.reward_models["coin_flips"].state_action_rewards:
            assert reward == 1.0 or reward == 0.0
        assert not dtmc.reward_models["coin_flips"].has_transition_rewards

        # Test choice_labeling
        assert not dtmc.has_choice_labeling()

        # Test state_valuations
        assert dtmc.has_state_valuations()
        assert dtmc.state_valuations
        value_s = [None] * nr_states
        value_d = [None] * nr_states
        for s in range(0, dtmc.nr_states):
            value_s[s] = dtmc.state_valuations.get_integer_value(s, var_s)
            value_d[s] = dtmc.state_valuations.get_integer_value(s, var_d)
        assert value_s == [0, 1, 2, 3, 4, 5, 6, 7, 7, 7, 7, 7, 7]
        assert value_d == [0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6]

        # Test choice_origins
        assert dtmc.has_choice_origins()
        assert dtmc.choice_origins is components.choice_origins
        assert dtmc.choice_origins.get_number_of_identifiers() == 9

    @numpy_avail
    def test_build_ctmc_from_model_components(self):
        import numpy as np

        nr_states = 12
        nr_choices = 12

        # Build transition_matrix
        transitions = np.array([
            [0, 0.5, 0.5, 200, 0, 0, 0, 0, 0, 0, 0, 0],
            [0, 0, 0, 0, 0.5, 200, 0, 0, 0, 0, 0, 0],
            [0, 0, 0, 0, 0.5, 0, 200, 0, 0, 0, 0, 0],
            [200, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0],
            [0, 0, 0, 0, 0, 0, 0, 0, 200, 0, 0, 0],
            [0, 0, 0, 1, 0, 0, 0, 0, 0.5, 0, 0, 0],
            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 200, 0],
            [0, 200, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0],
            [0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0],
            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5],
            [0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]], dtype='float64')

        transition_matrix = stormpy.build_sparse_matrix(transitions)

        # state_labeling
        state_labeling = stormpy.storage.StateLabeling(nr_states)
        # Add labels
        state_labels = {'init', 'deadlock', 'target'}
        for label in state_labels:
            state_labeling.add_label(label)

        # Add labels to states
        state_labeling.add_label_to_state('init', 0)
        state_labeling.set_states('target', stormpy.BitVector(nr_states, [5, 8]))

        # reward_models
        reward_models = {}
        # vector representing state-action rewards
        action_reward = [0.0, 0.0, 0.0, 0.0, 0.0, 2 / 3, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0]
        reward_models['served'] = stormpy.SparseRewardModel(optional_state_action_reward_vector=action_reward)

        # vector representing state rewards
        state_reward = [0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0]
        reward_models['waiting'] = stormpy.SparseRewardModel(optional_state_reward_vector=state_reward)

        # choice_labeling
        choice_labeling = stormpy.storage.ChoiceLabeling(nr_choices)
        choice_labels = {'loop1a', 'loop1b', 'serve1', 'loop2a', 'loop2b', 'serve2'}
        # Add labels
        for label in choice_labels:
            choice_labeling.add_label(label)

        choice_labeling.set_choices('loop1a', stormpy.BitVector(nr_choices, [0, 2]))
        choice_labeling.set_choices('loop1b', stormpy.BitVector(nr_choices, [1, 4]))
        choice_labeling.set_choices('serve1', stormpy.BitVector(nr_choices, [5, 8]))
        choice_labeling.set_choices('loop2a', stormpy.BitVector(nr_choices, [3, 7]))
        choice_labeling.set_choices('loop2b', stormpy.BitVector(nr_choices, [6, 9]))
        choice_labeling.set_choices('serve2', stormpy.BitVector(nr_choices, [10, 11]))

        # state exit rates
        exit_rates = [201.0, 200.5, 200.5, 201.0, 200.0, 1.5, 200.5, 200.5, 1.0, 200.0, 1.5, 1.0]

        # state_valuations
        manager = stormpy.ExpressionManager()
        var_s = manager.create_integer_variable(name='s')
        var_a = manager.create_integer_variable(name='a')
        var_s1 = manager.create_integer_variable(name='s1')
        var_s2 = manager.create_integer_variable(name='s2')
        v_builder = stormpy.StateValuationsBuilder()
        v_builder.add_variable(var_s)
        v_builder.add_variable(var_a)
        v_builder.add_variable(var_s1)
        v_builder.add_variable(var_s2)

        v_builder.add_state(state=0, boolean_values=[], integer_values=[1, 0, 0, 0], rational_values=[])
        v_builder.add_state(state=1, boolean_values=[], integer_values=[1, 0, 1, 0], rational_values=[])
        v_builder.add_state(state=2, boolean_values=[], integer_values=[1, 0, 0, 1], rational_values=[])
        v_builder.add_state(state=3, boolean_values=[], integer_values=[2, 0, 0, 0], rational_values=[])
        v_builder.add_state(state=4, boolean_values=[], integer_values=[1, 0, 1, 1], rational_values=[])
        v_builder.add_state(state=5, boolean_values=[], integer_values=[1, 1, 1, 0], rational_values=[])
        v_builder.add_state(state=6, boolean_values=[], integer_values=[2, 0, 0, 1], rational_values=[])
        v_builder.add_state(state=7, boolean_values=[], integer_values=[2, 0, 1, 0], rational_values=[])
        v_builder.add_state(state=8, boolean_values=[], integer_values=[1, 1, 1, 1], rational_values=[])
        v_builder.add_state(state=9, boolean_values=[], integer_values=[2, 0, 1, 1], rational_values=[])
        v_builder.add_state(state=10, boolean_values=[], integer_values=[2, 1, 0, 1], rational_values=[])
        v_builder.add_state(state=11, boolean_values=[], integer_values=[2, 1, 1, 1], rational_values=[])

        state_valuations = v_builder.build(nr_states)

        # set rate_transitions to True: the transition values are interpreted as rates
        components = stormpy.SparseModelComponents(transition_matrix=transition_matrix, state_labeling=state_labeling,
                                                   reward_models=reward_models, rate_transitions=True)
        components.choice_labeling = choice_labeling
        components.exit_rates = exit_rates
        components.state_valuations = state_valuations

        ctmc = stormpy.storage.SparseCtmc(components)
        assert type(ctmc) is stormpy.SparseCtmc
        assert not ctmc.supports_parameters

        # Test transition matrix
        assert ctmc.nr_choices == nr_choices
        assert ctmc.nr_states == nr_states
        assert ctmc.nr_transitions == 22
        assert ctmc.transition_matrix.nr_columns == nr_states
        assert ctmc.transition_matrix.nr_rows == nr_choices
        for e in ctmc.transition_matrix:
            assert e.value() == 0.5 or e.value() == 0 or e.value() == 200 or e.value() == 1.0
        for state in ctmc.states:
            assert len(state.actions) <= 1

        # Test state_labeling
        assert ctmc.labeling.get_labels() == {'target', 'init', 'deadlock'}

        # Test reward_models
        assert len(ctmc.reward_models) == 2
        assert not ctmc.reward_models["served"].has_state_rewards
        assert ctmc.reward_models["served"].has_state_action_rewards
        assert ctmc.reward_models["served"].state_action_rewards == [0.0, 0.0, 0.0, 0.0, 0.0, 0.6666666666666666, 0.0,
                                                                     0.0, 1.0, 0.0,
                                                                     0.0, 0.0]
        assert not ctmc.reward_models["served"].has_transition_rewards

        assert ctmc.reward_models["waiting"].has_state_rewards
        assert not ctmc.reward_models["waiting"].has_state_action_rewards
        assert ctmc.reward_models["waiting"].state_rewards == [0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0,
                                                               1.0]
        assert not ctmc.reward_models["waiting"].has_transition_rewards

        # Test choice_labeling
        assert ctmc.has_choice_labeling()
        assert ctmc.choice_labeling.get_labels() == {'loop1a', 'loop1b', 'serve1', 'loop2a', 'loop2b', 'serve2'}

        # Test state_valuations
        assert ctmc.has_state_valuations()
        assert ctmc.state_valuations
        value_s = [None] * nr_states
        value_a = [None] * nr_states
        value_s1 = [None] * nr_states
        value_s2 = [None] * nr_states
        for s in range(0, ctmc.nr_states):
            value_s[s] = ctmc.state_valuations.get_integer_value(s, var_s)
            value_a[s] = ctmc.state_valuations.get_integer_value(s, var_a)
            value_s1[s] = ctmc.state_valuations.get_integer_value(s, var_s1)
            value_s2[s] = ctmc.state_valuations.get_integer_value(s, var_s2)
        assert value_s == [1, 1, 1, 2, 1, 1, 2, 2, 1, 2, 2, 2]
        assert value_a == [0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1]
        assert value_s1 == [0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1]
        assert value_s2 == [0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1]

        # Test choice_origins
        assert not ctmc.has_choice_origins()

        # Test exit_rates
        assert ctmc.exit_rates == [201.0, 200.5, 200.5, 201.0, 200.0, 1.5, 200.5, 200.5, 1.0, 200.0, 1.5, 1.0]

    def test_build_ma_from_model_components(self):
        nr_states = 5
        nr_choices = 10

        # Build transition_matrix
        builder = stormpy.SparseMatrixBuilder(rows=0, columns=0, entries=0, force_dimensions=False,
                                              has_custom_row_grouping=True, row_groups=0)

        # Row group, state 0
        builder.new_row_group(0)
        # Add Transition for (state) 0 to target states
        builder.add_next_value(0, 2, 1)
        builder.add_next_value(1, 2, 1)
        builder.add_next_value(2, 0, 0.8)
        builder.add_next_value(2, 1, 0.2)

        # Row group, state 1
        builder.new_row_group(3)
        # New Transition (state) 1 to target state
        builder.add_next_value(3, 3, 1)

        # Row group, state 2
        builder.new_row_group(4)
        # New Transition (state) 1 to target state
        builder.add_next_value(4, 0, 0.9)
        builder.add_next_value(4, 4, 0.1)

        # Row group, state 3
        builder.new_row_group(5)
        # New Transition (state) 1 to target state
        builder.add_next_value(5, 4, 1)
        builder.add_next_value(6, 3, 1)

        # Row group, state 4
        builder.new_row_group(7)
        # New Transition (state) 1 to target state
        builder.add_next_value(7, 3, 0.5)
        builder.add_next_value(7, 4, 0.5)
        builder.add_next_value(8, 3, 1)
        builder.add_next_value(9, 4, 1)

        transition_matrix = builder.build(nr_choices, nr_states)

        # state_labeling
        state_labeling = stormpy.storage.StateLabeling(nr_states)
        # Add labels
        state_labels = {'init', 'deadlock'}
        for label in state_labels:
            state_labeling.add_label(label)

        # Add label to states
        state_labeling.add_label_to_state('init', 0)

        # state_valuations
        manager = stormpy.ExpressionManager()
        var_s = manager.create_integer_variable(name='s')
        v_builder = stormpy.StateValuationsBuilder()
        v_builder.add_variable(var_s)

        v_builder.add_state(state=0, boolean_values=[], integer_values=[0], rational_values=[])
        v_builder.add_state(state=1, boolean_values=[], integer_values=[2], rational_values=[])
        v_builder.add_state(state=2, boolean_values=[], integer_values=[1], rational_values=[])
        v_builder.add_state(state=3, boolean_values=[], integer_values=[4], rational_values=[])
        v_builder.add_state(state=4, boolean_values=[], integer_values=[3], rational_values=[])

        state_valuations = v_builder.build(nr_states)

        # choice origins:
        prism_program = stormpy.parse_prism_program(get_example_path("ma", "hybrid_states.ma"))
        index_to_identifier_mapping = [1, 2, 3, 4, 5, 6, 7, 8, 9,
                                       10]
        id_to_command_set_mapping = [stormpy.FlatSet() for _ in range(11)]

        id_to_command_set_mapping[1].insert(2)
        id_to_command_set_mapping[2].insert(1)
        id_to_command_set_mapping[3].insert(0)
        id_to_command_set_mapping[4].insert(4)
        id_to_command_set_mapping[5].insert(3)
        id_to_command_set_mapping[6].insert(9)
        id_to_command_set_mapping[7].insert(8)
        id_to_command_set_mapping[8].insert(7)
        id_to_command_set_mapping[9].insert(6)
        id_to_command_set_mapping[10].insert(5)

        choice_origins = stormpy.PrismChoiceOrigins(prism_program, index_to_identifier_mapping,
                                                    id_to_command_set_mapping)

        exit_rates = [3.0, 12.0, 10.0, 3.0, 4.0]

        markovian_states = stormpy.BitVector(5, [0, 1, 2, 3, 4])

        # Build components, set rate_transitions to False
        components = stormpy.SparseModelComponents(transition_matrix=transition_matrix, state_labeling=state_labeling,
                                                   rate_transitions=False, markovian_states=markovian_states)
        components.state_valuations = state_valuations
        components.choice_origins = choice_origins
        components.exit_rates = exit_rates

        ma = stormpy.storage.SparseMA(components)
        assert type(ma) is stormpy.SparseMA
        assert not ma.supports_parameters

        # Test transition matrix
        assert ma.nr_choices == nr_choices
        assert ma.nr_states == nr_states
        assert ma.nr_transitions == 13
        assert ma.transition_matrix.nr_columns == nr_states
        assert ma.transition_matrix.nr_rows == nr_choices
        # Check row groups
        assert ma.transition_matrix.get_row_group_start(0) == 0
        assert ma.transition_matrix.get_row_group_end(0) == 3
        assert ma.transition_matrix.get_row_group_start(1) == 3
        assert ma.transition_matrix.get_row_group_end(1) == 4
        assert ma.transition_matrix.get_row_group_start(2) == 4
        assert ma.transition_matrix.get_row_group_end(2) == 5
        assert ma.transition_matrix.get_row_group_start(3) == 5
        assert ma.transition_matrix.get_row_group_end(3) == 7
        assert ma.transition_matrix.get_row_group_start(4) == 7
        assert ma.transition_matrix.get_row_group_end(4) == 10

        for e in ma.transition_matrix:
            assert e.value() == 1.0 or e.value() == 0 or e.value() == 0.8 or e.value() == 0.2 or e.value() == 0.1 or e.value() == 0.5 or e.value() == 0.9
        for state in ma.states:
            assert len(state.actions) <= 3

        # Test state_labeling
        assert ma.labeling.get_labels() == {'deadlock', 'init'}

        # Test reward_models
        assert len(ma.reward_models) == 0

        # Test choice_labeling
        assert not ma.has_choice_labeling()

        # Test state_valuations
        assert ma.has_state_valuations()

        value_s = [None] * nr_states
        for s in range(0, ma.nr_states):
            value_s[s] = ma.state_valuations.get_integer_value(s, var_s)
        assert value_s == [0, 2, 1, 4, 3]

        # Test choice_origins
        assert ma.has_choice_origins()
        assert ma.choice_origins.get_number_of_identifiers() == 11

        # Test exit_rates
        assert ma.exit_rates == [3.0, 12.0, 10.0, 3.0, 4.0]

        # Test markovian states
        assert ma.markovian_states == stormpy.BitVector(5, [0, 1, 2, 3, 4])
