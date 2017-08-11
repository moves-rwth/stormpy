***********************
Installation
***********************

Requirements
==================

Before installing stormpy, make sure

- `pycarl <https://moves-rwth.github.io/pycarl>`_
- `Storm <http://www.stormchecker.org/>`_

are both available on your system. To avoid issues, we suggest that both use the same version of `carl <https://smtrat.github.io/carl>`_.
The simplest way of ensuring this is to first install carl as explained in the `Storm installation guide <http://www.stormchecker.org/documentation/installation/manual-configuration.html#carl>`_.
You can then install Storm and pycarl independently.

.. topic:: Virtual Environments

	Virtual environments create isolated environments for your projects. This helps to keep your system clean, work with different versions of packages and different versions of python. While it is not required, we recommend the use of
	such virtual environments. To get you started, we recommend `this guide <http://docs.python-guide.org/en/latest/dev/virtualenvs/>`_ or `this primer <https://realpython.com/blog/python/python-virtual-environments-a-primer>`_.


Installation Steps
====================

Clone stormpy into any suitable location::

	$ git clone https://github.com/moves-rwth/stormpy.git
	$ cd stormpy
	
Here, build stormpy in develop mode using your favourite python distribution way of installing: e.g.::

	$ python3 setup.py develop
	
or::

	$ pip install -ve .
	

.. topic:: Specifying which Storm library to use

	If you have multiple versions of Storm or cmake is not able to find your Storm version, 
	you can specify the `--storm-dir YOUR-PATH-TO-STORM` flag in the build_ext step::
	
	$ python3 setup.py build_ext --storm-dir YOUR-PATH-TO-STORM develop
	
After building, you can run the test files by::

	py.test tests/
	
If tests pass, you can continue with our :doc:`getting_started`.


