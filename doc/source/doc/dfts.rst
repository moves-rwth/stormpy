*******************
Dynamic Fault Trees
*******************


Building DFTs
=============
.. seealso:: `01-dfts.py <https://github.com/moves-rwth/stormpy/blob/master/examples/dfts/01-dfts.py>`_

Dynamic fault trees can be loaded from either the Galileo format or from a custom JSON form.
A file containing the DFT in the Galileo format can be loaded via ``load_dft_galileo_file(path)``.
The custom JSON format can be loaded from a file via ``load_dft_json_file(path)`` or directly from a string via ``load_dft_json_string(path)``.
We start by loading a simple DFT containing an AND gate from JSON::

    >>> import stormpy
    >>> import stormpy.dft
    >>> import stormpy.examples
    >>> import stormpy.examples.files
    >>> path_json = stormpy.examples.files.dft_json_and
    >>> dft_small = stormpy.dft.load_dft_json_file(path_json)
    >>> print(dft_small)
    Top level index: 2, Nr BEs2

Next we load a more complex DFT from the Galileo format::

    >>> path_galileo = stormpy.examples.files.dft_galileo_hecs
    >>> dft = stormpy.dft.load_dft_galileo_file(path_galileo)

After loading the DFT, we can display some common statistics about the model::

    >>> print("DFT with {} elements.".format(dft.nr_elements()))
    DFT with 23 elements.
    >>> print("DFT has {} BEs and {} dynamic elements.".format(dft.nr_be(), dft.nr_dynamic()))
    DFT has 13 BEs and 2 dynamic elements.


Analyzing DFTs
==============
.. seealso:: `01-dfts.py <https://github.com/moves-rwth/stormpy/blob/master/examples/dfts/01-dfts.py>`_

The next step is to analyze the DFT via ``analyze_dft(dft, formula)``.
Here we can use all standard properties as described in :ref:`getting-started-building-properties`.
In our example we compute the `Mean-time-to-failure (MTTF)`::

    >>> formula_str = "T=? [ F \"failed\" ]"
    >>> formulas = stormpy.parse_properties(formula_str)
    >>> results = stormpy.dft.analyze_dft(dft, [formulas[0].raw_formula])
    >>> result = results[0]
    >>> print("MTTF: {:.2f}".format(result))
    MTTF: 363.89

