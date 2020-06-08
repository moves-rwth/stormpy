Changelog
=============

Version 1.6.x
-------------

### Version 1.6.0 (2020/06)
Requires storm version >= 1.6.0 and pycarl version >= 2.0.4

- Adaptions to changes in Storm: most notably state valuations
- Support for GSPNs: parsing, exploring, building
- Support for matrix building
- Extended expression operators with {conjunction, disjunction}
- Added information collector to extract information from jani models
- Bindings for elimination of chains of non-Markovian states


Version 1.5.x
-------------

### Version 1.5.1
Skipped for compatibility with Storm.

### Version 1.5.0
Skipped for compatibility with Storm.


Version 1.4.x
-------------

### Version 1.4.1 (2019/12)
Requires storm version >= 1.4.1 and pycarl version >= 2.0.4
- Adaptions to changes in Storm
- Extended simple accessors such as `parse_properties`, `model_checking`, to handle variety of inputs.
- Added and extended environments
- Changed constructor of `ParameterRegion` to take a valuation instead of string.
  Use `ParameterRegion.create_from_string()` to create a region from string.
- Added InstantiationModelChecker for pMDPs and allow instations with rational numbers
- Added transformation of CTMCs to DTMCs
- Further bindings for Prism Programs and their preprocessing
- SettingsManager updated
- Allow building models annotated with valuations
- A custom state exploration that allows to explore successor states of a prism program
- Extended documentation, in particular examples.
- Fix for compilation with Xcode 11


### Version 1.4.0
Skipped for compatibility with Storm.


Version 1.3.x
-------------

### Version 1.3.0 (2019/01)
Requires storm version >= 1.3.0 and pycarl version >= 2.0.3
- Adaptions to changes in Storm
- Bindings for symbolic models:
    * building symbolic models
    * bisimulation
    * transforming symbolic to sparse models
- Extraction of schedulers and queries on schedulers
- High-level counterexamples connected
- Drastically extended JANI bindings
- Extended bindings for expressions
- Extended PLA bindings
- Extended DFT bindings
- Extended documentation
- Improved and extended setup


Version 1.2.x
-------------

### Version 1.2.0
Requires storm version >= 1.2.0 and pycarl version >= 2.0.2
- Adaptions to changes in Storm
- Bindings for simplification of parametric models
- Handling of rewards
- Building of model with all labels if no formula is given
- Support for `submatrix()`
- Extended documentation
- Extended build script


Version 1.1.x
-------------

### Version 1.1.0 (2017/08)
Requires storm version >= 1.1.0
- Bindings for `storm-pars`
- Bindings for graph constraints
- Bindings for handling JANI files
- Moved expressions from own module into `storage` module
- Travis support for build process
- Tests for documentation
- Updated pybind version


Version 1.0.x
-------------
Skipped, to keep on par with storm


Version 0.9.x
-------------

### Version 0.9.1 (2017/04)
- Bindings for DFTs
- Bindings for PLA
- Updated to pycarl version 2.0.0 (support for both CLN and GMP)
- Improved building system, read flags from storm build system

### Version 0.9 (2017/03)
- Start of this changelog
