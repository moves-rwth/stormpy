import stormpy
import stormpy.core

import stormpy.examples
import stormpy.examples.files


def example_building_models_01():
    path = stormpy.examples.files.drn_ctmc_dft
    model = stormpy.build_model_from_drn(path)
    print(model.model_type)
    print("Number of states: {}".format(model.nr_states))

    # And the parametric
    path = stormpy.examples.files.drn_pdtmc_die
    model = stormpy.build_parametric_model_from_drn(path)
    print(model.model_type)
    print("Number of states: {}".format(model.nr_states))

    path = stormpy.examples.files.jani_dtmc_die
    jani_program, properties = stormpy.parse_jani_model(path)
    model = stormpy.build_model(jani_program)
    print(model.model_type)
    print("Number of states: {}".format(model.nr_states))


if __name__ == '__main__':
    example_building_models_01()