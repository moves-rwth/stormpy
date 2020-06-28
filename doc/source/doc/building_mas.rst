**************************************
Markov automata (MAs)
**************************************


Background
=====================

We already saw the process of building CTMCs with MDPs via Stormpy in :doc:`building_ctmcs` and :doc:`building_mdps`.
In this section, we build a small Markarov automata with five states from which the first four are markovian.
Since we covered labeling and exit rates already in the previous examples we omit the description of these components.
The full example can be found here:

.. seealso:: `01-building-mas.py <todo /examples/building_mas/01-building-mas.py>`

First, we import Stormpy::

    >>>	import stormpy

Transition Matrix
==================
In :doc:`building_mdps`, we used the SparseMatrixBuilder to create a matrix with a custom row grouping.
In this example, we use the numpy library:

In the beginning, we create a numpy array that will be used to build the transition matrix of our model.::

    >>> import numpy as np
    >>> transitions = np.array([
    ...        [0, 1, 0, 0, 0],
    ...        [0.8, 0, 0.2, 0, 0],
    ...        [0.9, 0, 0, 0.1, 0],
    ...        [0, 0, 0, 0, 1],
    ...        [0, 0, 0, 1, 0],
    ...        [0, 0, 0, 0, 1]], dtype='float64')

When building the matrix we define a custom row grouping by passing a list containing the starting row of each row group in ascending order::

    >>> transition_matrix = stormpy.build_sparse_matrix(transitions, [0, 2, 3, 4, 5])
    >>> print(transition_matrix)

            0	1	2	3	4
        ---- group 0/4 ----
    0	(	0	1	0	0	0	)	0
    1	(	0.8	0	0.2	0	0	)	1
        ---- group 1/4 ----
    2	(	0.9	0	0	0.1	0	)	2
        ---- group 2/4 ----
    3	(	0	0	0	0	1	)	3
        ---- group 3/4 ----
    4	(	0	0	0	1	0	)	4
        ---- group 4/4 ----
    5	(	0	0	0	0	1	)	5
            0	1	2	3	4


Markovian States
==================
Feature of MA: Markovian states.
Those are represented by a BitVector that containing the respective states::

    >>> markovian_states =  stormpy.BitVector(5, [1, 2, 3, 4])

Building the Model
====================
Now, we can collect all components::

    >>> components = stormpy.SparseModelComponents(transition_matrix=transition_matrix, state_labeling=state_labeling,
                                               markovian_states=markovian_states)
    >>> components.choice_labeling = choice_labeling
    >>> components.exit_rates = exit_rates

Finally, we can build the model::

    >>> ma = stormpy.storage.SparseMA(components)
    >>> print(ma)
    --------------------------------------------------------------
    Model type: 	Markov Automaton (sparse)
    States: 	5
    Transitions: 	8
    Choices: 	6
    Markovian St.: 	4
    Max. Rate.: 	12
    Reward Models:  none
    State Labels: 	2 labels
       * deadlock -> 0 item(s)
       * init -> 1 item(s)
    Choice Labels: 	2 labels
       * alpha -> 1 item(s)
       * beta -> 1 item(s)
    --------------------------------------------------------------
