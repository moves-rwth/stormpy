import stormpy
import stormpy.core

import stormpy.examples
import stormpy.examples.files

path = stormpy.examples.files.prism_dtmc_die
prism_program = stormpy.parse_prism_program(path)

model = stormpy.build_model(prism_program)
print("Number of states: {}".format(model.nr_states))
print("Number of transitions: {}".format(model.nr_transitions))
print("Labels: {}".format(model.labels))