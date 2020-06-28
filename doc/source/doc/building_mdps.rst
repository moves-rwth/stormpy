***********************************************
Discrete-time Markov decision processes (MDPs)
***********************************************

Background
=====================

In :doc:`building_dtmcs` we modelled Knuth's model of a fair die by the means of a DTMC.
In the following we extend this model with nondeterministic choice by building a Markov Decision process.

.. seealso:: `01-building-mdps.py <todo /examples/mdps/01-building-mdps.py>`

First, we import Stormpy::

    >>>	import stormpy

Transition Matrix
=====================
Since we want build a nondeterminstic model, we create a transition matrix with a custom row group for each state::

    >>> builder = stormpy.SparseMatrixBuilder(rows=0, columns=0, entries=0, force_dimensions=False, has_custom_row_grouping=True, row_groups=0)

We need more than one row for the transitions starting in state 0 because a nondeterministic choice over the actions is available.
Therefore, we start a new group that will contain the rows representing actions of state 0.
Note that the row group needs to be added before any entries are added to the group::

    >>> builder.new_row_group(0)
    >>> builder.add_next_value(0, 1, 0.5)
    >>> builder.add_next_value(0, 2, 0.5)
    >>> builder.add_next_value(1, 1, 0.2)
    >>> builder.add_next_value(1, 2, 0.8)

For the remaining states, we need to specify the starting rows of row groups::

    >>> builder.new_row_group(2)
    >>> builder.add_next_value(2, 3, 0.5)
    >>> builder.add_next_value(2, 4, 0.5)
    >>> builder.new_row_group(3)
    >>> builder.add_next_value(3, 5, 0.5)
    >>> builder.add_next_value(3, 6, 0.5)
    >>> builder.new_row_group(4)
    >>> builder.add_next_value(4, 7, 0.5)
    >>> builder.add_next_value(4, 1, 0.5)
    >>> builder.new_row_group(5)
    >>> builder.add_next_value(5, 8, 0.5)
    >>> builder.add_next_value(5, 9, 0.5)
    >>> builder.new_row_group(6)
    >>> builder.add_next_value(6, 10, 0.5)
    >>> builder.add_next_value(6, 11, 0.5)
    >>> builder.new_row_group(7)
    >>> builder.add_next_value(7, 2, 0.5)
    >>> builder.add_next_value(7, 12, 0.5)

    >>> for s in range(8, 14):
    ...    builder.new_row_group(s)
    ...    builder.add_next_value(s, s - 1, 1)

Build::

    >>> transition_matrix = builder.build()

Labeling
================
We have seen the construction of a state labeling in previous examples. Therefore we omit the description here.

Instead we focus on the choices.
Since in state 0 a nondeterministic choice over two actions is available
The number of choices is 14.
To distinguish those we can define a choice labeling::

    >>> choice_labeling = stormpy.storage.ChoiceLabeling(14)
    >>> choice_labels = {'a', 'b'}

    >>> for label in choice_labels:
    ...    choice_labeling.add_label(label)

We assign the label 'a' to the first action of state 0 and 'b' to the second.
Recall that those actions where defined in row one and two of the transition matrix respectively::

    >>> choice_labeling.add_label_to_choice('a', 0)
    >>> choice_labeling.add_label_to_choice('b', 1)
    >>> print(choice_labeling)
    Choice 2 labels
       * a -> 1 item(s)
       * b -> 1 item(s)


Reward models
==================
Reward models, length of vector coincides with number of choices::

    >>> reward_models = {}
    >>> action_reward = [0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
    >>> reward_models['coin_flips'] = stormpy.SparseRewardModel(optional_state_action_reward_vector=action_reward)

Building the Model
====================
Collect components::

    >>> components = stormpy.SparseModelComponents(transition_matrix=transition_matrix, state_labeling=state_labeling, reward_models=reward_models, rate_transitions=False)
    >>> components.choice_labeling = choice_labeling

Build the model::

    >>> mdp = stormpy.storage.SparseMdp(components)
    >>> print(mdp)
    Model type: 	MDP (sparse)
    States: 	13
    Transitions: 	22
    Choices: 	14
    Reward Models:  coin_flips
    State Labels: 	9 labels
       * one -> 1 item(s)
       * six -> 1 item(s)
       * three -> 1 item(s)
       * four -> 1 item(s)
       * done -> 6 item(s)
       * init -> 1 item(s)
       * five -> 1 item(s)
       * deadlock -> 0 item(s)
       * two -> 1 item(s)
    Choice Labels: 	2 labels
       * a -> 1 item(s)
       * b -> 1 item(s)

Partially observable Markov decision process (POMDPs)
========================================================

To build a partially observable Markov decision process,
components.observations can be set to a list of numbers that defines the status of the observables in each state.
