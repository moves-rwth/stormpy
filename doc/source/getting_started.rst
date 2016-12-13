****************************
Getting Started
****************************


High-level API
======================

We start with a selection of high-level constructs in stormpy. 

In order to do this, we import stormpy::

	import stormpy
	import stormpy.core
	
	
Building models 
-----------------------
	
There are several ways to create a Markov chain. 
One of the easiest is to parse a description of such a Markov chain and to let storm build the chain. 

Here, we build a Markov chain from a prism program.
Stormpy comes with a small set of examples, which we use here::

	import stormpy.examples

With this, we can now import the path of our prism file::

	path = stormpy.examples.knuth_yao
	prism_program = stormpy.parse_prism_program(path)
	
The `prism_program` can be translated into 

	

	



