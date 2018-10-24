***************
Building Models
***************

Background
=====================

Storm supports a wide range of formalisms. Stormpy can be used to build models from some of these formalisms.
Moreover, during construction, various options can be set. This document yields information about the most important options.


Building different formalisms
===============================

We use some standard examples::

    >>> import stormpy.examples
    >>> import stormpy.examples.files

Storm supports the explicit DRN format.
From this, models can be built directly::

    >>> path = stormpy.examples.files.drn_ctmc_dft
    >>> model = stormpy.build_model_from_drn(path)
    >>> print(model.model_type)
    ModelType.CTMC

And the same for parametric models::

    >>> path = stormpy.examples.files.drn_pdtmc_die
    >>> model = stormpy.build_parametric_model_from_drn(path)
    >>> print(model.model_type)
    ModelType.DTMC

Another option are JANI descriptions. These are another high-level description format.
Building models from JANI is done in two steps. First the Jani-description is parsed, and then the model is built from this description.

    >>> path = stormpy.examples.files.jani_dtmc_die
    >>> jani_program, properties = stormpy.parse_jani_model(path)
    >>> model = stormpy.build_model(jani_program)
    >>> print(model.model_type)
    ModelType.DTMC

Notice that parsing JANI files also returns properties. In JANI, properties can be embedded in the model file.
