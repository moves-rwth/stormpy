***********************
Installation
***********************

Requirements
==================

Before installing pycarl, make sure

- `carl <https://smtrat.github.io/carl>`_

is available on your system.

.. topic:: Virtual Environments

	Virtual environments create isolated environments for your projects. This helps to keep your system clean, work with different versions of packages and different version of python. While it is not required, we recommend the use of
	such virtual environments. To get you started, we recommend `this guide <http://docs.python-guide.org/en/latest/dev/virtualenvs/>`_ or `this primer <https://realpython.com/blog/python/python-virtual-environments-a-primer>`_.

In order to have full parsing capabilities, you additionally need `carl-parser <https://github.com/smtrat/carl-parser>`_


Installation Steps
====================

Clone pycarl into any suitable location::

	$ git clone https://github.com/moves-rwth/pycarl.git
	$ cd pycarl

Here, build pycarl in develop mode using your favourite python distribution way of installing: e.g.::

	$ python3 setup.py develop

or::

	$ pip install -ve .


.. topic:: Specifying which carl library to use

	If you have multiple versions of storm or cmake is not able to find your storm version,
	you can specify the `--carl-DIR YOUR-PATH-TO-CARL` flag in the build_ext step::

		$ python3 setup.py build_ext --carl-DIR YOUR-PATH-TO-CARL develop


After building, you can run the test files by::

	$ py.test tests/

If you do not experience any issues, you can now use pycarl. To get started, consult the test files in `tests/` or the :doc:`api` (work in progress)
