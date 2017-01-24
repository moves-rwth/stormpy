***********************
Installation
***********************

Requirements
==================

Before installing stormpy, make sure 

- `pycarl <https://moves-rwth.github.io/pycarl>`_ 
- `storm <https://moves-rwth.github.io/storm/>`_ 

are both available on your system. To avoid issues, we suggest that both use the same version of `carl <https://smtrat.github.io/carl>`_. 
The simplest way of ensuring this is to first install carl as explained in the `storm installation guide <https://moves-rwth.github.io/storm/documentation/installation/configuration-guide.html#carl>`_.
You can then install storm and pycarl independently.

.. topic:: Virtual Environments

	Virtual environments create isolated environments for your projects. This helps to keep your system clean, work with different versions of packages and different version of python. While it is not required, we recommend the use of
	such virtual environments. To get you started, we recommend `this guide <http://docs.python-guide.org/en/latest/dev/virtualenvs/>`_ or `this primer <https://realpython.com/blog/python/python-virtual-environments-a-primer>`_.


Installation Steps
====================

Clone stormpy into any suitable location::

	$ git clone https://srv-i2.informatik.rwth-aachen.de/scm/git/stormpy.git
	$ cd stormpy
	
Here, build stormpy in develop mode using your favourite python distribution way of installing: e.g.::

	$ python3 setup.py develop
	
or::

	$ pip install -ve .
	

.. topic:: Specifying which storm library to use

	If you have multiple versions of storm or cmake is not able to find your storm version, 
	you can specify the `--storm-DIR YOUR-PATH-TO-STORM` flag in the build_ext step::
	
		$ python3 setup.py build_ext --storm-DIR YOUR-PATH-TO-STORM develop
		
		
After building, you can run the test files by::

	py.test tests/ 
	
If tests pass, you can continue with our :doc:`getting_started`.


