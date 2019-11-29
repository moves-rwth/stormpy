****************************
Getting Started
****************************

Before starting with this guide, one should follow the instructions for :doc:`installation`.

A Quick Tour through Stormpy
================================

This guide is intended for people which have a basic understanding of probabilistic models and their verification. More details and further pointers to literature can be found on the
`Storm website <http://www.stormchecker.org/>`_.
While we assume some very basic programming concepts, we refrain from using more advanced concepts of python throughout the guide.

We start with a selection of high-level constructs in stormpy, and go into more details afterwards. More in-depth examples can be found in the :doc:`advanced_topics`.

.. seealso:: The code examples are also given in the `examples/ <https://github.com/moves-rwth/stormpy/blob/master/examples/>`_ folder. These boxes throughout the text will tell you which example contains the code discussed.

We start by launching the python 3 interpreter::

	$ python3

First we import stormpy::

    >>>	import stormpy
	
Building models 
------------------------------------------------
.. seealso:: `01-getting-started.py <https://github.com/moves-rwth/stormpy/blob/master/examples/01-getting-started.py>`_

There are several ways to create a Markov chain. 
One of the easiest is to parse a description of such a Markov chain and to let Storm build the chain. 

Here, we build a Markov chain from a prism program.
Stormpy comes with a small set of examples, which we use here::

	>>> import stormpy.examples
	>>> import stormpy.examples.files

With this, we can now import the path of our prism file::

	>>> path = stormpy.examples.files.prism_dtmc_die
	>>> prism_program = stormpy.parse_prism_program(path)
	
The ``prism_program`` can be translated into a Markov chain::

    >>> model = stormpy.build_model(prism_program)
    >>> print("Number of states: {}".format(model.nr_states))
    Number of states: 13
    >>> print("Number of transitions: {}".format(model.nr_transitions))
    Number of transitions: 20
    
This tells us that the model has 13 states and 20 transitions.

Moreover, initial states and deadlocks are indicated with a labelling function. We can see the labels present in the model by::

    >>> print("Labels: {}".format(model.labeling.get_labels()))
    Labels: ...
	
We will investigate ways to examine the model in more detail later in :ref:`getting-started-investigating-the-model`.

.. _getting-started-building-properties:

Building properties
--------------------------
.. seealso:: `02-getting-started.py <https://github.com/moves-rwth/stormpy/blob/master/examples/02-getting-started.py>`_

Storm takes properties in the prism-property format. 
To express that one is interested in the reachability of any state where the prism program variable ``s`` is 2, one would formulate::

	P=? [F s=2]

Stormpy can be used to parse this. As the variables in the property refer to a program, the program has to be passed as an additional parameter::

    >>> formula_str = "P=? [F s=2]"
    >>> properties = stormpy.parse_properties(formula_str, prism_program)

Notice that properties is now a list of properties containing a single element. 

However, if we build the model as before, then the appropriate information that the variable ``s=2`` in some states is not present.
In order to label the states accordingly, we should notify Storm upon building the model that we would like to preserve given properties. 
Storm will then add the labels accordingly::

    >>> model = stormpy.build_model(prism_program, properties)
    >>> print("Labels in the model: {}".format(sorted(model.labeling.get_labels())))
    Labels in the model: ['(s = 2)', 'deadlock', 'init']

Model building however now behaves slightly different: Only the properties passed are preserved, which means that model building might skip parts of the model.
In particular, to check the probability of eventually reaching a state ``x`` where ``s=2``, successor states of ``x`` are not relevant::

    >>> print("Number of states: {}".format(model.nr_states))
    Number of states: 8
	
If we consider another property, however, such as::

	P=? [F s=7 & d=2]

then Storm is only skipping exploration of successors of the particular state ``y`` where ``s=7`` and ``d=2``. In this model, state ``y`` has a self-loop, so effectively, the whole model is explored.

.. _getting-started-checking-properties:

Checking properties
------------------------------------
.. seealso:: `03-getting-started.py <https://github.com/moves-rwth/stormpy/blob/master/examples/03-getting-started.py>`_

The last lesson taught us to construct properties and models with matching state labels. 
Now default checking routines are just a simple command away::

    >>> properties = stormpy.parse_properties(formula_str, prism_program)
    >>> model = stormpy.build_model(prism_program, properties)
    >>> result = stormpy.model_checking(model, properties[0])
    
The result may contain information about all states.
Instead, we can iterate over the results::

    >>> assert result.result_for_all_states
    >>> for x in result.get_values():
    ...    pass # do something with x


.. topic:: Results for all states

    Some model checking algorithms do not provide results for all states. In those cases, the result is not valid for all states, and to iterate over them, a different method is required. We will explain this later.

A good way to get the result for the initial states is as follows::

    >>> initial_state = model.initial_states[0]
    >>> print(result.at(initial_state))
    0.5

.. _getting-started-investigating-the-model:

Investigating the model
-------------------------------------
.. seealso:: `04-getting-started.py <https://github.com/moves-rwth/stormpy/blob/master/examples/04-getting-started.py>`_

One powerful part of the Storm model checker is to quickly create the Markov chain from higher-order descriptions, as seen above::

    >>> path = stormpy.examples.files.prism_dtmc_die
    >>> prism_program = stormpy.parse_prism_program(path)
    >>> model = stormpy.build_model(prism_program)

In this example, we will exploit this, and explore the underlying Markov chain of the model.
The most basic question might be what the type of the constructed model is::

    >>> print(model.model_type)
    ModelType.DTMC

We can also directly explore the underlying state space/matrix.
Notice that this code can be applied to both deterministic and non-deterministic models::

    >>> for state in model.states:
    ...    for action in state.actions:
    ...        for transition in action.transitions:
    ...            print("From state {}, with probability {}, go to state {}".format(state, transition.value(), transition.column))
    From state 0, with probability 0.5, go to state 1
    From state 0, with probability 0.5, go to state 2
    From state 1, with probability 0.5, go to state 3
    From state 1, with probability 0.5, go to state 4
    From state 2, with probability 0.5, go to state 5
    From state 2, with probability 0.5, go to state 6
    From state 3, with probability 0.5, go to state 1
    From state 3, with probability 0.5, go to state 7
    From state 4, with probability 0.5, go to state 8
    From state 4, with probability 0.5, go to state 9
    From state 5, with probability 0.5, go to state 10
    From state 5, with probability 0.5, go to state 11
    From state 6, with probability 0.5, go to state 2
    From state 6, with probability 0.5, go to state 12
    From state 7, with probability 1.0, go to state 7
    From state 8, with probability 1.0, go to state 8
    From state 9, with probability 1.0, go to state 9
    From state 10, with probability 1.0, go to state 10
    From state 11, with probability 1.0, go to state 11
    From state 12, with probability 1.0, go to state 12

Let us go into some more details. For DTMCs, each state has (at most) one outgoing probability distribution.
Thus::

    >>> for state in model.states:
    ...    assert len(state.actions) <= 1


We can also check if a state is indeed an initial state. Notice that ``model.initial_states`` contains state ids, not states.::

    >>> for state in model.states:
    ...     if state.id in model.initial_states:
    ...         pass

