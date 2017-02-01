import stormpy
import stormpy.core

import pycarl
import pycarl.core

import stormpy.examples
import stormpy.examples.files

path = stormpy.examples.files.prism_pdtmc_die
prism_program = stormpy.parse_prism_program(path)


formula_str = "P=? [F s=7 & d=2]"
properties = stormpy.parse_properties_for_prism_program(formula_str, prism_program)
model = stormpy.build_parametric_model(prism_program, properties)
print("Model supports parameters: {}".format(model.supports_parameters))

instantiator = stormpy.PdtmcInstantiator(model)
point = dict()
parameters = model.collect_probability_parameters()
for x in parameters:
    print(x.name)
    point[x] = 0.4
instantiated_model = instantiator.instantiate(point)
result = stormpy.model_checking(instantiated_model, properties[0])

