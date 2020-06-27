import stormpy
import numpy as np

# polling example [IT90]
# gxn/dxp 26/01/00
def example_building_ctmcs_01():

    # Building the transition matrix using numpy
    transitions = np.array([
        [0, 0.5, 0.5, 200, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0.5, 200, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0.5, 0, 200, 0, 0, 0, 0, 0],
        [200, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 200, 0, 0, 0],
        [0, 0, 0, 1, 0, 0, 0, 0, 0.5, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 200, 0],
        [0, 200, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0],
        [0, 0, 0, 0, 0, 0, 1, 0, 0, 0 ,0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200],
        [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5	],
        [0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    ], dtype='float64')

    # Default row groups: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]
    transition_matrix = stormpy.build_sparse_matrix(transitions)


    # State labeling
    state_labeling = stormpy.storage.StateLabeling(12)
    state_labels = {'init', 'deadlock', 'target'}
    for label in state_labels:
        state_labeling.add_label(label)

    # Adding label to states
    state_labeling.add_label_to_state('init', 0)
    # Sets the labeling of states given in a BitVector (length: nr_states)
    state_labeling.set_states('target', stormpy.BitVector(12, [5, 8]))

    # Choice labeling
    nr_choices = 12
    choice_labeling = stormpy.storage.ChoiceLabeling(nr_choices)
    choice_labels = {'loop1a', 'loop1b', 'serve1', 'loop2a', 'loop2b', 'serve2'}
    for label in choice_labels:
        choice_labeling.add_label(label)
    # Sets the labeling of states given in a bit vector (length: nr_choices)
    choice_labeling.set_choices('loop1a', stormpy.BitVector(nr_choices, [0, 2]))
    choice_labeling.set_choices('loop1b', stormpy.BitVector(nr_choices, [1, 4]))
    choice_labeling.set_choices('serve1', stormpy.BitVector(nr_choices, [5, 8]))
    choice_labeling.set_choices('loop2a', stormpy.BitVector(nr_choices, [3, 7]))
    choice_labeling.set_choices('loop2b', stormpy.BitVector(nr_choices, [6, 9]))
    choice_labeling.set_choices('serve2', stormpy.BitVector(nr_choices, [10, 11]))

    # Reward models:
    reward_models = {}
    # Create a vector representing the state-action rewards
    action_reward = [0.0, 0.0, 0.0, 0.0, 0.0, 2/3, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0]
    reward_models['served'] = stormpy.SparseRewardModel(optional_state_action_reward_vector = action_reward)

    ## Create a vector representing the state rewards
    state_reward = [0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0]
    reward_models['waiting'] = stormpy.SparseRewardModel(optional_state_reward_vector = state_reward)

    # Exit rate for each state
    exit_rates =  [201.0, 200.5, 200.5, 201.0, 200.0, 1.5, 200.5, 200.5, 1.0, 200.0, 1.5, 1.0]

    # Collect components, rate_transitions = True, because the transition values are interpreted as rates (CTMC specific)
    components = stormpy.SparseModelComponents(transition_matrix=transition_matrix, state_labeling=state_labeling, reward_models=reward_models, rate_transitions = True)
    components.choice_labeling = choice_labeling
    components.exit_rates = exit_rates

    # Build the model
    ctmc = stormpy.storage.SparseCtmc(components)

    print(ctmc)

if __name__ == '__main__':
    example_building_ctmcs_01()