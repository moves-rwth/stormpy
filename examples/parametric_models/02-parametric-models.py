import stormpy
import stormpy.info

import pycarl

import stormpy.examples
import stormpy.examples.files

import stormpy._config as config


def example_parametric_models_02():
    # Check support for parameters
    if not config.storm_with_pars:
        print("Support parameters is missing. Try building storm-pars.")
        return

    import stormpy.pars
    from pycarl.formula import FormulaType, Relation

    if stormpy.info.storm_ratfunc_use_cln():
        import pycarl.cln.formula
    else:
        import pycarl.gmp.formula

    path = stormpy.examples.files.prism_pdtmc_die
    prism_program = stormpy.parse_prism_program(path)

    formula_str = "P=? [F s=7 & d=2]"
    properties = stormpy.parse_properties_for_prism_program(formula_str, prism_program)
    model = stormpy.build_parametric_model(prism_program, properties)

    initial_state = model.initial_states[0]
    result = stormpy.model_checking(model, properties[0])
    print("Result: {}".format(result.at(initial_state)))

    collector = stormpy.ConstraintCollector(model)
    print("Well formed constraints:")
    for formula in collector.wellformed_constraints:
        print(formula.get_constraint())
    print("Graph preserving constraints:")
    for formula in collector.graph_preserving_constraints:
        print(formula.get_constraint())


if __name__ == "__main__":
    example_parametric_models_02()
