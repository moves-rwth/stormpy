***************
Engines
***************

Background
=====================

Storm supports different engines for building and checking a model. A detailed comparison of the different engines provided in Storm can be found on the `Storm website <http://www.stormchecker.org/documentation/usage/engines.html>`_.


Sparse engine
===============================

In all of the examples so far we used the default sparse engine:

    >>> import stormpy.examples
    >>> import stormpy.examples.files
	>>> prism_program = stormpy.parse_prism_program(stormpy.examples.files.prism_dtmc_die)
    >>> properties = stormpy.parse_properties('P=? [F "one"]', prism_program)
    >>> sparse_model = stormpy.build_sparse_model(prism_program, properties)
    >>> print(type(sparse_model))
    <class 'stormpy.storage.storage.SparseDtmc'>
    >>> print("Number of states: {}".format(sparse_model.nr_states))
    Number of states: 13
    >>> print("Number of transitions: {}".format(sparse_model.nr_transitions))
    Number of transitions: 20

The model checking was also done in the sparse engine:

    >>> sparse_result = stormpy.check_model_sparse(sparse_model, properties[0])
    >>> initial_state = sparse_model.initial_states[0]
    >>> print(sparse_result.at(initial_state))
    0.16666666666666666


Symbolic engine
===============================

Instead of using the sparse engine, one can also use a symbolic representation in terms of `binary decision diagrams (BDDs)`.
To use the symbolic (dd) engine, we use the symbolic versions for the building and model checking:

    >>> symbolic_model = stormpy.build_symbolic_model(prism_program, properties)
    >>> print(type(symbolic_model))
    <class 'stormpy.storage.storage.SymbolicSylvanDtmc'>
    >>> print("Number of states: {}".format(symbolic_model.nr_states))
    Number of states: 13
    >>> print("Number of transitions: {}".format(symbolic_model.nr_transitions))
    Number of transitions: 20
    >>> symbolic_result = stormpy.check_model_dd(symbolic_model, properties[0])
    >>> print(symbolic_result)
    [0, 1] (range)

We can also filter the computed results and only consider the initial states:

    >>> filter = stormpy.create_filter_initial_states_symbolic(symbolic_model)
    >>> symbolic_result.filter(filter)
    >>> print(symbolic_result.min)
    0.16666650772094727

It is also possible to first build the model symbolically and then transform it into a sparse model:

    >>> print(type(symbolic_model))
    <class 'stormpy.storage.storage.SymbolicSylvanDtmc'>
    >>> transformed_model = stormpy.transform_to_sparse_model(symbolic_model)
    >>> print(type(transformed_model))
    <class 'stormpy.storage.storage.SparseDtmc'>


Hybrid engine
===============================

A third possibility is to use the hybrid engine, a combination of sparse and dd engines.
It first builds the model symbolically.
The actual model checking is then performed with the engine which is deemed most suitable for the given task.

    >>> print(type(symbolic_model))
    <class 'stormpy.storage.storage.SymbolicSylvanDtmc'>
    >>> hybrid_result = stormpy.check_model_hybrid(symbolic_model, properties[0])
    >>> filter = stormpy.create_filter_initial_states_symbolic(symbolic_model)
    >>> hybrid_result.filter(filter)
    >>> print(hybrid_result)
    0.166667
