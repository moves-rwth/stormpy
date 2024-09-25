import stormpy

import stormpy.examples
import stormpy.examples.files


def example_analysis_05():
    path = stormpy.examples.files.prism_mdp_maze_multigoal
    prism_program = stormpy.parse_prism_program(path)

    formula_str = "multi(Pmax=?  [F \"goalalt1\"], Pmax=?  [F \"goalalt2\"])"
    properties = stormpy.parse_properties(formula_str, prism_program)

    options = stormpy.BuilderOptions([p.raw_formula for p in properties])
    options.set_build_state_valuations()
    model = stormpy.build_sparse_model_with_options(prism_program, options)
    env = stormpy.Environment()
    lower_bound, upper_bound = stormpy.compute_rel_reach_helper(env, model, properties[0].raw_formula)
    print(lower_bound)
    print(upper_bound)

if __name__ == '__main__':
    example_analysis_05()
