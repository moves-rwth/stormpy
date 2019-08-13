Changelog
=============

Version 1.3.x
-------------

### Version 1.3.1 (under development)
- Changed constructor of `ParameterRegion` to take a valuation instead of string.
  Use `ParameterRegion.create_from_string()` to create a region from string.


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
