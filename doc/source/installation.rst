Installation
***********************

Before installing stormpy, make sure `storm <https://moves-rwth.github.io/storm/>`_ is installed.

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
	
