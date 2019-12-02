*****************
Parametric Models
*****************



Instantiating parametric models
===============================
.. seealso:: `01-parametric-models.py <https://github.com/moves-rwth/stormpy/blob/master/examples//parametric_models/01-parametric-models.py>`_

Input formats such as prism allow to specify programs with open constants. We refer to these open constants as parameters.
If the constants only influence the probabilities or rates, but not the topology of the underlying model, we can build these models as parametric models::

    >>> import stormpy
    >>> import stormpy.examples
    >>> import stormpy.examples.files
    >>> path = stormpy.examples.files.prism_dtmc_die
    >>> prism_program = stormpy.parse_prism_program(path)
    >>> formula_str = "P=? [F s=7 & d=2]"
    >>> properties = stormpy.parse_properties(formula_str, prism_program)
    >>> model = stormpy.build_parametric_model(prism_program, properties)
    >>> parameters = model.collect_probability_parameters()
    >>> for x in parameters:
    ...     print(x)

In order to obtain a standard DTMC, MDP or other Markov model, we need to instantiate these models by means of a model instantiator::

    >>> import stormpy.pars
    >>> instantiator = stormpy.pars.PDtmcInstantiator(model)

Before we obtain an instantiated model, we need to map parameters to values: We build such a dictionary as follows::

    >>> point = dict()
    >>> for x in parameters:
    ...    print(x.name)
    ...    point[x] = 0.4
    >>> instantiated_model = instantiator.instantiate(point)
    >>> result = stormpy.model_checking(instantiated_model, properties[0])

Initial states and labels are set as for the parameter-free case.


Checking parametric models
==========================
.. seealso:: `02-parametric-models.py <https://github.com/moves-rwth/stormpy/blob/master/examples//parametric_models/02-parametric-models.py>`_

It is also possible to check the parametric model directly, similar as before in :ref:`getting-started-checking-properties`::

    >>> result = stormpy.model_checking(model, properties[0])
    >>> initial_state = model.initial_states[0]
    >>> func = result.at(initial_state)

We collect the constraints ensuring that underlying model is well-formed and the graph structure does not change.

    >>> collector = stormpy.ConstraintCollector(model)
    >>> for formula in collector.wellformed_constraints:
    ...     print(formula)
    >>> for formula in collector.graph_preserving_constraints:
    ...     print(formula)

Collecting information about the parametric models
==================================================
.. seealso:: `03-parametric-models.py <https://github.com/moves-rwth/stormpy/blob/master/examples//parametric_models/03-parametric-models.py>`_

This example shows three implementations to obtain the number of transitions with probability one in a parametric model.


