**************************************
Continuous-time Markov chains (CTMCs)
**************************************


Background
=====================

In this section, we explain how Stormpy can be used to build a simple CTMC.
Building CTMCs works similar to building DTMCs as in :doc:`building_dtmcs`, but additionally every state is equipped with an exit rate.

.. seealso:: `01-building-ctmcs.py <https://github.com/moves-rwth/stormpy/blob/master/examples/building_ctmcs/01-building-ctmcs.py>`_

First, we import Stormpy::

    >>>	import stormpy

Transition Matrix
=====================
In this example, we build the transition matrix using a numpy array::

    >>> import numpy as np

    >>> transitions = np.array([
    ...    [0, 1.5, 0, 0],
    ...    [3, 0, 1.5, 0],
    ...    [0, 3, 0, 1.5],
    ...    [0, 0, 3, 0], ], dtype='float64')

The following function call returns a sparse matrix with default row groups::

    >>> transition_matrix = stormpy.build_sparse_matrix(transitions)
    >>> print(transition_matrix)
            0	1	2	3
        ---- group 0/3 ----
    0	(	0	1.5	0	0	)	0
        ---- group 1/3 ----
    1	(	3	0	1.5	0	)	1
        ---- group 2/3 ----
    2	(	0	3	0	1.5	)	2
        ---- group 3/3 ----
    3	(	0	0	3	0	)	3
            0	1	2	3


Labeling
================
The state labeling is created analogously to the previous example in :ref:`building DTMCs<doc/models/building_dtmcs:Labeling>`.

    >>> state_labeling = stormpy.storage.StateLabeling(4)
    >>> state_labels = {'empty', 'init', 'deadlock', 'full'}
    >>> for label in state_labels:
    ...    state_labeling.add_label(label)
    >>> state_labeling.add_label_to_state('init', 0)
    >>> state_labeling.add_label_to_state('empty', 0)
    >>> state_labeling.add_label_to_state('full', 3)

Exit Rates
====================
Lastly, we initialize a list to equip every state with an exit rate::

    >>> exit_rates = [1.5, 4.5, 4.5, 3.0]

Building the Model
====================

Now, we can collect all components, including the choice labeling and the exit rates.
To let the transition values be interpreted as rates we set `rate_transitions` to `True`::

    components = stormpy.SparseModelComponents(transition_matrix=transition_matrix, state_labeling=state_labeling, rate_transitions=True)
    components.exit_rates = exit_rates

And finally, we can build the model::

    >>> ctmc = stormpy.storage.SparseCtmc(components)
    >>> print(ctmc)
    --------------------------------------------------------------
    Model type: 	CTMC (sparse)
    States: 	4
    Transitions: 	6
    Reward Models:  none
    State Labels: 	4 labels
       * init -> 1 item(s)
       * empty -> 1 item(s)
       * deadlock -> 0 item(s)
       * full -> 1 item(s)
    Choice Labels: 	none
    --------------------------------------------------------------


