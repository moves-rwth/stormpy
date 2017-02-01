import stormpy
import stormpy.core

import stormpy.examples
import stormpy.examples.files

path = stormpy.examples.files.prism_dtmc_die
prism_program = stormpy.parse_prism_program(path)


model = stormpy.build_model(prism_program)
print("Number of states: {}".format(model.nr_states))
print("Number of transitions: {}".format(model.nr_transitions))
print("Labels in the model: {}".format(model.labels))

formula_str = "P=? [F s=2]"
properties = stormpy.parse_properties_for_prism_program(formula_str, prism_program)
model_for_formula = stormpy.build_model(prism_program, properties)
print("Number of states: {}".format(model_for_formula.nr_states))
print("Number of transitions: {}".format(model_for_formula.nr_transitions))
print("Labels in the model: {}".format(model_for_formula.labels))


formula_str_2 = "P=? [F s=7 & d=2]"
properties_2 = stormpy.parse_properties_for_prism_program(formula_str_2, prism_program)
model_for_formula_2 = stormpy.build_model(prism_program, properties_2)
print("Number of states: {}".format(model_for_formula_2.nr_states))
print("Number of transitions: {}".format(model_for_formula_2.nr_transitions))
print("Labels in the model: {}".format(model_for_formula_2.labels))
