***********************
Installation
***********************

Requirements
==================

Before installing stormpy, make sure

- Python 3 is available on your system. Stormpy does not work with python 2.
- `pycarl <https://moves-rwth.github.io/pycarl>`_ is available.
- `Storm <http://www.stormchecker.org/>`_ is available on your system.

To avoid issues, we suggest that both pycarl and Storm use the same version of `carl <https://smtrat.github.io/carl>`_.
The simplest way of ensuring this is to first install carl as explained in the `Storm installation guide <http://www.stormchecker.org/documentation/installation/manual-configuration.html#carl>`_.
You can then install Storm and pycarl independently.


Installation Steps
====================

Virtual Environments
--------------------

Virtual environments create isolated environments for your projects.
This helps to keep your system clean, work with different versions of packages and different version of python.
While it is not required, we recommend the use of such virtual environments. To get you started, we recommend
`this guide <http://docs.python-guide.org/en/latest/dev/virtualenvs/>`_ or
`this primer <https://realpython.com/blog/python/python-virtual-environments-a-primer>`_.

In short you can create a virtual environment ``env`` with::

	$ pip install virtualenv
	$ virtualenv -p python3 env
	$ source env/bin/activate

The last step activates the virtual environment.
Whenever using the environment the console prompt is prefixed with ``(env)``.


Building stormpy
----------------

Clone stormpy into any suitable location::

	$ git clone https://github.com/moves-rwth/stormpy.git
	$ cd stormpy
	
Build stormpy in develop mode using your favourite python distribution way of installing: e.g.::

	$ python3 setup.py develop
	
or::

	$ pip install -ve .
	

Optional build arguments
^^^^^^^^^^^^^^^^^^^^^^^^

The build step ``build_ext`` also takes optional arguments for a more advanced configuration of stormpy.

*	*Specifying which Storm library to use*

	If you have multiple versions of Storm or cmake is not able to find your Storm version,
	you can specify the ``--storm-dir YOUR-PATH-TO-STORM`` flag::

	$ python3 setup.py build_ext --storm-dir YOUR-PATH-TO-STORM develop

*	*Disabling functionality*

	If you want to disable certain functionality in stormpy from being built you can use the following flags:

	* ``--disable-dft`` to disable support for dynamic fault trees (DFTs)
	* ``--disable-pars`` to disable support for parametric models

*	*Building stormpy in debug mode*

	If you want to build stormpy in debug mode you can add the ``--debug`` flag::

	$ python3 setup.py build_ext --debug develop

*	*Setting number of build threads*

	The build of stormpy uses all available cores per default.
	If you want to configure the number of threads manually you can specify the ``--jobs`` (or ``-j``) flag::

	$ python3 setup.py build_ext --jobs 2 develop


Testing stormpy installation
----------------------------

After building, you can run the test files by either::

	$ python setup.py test

or by invoking pytest directly with::

	$ pip install pytest
	$ py.test tests/

If the tests pass, you can now use stormpy.
To get started, continue with our :doc:`getting_started`, consult the test files in ``tests/`` or the :doc:`api` (work in progress).
