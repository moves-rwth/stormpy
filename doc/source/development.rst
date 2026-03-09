**************
Developer information
**************

The following contains some general guidelines for developers.


Structure
=========
- C++ bindings are defined in ``src``
- Python methods are defined in ``lib/stormpy``
- Tests are in ``tests``
- The Sphinx documentation is in ``doc/source``


Coding conventions
==================
Formatting
----------
- Code should be formatted according to the given rules set by black (for Python).
  Proper formatting can be ensured by executing ``black .``.
  The CI checks for proper formatting.


Dependencies
============

- The bindings are created with `pybind11 <https://pybind11.readthedocs.io>`_.
  The pybind11 version is set in ``pyproject.toml`` through ``requires``, and in ``CMakeLists.txt`` through ``find_package(pybind11 ...)``.
- The minimal Python version is set in ``pyproject.toml`` through ``requires-python``, and in ``CMakeLists.txt`` through ``find_package(Python ...)``.
- The `Storm <https://www.stormchecker.org>`_ version is set in ``pyproject.toml`` through ``STORM_GIT_TAG``, and in ``CMakeLists.txt`` through ``STORM_MIN_VERSION``.
