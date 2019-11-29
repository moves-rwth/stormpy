***********************
Working with Schedulers
***********************

In non-deterministic models the notion of a scheduler (or policy) is important.
The scheduler determines which action to take at each state.

For a given reachability property, Storm can return the scheduler realizing the resulting probability.

Examining Schedulers for MDPs
=============================

.. seealso:: `01-schedulers.py <https://github.com/moves-rwth/stormpy/blob/master/examples/schedulers/01-schedulers.py>`_

As in :doc:`../getting_started`, we import some required modules and build a model from the example files::

    >>> import stormpy
    >>> import stormpy.core
    >>> import stormpy.examples
    >>> import stormpy.examples.files

    >>> path = stormpy.examples.files.prism_mdp_coin_2_2
    >>> formula_str = "Pmin=? [F \"finished\" & \"all_coins_equal_1\"]"
    >>> program = stormpy.parse_prism_program(path)
    >>> formulas = stormpy.parse_properties(formula_str, program)
    >>> model = stormpy.build_model(program, formulas)

Next we check the model and make sure to extract the scheduler:

    >>> result = stormpy.model_checking(model, formulas[0], extract_scheduler=True)

The result then contains the scheduler we want:

    >>> assert result.has_scheduler
    >>> scheduler = result.scheduler
    >>> assert scheduler.memoryless
    >>> assert scheduler.deterministic
    >>> print(scheduler)
    ___________________________________________________________________
    Fully defined memoryless deterministic scheduler:
    model state:    choice(s)
               0    0
               1    0
               2    1
               3    0
    -etc-

To get the information which action the scheduler chooses in which state, we can simply iterate over the states:

    >>> for state in model.states:
    ...     choice = scheduler.get_choice(state)
    ...     action = choice.get_deterministic_choice()
    ...     print("In state {} choose action {}".format(state, action))
    In state 0 choose action 0
    In state 1 choose action 0
    In state 2 choose action 1
    In state 3 choose action 0
    In state 4 choose action 0
    In state 5 choose action 0
    -etc-


Examining Schedulers for Markov automata
========================================

.. seealso:: `02-schedulers.py <https://github.com/moves-rwth/stormpy/blob/master/examples/schedulers/02-schedulers.py>`_

Currently there is no support yet for scheduler extraction on MAs.
However, if the timing information is not relevant for the property, we can circumvent this lack by first transforming the MA to an MDP.

We build the model as before:

    >>> path = stormpy.examples.files.prism_ma_simple
    >>> formula_str = "Tmin=? [ F s=4 ]"

    >>> program = stormpy.parse_prism_program(path, False, True)
    >>> formulas = stormpy.parse_properties_for_prism_program(formula_str, program)
    >>> ma = stormpy.build_model(program, formulas)

Next we transform the continuous-time model into a discrete-time model.
Note that all timing information is lost at this point.

    >>> mdp, mdp_formulas = stormpy.transform_to_discrete_time_model(ma, formulas)
    >>> assert mdp.model_type == stormpy.ModelType.MDP

After the transformation we have obtained an MDP where we can extract the scheduler as shown before:

    >>> result = stormpy.model_checking(mdp, mdp_formulas[0], extract_scheduler=True)
    >>> scheduler = result.scheduler
    >>> print(scheduler)
    ___________________________________________________________________
    Fully defined memoryless deterministic scheduler:
    model state:    choice(s)
               0    1
               1    0
               2    0
               3    0
               4    0
    -etc-
