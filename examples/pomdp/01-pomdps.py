import stormpy
import stormpy.core
import stormpy.info

import pycarl
import pycarl.core

import stormpy.examples
import stormpy.examples.files

import stormpy.pomdp

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

    path = stormpy.examples.files.prism_pomdp_maze
    prism_program = stormpy.parse_prism_program(path)

    formula_str = "P=? [\"goal\"]"
    properties = stormpy.parse_properties_for_prism_program(formula_str, prism_program)
    pomdp = stormpy.build_model(prism_program, properties)
    pomdp = stormpy.pomdp.make_canonic(pomdp)
    pomdp = stormpy.pomdp.make_simple(pomdp)
    memory_builder = stormpy.pomdp.PomdpMemoryBuilder()
    memory = memory_builder.build(stormpy.pomdp.PomdpMemoryPattern.selective_counter, 2)
    pomdp = stormpy.pomdp.unfold_memory(pomdp, memory)
    pmc = stormpy.pomdp.apply_unknown_fsc(pomdp, stormpy.pomdp.PomdpFscApplicationMode.simple_linear)

    stormpy.export_parametric_to_drn(pmc, "test.out")

if __name__ == '__main__':
    example_parametric_models_02()
