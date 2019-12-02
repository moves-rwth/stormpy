import stormpy
import stormpy.core

import stormpy.examples
import stormpy.examples.files


def example_analysis_02():
    path = stormpy.examples.files.prism_dtmc_die
    prism_program = stormpy.parse_prism_program(path)

    formula_str = "P=? [F s=7 & d=2]"
    properties = stormpy.parse_properties(formula_str, prism_program)
    model = stormpy.build_symbolic_model(prism_program, properties)
    result = stormpy.model_checking(model, properties[0])
    filter = stormpy.create_filter_initial_states_symbolic(model)
    result.filter(filter)
    assert result.min == result.max
    print(result.min)

if __name__ == '__main__':
    example_analysis_02()