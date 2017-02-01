import stormpy
import stormpy.core

import stormpy.examples
import stormpy.examples.files

path = stormpy.examples.files.prism_dtmc_die
prism_program = stormpy.parse_prism_program(path)


formula_str = "P=? [F s=7 & d=2]"
properties = stormpy.parse_properties_for_prism_program(formula_str, prism_program)
model = stormpy.build_model(prism_program, properties)
result = stormpy.model_checking(model, properties[0])
print(result)
assert result.result_for_all_states
for x in result.get_values():
    print(x)
