****************************
Getting Started
****************************

Before starting with this guide, one should follow the instructions for :doc:`installation`.

A Quick Tour through Stormpy
================================

This guide is intended for people which have a basic understanding of probabilistic models and their verification. More details and further pointers to literature can be found on the
`storm website <https://moves-rwth.github.io/storm>`_. 
While we assume some very basic programming concepts, we refrain from using more advanced concepts of python throughout the guide.

We start with a selection of high-level constructs in stormpy, and go into more details afterwards. 

.. seealso:: The code examples are also given in the examples/ folder. These boxes throughout the text will tell you which example contains the code discussed.

In order to do this, we import stormpy::

	import stormpy
	import stormpy.core
	
	
Building models 
------------------------------------------------
.. seealso:: ``01-getting-started.py``	

There are several ways to create a Markov chain. 
One of the easiest is to parse a description of such a Markov chain and to let storm build the chain. 

Here, we build a Markov chain from a prism program.
Stormpy comes with a small set of examples, which we use here::

	import stormpy.examples
	import stormpy.examples.files

With this, we can now import the path of our prism file::

	path = stormpy.examples.files.prism_dtmc_die
	prism_program = stormpy.parse_prism_program(path)
	
The `prism_program` can be translated into Markov chains::

    model = stormpy.build_model(prism_program)
    print("Number of states: {}".format(model.nr_states)) # out: 13
    print("Number of states: {}".format(model.nr_states)) # out: 20
    
This tells us that the model has 13 states and 20 transitions.

Moreover, initial states and deadlocks are indicated with a labelling function. We can see the labels present in the model by::

	print("Labels: {}".format(model.labels)  # out: {"init", "deadlock"}
	
We will investigate ways to examine the model in more detail in :ref:`getting-started-investigating-the-model`


Building properties
--------------------------
.. seealso:: ``02-getting-started.py``

Storm takes properties in the prism-property format. 
To express that one is interested in the reachability of any state where the prism program variable s is 2, one would formulate::

	P=? [F s=2]

Stormpy can be used to parse this. As the variables in the property refer to a program, the program has to be passed as an additional parameter::

	formula_str = "P=? [F s=2]"
	properties = parse_properties_for_prism_program(formula_str, prism_program)  

Notice that properties is now a list of properties containing a single element. 

However, if we build the model as before, then the appropriate information that the variable s=2 in some states is not present.
In order to label the states accordingly, we should notify storm upon building the model that we would like to preserve given properties. 
Storm will then add the labels accordingly::

	model = stormpy.build_model(prism_program, properties)
	print("Labels: {}".format(model.labels)  # {"init", "deadlock", "s=2"})

Model building however now behaves slightly different: Only the properties passed are preserved, which means that model building might skip parts of the model.
In particular, to check the probability of eventually reaching a state x where s=2, successor states of x are not relevant::

    print("Number of states: {}".format(model.nr_states))  # out: 8
	
If we consider another property, however, such as::

	P=? [F s=7 & d=2]

then storm is only skipping exploration of successors of the particular state y where s=7 and d=2. In this model, state y has a self-loop, so effectively, the whole model is explored.


Checking properties
------------------------------------
.. seealso:: ``03-getting-started.py``

The last lesson taught us to construct properties and models with matching state labels. 
Now default checking routines are just a simple command away::

	properties = parse_properties_for_prism_program(formula_str, prism_program) 
	model = stormpy.build_model(prism_program, properties)
    result = stormpy.model_checking(model, properties[0])
    
The result may contain information about all states. Merely printing does not give all information in there::

	print(result) # out: [0,1] range
	
Instead, we can iterate over the results:

	




Instantiating parametric models
------------------------------------
.. seealso:: ``04-getting-started.py``




Checking parametric models
------------------------------------
.. seealso:: ``05-getting-started.py``




.. _getting-started-investigating-the-model:

Investigating the model
-------------------------------------




