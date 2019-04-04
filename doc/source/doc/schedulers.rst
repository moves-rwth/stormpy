***********************
Working with Schedulers
***********************

In non-deterministic models the notion of a scheduler (or policy) is important.
The scheduler determines which action to take at each state.

For a given reachability property, Storm can return the scheduler realizing the resulting probability.

Examining Schedulers
====================

.. seealso:: `01-schedulers.py <https://github.com/moves-rwth/stormpy/blob/master/examples/schedulers/01-schedulers.py>`_

As in :doc:`../getting_started`, we import some required modules and build a model from the example files::

    >>> import stormpy
    >>> import stormpy.core
    >>> import stormpy.examples
    >>> import stormpy.examples.files

    >>> path = stormpy.examples.files.prism_mdp_coin_2_2
    >>> formula_str = "Pmin=? [F \"finished\" & \"all_coins_equal_1\"]"
    >>> program = stormpy.parse_prism_program(path)
    >>> formulas = stormpy.parse_properties_for_prism_program(formula_str, program)
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

