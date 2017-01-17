****************************
Getting Started
****************************


A Quick Tour through Stormpy
================================

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
	import stormpy.examples.files

With this, we can now import the path of our prism file::

	path = stormpy.examples.files.knuth_yao
	prism_program = stormpy.parse_prism_program(path)
	
The `prism_program` can be translated into Markov chains::

    model = stormpy.build_model(program_program)
    print("Number of states: {}".format(model.nr_states))


Building properties
--------------------------
Storm takes properties in the prism-property format. To express that one is interested in the reachability of any state where the prism program variable s is 7 and d is 2, one would formulate::

	P=? [F s=7 & d=2]

Stormpy can be used to parse this. As the variables in the property refer to a program, the program has to be passed as an additional parameter::

	formula_str = "P=? [F s=7 & d=2]"
	properties = parse_properties_for_prism_program(formula_str, prism_program)  

Notice that properties is now a list of properties containing a single element. 







Checking properties
---------------------------

