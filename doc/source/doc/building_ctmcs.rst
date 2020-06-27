**************************************
Continuous-time Markov chains (CTMCs)
**************************************


Background
=====================

In this section, we explain how Stormpy can be used to build a CTMC representing a cyclic server polling system.
Building CTMCs works similar to building DTMCs as in :doc:`building_dtmcs`, but additionally every state is equipped with an exit rate.

.. seealso:: `01-building-ctmcs.py <todo /examples/building_ctmcs/01-building-ctmcs.py>`

First, we import Stormpy::

    >>>	import stormpy

Transition Matrix
=====================
We build the transition matrix using numpy. As an alternative, the SparseMatrixBuilder can be used here::

    >>> import numpy as np

    >>> transitions = np.array([
    ...     [0, 0.5, 0.5, 200, 0, 0, 0, 0, 0, 0, 0, 0],
    ...     [0, 0, 0, 0, 0.5, 200, 0, 0, 0, 0, 0, 0],
    ...     [0, 0, 0, 0, 0.5, 0, 200, 0, 0, 0, 0, 0],
    ...     [200, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0],
    ...     [0, 0, 0, 0, 0, 0, 0, 0, 200, 0, 0, 0],
    ...     [0, 0, 0, 1, 0, 0, 0, 0, 0.5, 0, 0, 0],
    ...     [0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 200, 0],
    ...     [0, 200, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0],
    ...     [0, 0, 0, 0, 0, 0, 1, 0, 0, 0 ,0, 0],
    ...     [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200],
    ...     [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5	],
    ...     [0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]], dtype='float64')

We do not specify the row_group_indices. Thus, the following function call returns a sparse matrix with default row groups.::

    >>> transition_matrix = stormpy.build_sparse_matrix(transitions)

Labeling
================
Next, we create the state labeling::

    >>> state_labeling = stormpy.storage.StateLabeling(12)
    >>> state_labels = {'init', 'deadlock', 'target'}
    >>> for label in state_labels:
    ...    state_labeling.add_label(label)
    >>> state_labeling.add_label_to_state('init', 0)
    >>> state_labeling.set_states('target', stormpy.BitVector(12, [5, 8]))

For the choice labeling this works similar::

    >>> nr_choices = 12
    >>> choice_labeling = stormpy.storage.ChoiceLabeling(nr_choices)
    >>> choice_labels = {'loop1a', 'loop1b', 'serve1', 'loop2a', 'loop2b', 'serve2'}
    >>> for label in choice_labels:
    ...     choice_labeling.add_label(label)

To set the same label for multiple choices, we can use a BitVector::

    >>> choice_labeling.set_choices('loop1a', stormpy.BitVector(nr_choices, [0, 2]))
    >>> choice_labeling.set_choices('loop1b', stormpy.BitVector(nr_choices, [1, 4]))
    >>> choice_labeling.set_choices('serve1', stormpy.BitVector(nr_choices, [5, 8]))
    >>> choice_labeling.set_choices('loop2a', stormpy.BitVector(nr_choices, [3, 7]))
    >>> choice_labeling.set_choices('loop2b', stormpy.BitVector(nr_choices, [6, 9]))
    >>> choice_labeling.set_choices('serve2', stormpy.BitVector(nr_choices, [10, 11]))

Reward models
==================

Now, we create the reward models, beginning with the reward model named 'served' which has state-action rewards::

    >>> reward_models = {}
    >>> action_reward = [0.0, 0.0, 0.0, 0.0, 0.0, 2/3, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0]
    >>> reward_models['served'] = stormpy.SparseRewardModel(optional_state_action_reward_vector = action_reward)

In the same way we can create a second reward model. This time we consider state rewards::

    >>> state_reward = [0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0]
    >>> reward_models['waiting'] = stormpy.SparseRewardModel(optional_state_reward_vector = state_reward)

Exit Rates
====================
Lastly, we equip every state with an exit rate::

    >>> exit_rates =  [201.0, 200.5, 200.5, 201.0, 200.0, 1.5, 200.5, 200.5, 1.0, 200.0, 1.5, 1.0]

Building the Model
====================

Now, we can collect all components, including the choice labeling and the exit rates.
To let the transition values be interpreted as rates we set rate_transitions to True::

    >>> components = stormpy.SparseModelComponents(transition_matrix=transition_matrix, state_labeling=state_labeling, reward_models=reward_models, rate_transitions = True)
    >>> components.choice_labeling = choice_labeling
    >>> components.exit_rates = exit_rates

And finally, we can build the model::

    >>> ctmc = stormpy.storage.SparseCtmc(components)
    >>> print(ctmc)
    --------------------------------------------------------------
    Model type: 	CTMC (sparse)
    States: 	12
    Transitions: 	22
    Reward Models:  waiting, served
    State Labels: 	3 labels
       * init -> 1 item(s)
       * deadlock -> 0 item(s)
       * target -> 2 item(s)
    Choice Labels: 	6 labels
       * loop2a -> 2 item(s)
       * serve1 -> 2 item(s)
       * serve2 -> 2 item(s)
       * loop2b -> 2 item(s)
       * loop1b -> 2 item(s)
       * loop1a -> 2 item(s)
    --------------------------------------------------------------


