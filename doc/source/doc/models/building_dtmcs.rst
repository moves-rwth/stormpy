************************************
Discrete-time Markov chains (DTMCs)
************************************


Background
=====================
As described in :doc:`../../getting_started`,
Storm can be used to translate a model description e.g. in form of a prism file into a Markov chain.

Here, we use Stormpy to create the components for a model and build a DTMC directly from these components without parsing a model description.
We consider the previous example of the Knuth-Yao die.

.. seealso:: `01-building-dtmcs.py <https://github.com/moves-rwth/stormpy/blob/master/examples/building_dtmcs/01-building-dtmcs.py>`_

In the following we create the transition matrix, the state labeling and the reward models of a DTMC.
First, we import stormpy::

    >>>	import stormpy

Transition Matrix
=====================
We begin by creating the matrix representing the transitions in the model in terms of probabilities.
For constructing the transition matrix, we use the SparseMatrixBuilder::

    >>> builder = stormpy.SparseMatrixBuilder(rows = 0, columns = 0, entries = 0, force_dimensions = False, has_custom_row_grouping = False)

Here, we start with an empty matrix to later insert more entries.
If the number of rows, columns and entries is known, the matrix can be constructed using these values.

For DTMCs each state has at most one outgoing probability distribution.
Thus, we create a matrix with trivial row grouping where each group contains one row representing the state action.
In :doc:`building_mdps` we will revisit the example of the die, but extend the model with nondeterministic choice.

We specify the transitions of the model by adding values to the matrix where the column represents the target state.
All transitions are equipped with a probability defined by the value::

    >>> builder.add_next_value(row = 0, column = 1, value = 0.5)
    >>> builder.add_next_value(0, 2, 0.5)
    >>> builder.add_next_value(1, 3, 0.5)
    >>> builder.add_next_value(1, 4, 0.5)
    >>> builder.add_next_value(2, 5, 0.5)
    >>> builder.add_next_value(2, 6, 0.5)
    >>> builder.add_next_value(3, 7, 0.5)
    >>> builder.add_next_value(3, 1, 0.5)
    >>> builder.add_next_value(4, 8, 0.5)
    >>> builder.add_next_value(4, 9, 0.5)
    >>> builder.add_next_value(5, 10, 0.5)
    >>> builder.add_next_value(5, 11, 0.5)
    >>> builder.add_next_value(6, 2, 0.5)
    >>> builder.add_next_value(6, 12, 0.5)

Lastly, we add a self-loop with probability one to the final states::

    >>> for s in range(7,13):
    ...    builder.add_next_value(s, s, 1)


Finally, we can build the matrix::

    >>> transition_matrix = builder.build()

It should be noted that entries can only be inserted in ascending order, i.e. row by row and column by column.
Stormpy provides the possibility to build a sparse matrix using the numpy library (https://numpy.org/ )
Instead of using the SparseMatrixBuilder, a sparse matrix can be build from a numpy array via the method `stormpy.build_sparse_matrix`.
An example is given in :ref:`building CTMCs <doc/models/building_ctmcs:Transition Matrix>`.

Labeling
====================

States can be labeled with sets of propositions, for example state 0 can be labeled with "init".
In order to specify the state labeling we create an empty labeling for the given number of states and add the labels to the labeling::

    >>> state_labeling = stormpy.storage.StateLabeling(13)

    >>> labels = {'init', 'one', 'two', 'three', 'four', 'five', 'six', 'done', 'deadlock'}
    >>> for label in labels:
    ...     state_labeling.add_label(label)


Labels can be asociated with states. As an example, we label the state 0 with "init"::

    >>> state_labeling.add_label_to_state('init', 0)
    >>> print(state_labeling.get_states('init'))
    bit vector(1/13) [0]

Next, we set the associations between the remaining labels and states.::

    >>> state_labeling.add_label_to_state('one', 7)
    >>> state_labeling.add_label_to_state('two', 8)
    >>> state_labeling.add_label_to_state('three', 9)
    >>> state_labeling.add_label_to_state('four', 10)
    >>> state_labeling.add_label_to_state('five', 11)
    >>> state_labeling.add_label_to_state('six', 12)

To set the same label for multiple states, we can use a BitVector representation for the set of states::

    >>> state_labeling.set_states('done', stormpy.BitVector(13, [7, 8, 9, 10, 11, 12]))
    >>> print(state_labeling)
    9 labels
       * one -> 1 item(s)
       * four -> 1 item(s)
       * done -> 6 item(s)
       * three -> 1 item(s)
       * init -> 1 item(s)
       * two -> 1 item(s)
       * six -> 1 item(s)
       * deadlock -> 0 item(s)
       * five -> 1 item(s)

Defining a choice labeling is possible in a similar way.

Reward Models
====================
Stormpy supports multiple reward models such as state rewards, state-action rewards and as transition rewards.
In this example, the actions of states which satisfy `s<7` acquire a reward of 1.0.

The state-action rewards are represented by a vector, which is associated to a reward model named "coin_flips"::

    >>> reward_models = {}
    >>> action_reward = [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
    >>> reward_models['coin_flips'] = stormpy.SparseRewardModel(optional_state_action_reward_vector = action_reward)

Building the Model
====================

Next, we collect all components::

    >>> components = stormpy.SparseModelComponents(transition_matrix=transition_matrix, state_labeling=state_labeling, reward_models=reward_models)

And finally, we can build the DTMC::

    >>> dtmc = stormpy.storage.SparseDtmc(components)
    >>> print(dtmc)
    --------------------------------------------------------------
    Model type: 	DTMC (sparse)
    States: 	13
    Transitions: 	20
    Reward Models:  coin_flips
    State Labels: 	9 labels
       * three -> 1 item(s)
       * six -> 1 item(s)
       * done -> 6 item(s)
       * four -> 1 item(s)
       * five -> 1 item(s)
       * deadlock -> 0 item(s)
       * init -> 1 item(s)
       * two -> 1 item(s)
       * one -> 1 item(s)
    Choice Labels: 	none
    --------------------------------------------------------------
