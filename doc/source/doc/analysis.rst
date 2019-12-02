***************
Analysis
***************

Background
=====================

Storm supports various model checking approaches that we consider in this section on analysis.

As always::

    >>>	import stormpy
    >>> import stormpy.examples
	>>> import stormpy.examples.files


Qualitative Analysis
======================


More to come...

Model checking algorithms
=========================
.. seealso:: `02-analysis.py <https://github.com/moves-rwth/stormpy/blob/master/examples/analysis/02-analysis.py>`_

Reconsider the model checking example from the getting started guide::

	>>> path = stormpy.examples.files.prism_dtmc_die
	>>> prism_program = stormpy.parse_prism_program(path)
    >>> properties = stormpy.parse_properties(formula_str, prism_program)
    >>> model = stormpy.build_model(prism_program, properties)
    >>> result = stormpy.model_checking(model, properties[0])

We can also vary the model checking algorithm::

    >>> env = stormpy.Environment()
    >>> env.solver_environment.set_linear_equation_solver_type(stormpy.EquationSolverType.native)
    >>> env.solver_environment.native_solver_environment.method = stormpy.NativeLinearEquationSolverMethod.power_iteration
    >>> result = stormpy.model_checking(model, properties[0], environment=env)

Finally, we allow to change some parameters of the algorithms. E.g., in iterative approaches,
we allow to change the number of iterations::

    >>> env.solver_environment.maximum_iterations = 3
    >>> result = stormpy.model_checking(model, properties[0])

Notice that by setting such parameters, the result may be off from the actual model checking algorithm.

