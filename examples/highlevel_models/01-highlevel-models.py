import stormpy
import stormpy.core

import stormpy.examples
import stormpy.examples.files


def example_highlevel_models():
    path = stormpy.examples.files.prism_pdtmc_die
    prism_program = stormpy.parse_prism_program(path)
    for c in prism_program.constants:
        print("constant {} with type {} is {} defined".format(c.name, c.type, "" if c.defined else "not"))



if __name__ == '__main__':
    example_highlevel_models()