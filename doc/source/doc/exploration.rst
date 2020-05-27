****************
Exploring Models
****************

Background
=====================

Often, stormpy is used as a testbed for new algorithms.
An essential step is to transfer the (low-level) descriptions of an MDP or other state-based model into
an own algorithm. In this section, we discuss some of the functionality.

Reading MDPs
=====================

.. seealso:: `01-exploration.py <https://github.com/moves-rwth/stormpy/blob/master/examples/exploration/01-exploration.py>`_

In :doc:`../getting_started`, we briefly iterated over a DTMC. In this section, we explore an MDP::

    >>> import doctest
    >>> doctest.ELLIPSIS_MARKER = '-etc-' # doctest:+ELLIPSIS
    >>> import stormpy
    >>> import stormpy.examples
    >>> import stormpy.examples.files
    >>> program = stormpy.parse_prism_program(stormpy.examples.files.prism_mdp_maze)
    >>> prop = "R=? [F \"goal\"]"

    >>> properties = stormpy.parse_properties_for_prism_program(prop, program, None)
    >>> model = stormpy.build_model(program, properties)

The iteration over the model is as before, but now, for every action, we can have several transitions::

    >>> for state in model.states:
    ...    if state.id in model.initial_states:
    ...        print("State {} is initial".format(state.id))
    ...    for action in state.actions:
    ...        for transition in action.transitions:
    ...            print("From state {} by action {}, with probability {}, go to state {}".format(state, action, transition.value(), transition.column))
    -etc-

The output (omitted for brievety) contains sentences like::

    From state 1 by action 0, with probability 1.0, go to state 2
    From state 1 by action 1, with probability 1.0, go to state 1



Internally, storm can hold hints to the origin of the actions, which may be helpful to give meaning and for debugging.
As the availability and the encoding of this data depends on the input model, we discuss these features in :doc:`highlevel_models`.


Storm currently supports deterministic rewards on states or actions. More information can be found in :doc:`reward_models`.


Reading POMDPs
======================
.. seealso:: `02-exploration.py <https://github.com/moves-rwth/stormpy/blob/master/examples/exploration/02-exploration.py>`_


Internally, POMDPs extend MDPs. Thus, iterating over the MDP is done as before.

    >>> import stormpy
    >>> import stormpy.examples
    >>> import stormpy.examples.files
    >>> program = stormpy.parse_prism_program(stormpy.examples.files.prism_pomdp_maze)
    >>> prop = "R=? [F \"goal\"]"
    >>> properties = stormpy.parse_properties_for_prism_program(prop, program, None)
    >>> model = stormpy.build_model(program, properties)

Indeed, all that changed in the code above is the example we use.
And, that the model type now is a POMDP::

    >>> print(model.model_type)
    ModelType.POMDP

Additionally, POMDPs have a set of observations, which are internally just numbered by an integer from 0 to the number of observations -1 ::

    >>> print(model.nr_observations)
    8
    >>> for state in model.states:
    ...     print("State {} has observation id {}".format(state.id, model.observations[state.id]))
    State 0 has observation id 6
    State 1 has observation id 1
    State 2 has observation id 4
    State 3 has observation id 7
    State 4 has observation id 4
    State 5 has observation id 3
    State 6 has observation id 0
    State 7 has observation id 0
    State 8 has observation id 0
    State 9 has observation id 0
    State 10 has observation id 0
    State 11 has observation id 0
    State 12 has observation id 2
    State 13 has observation id 2
    State 14 has observation id 5


Sorting states
==============
.. seealso:: `03-exploration.py <https://github.com/moves-rwth/stormpy/blob/master/examples/exploration/03-exploration.py>`_


Often, one may sort the states according to the graph structure.
Storm supports some of these sorting algorithms, e.g., topological sort.





Reading MAs
======================

To be continued...
