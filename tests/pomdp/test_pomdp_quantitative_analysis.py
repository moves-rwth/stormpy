import stormpy
import stormpy.logic
import stormpy.pomdp
from helpers.helper import get_example_path

class TestPomdpQuantitative:
    def test_underapprox_mc_maze_pmax(self):
        program = stormpy.parse_prism_program(get_example_path("pomdp", "maze_2.prism"))
        formulas = stormpy.parse_properties_for_prism_program("Pmax=? [ !\"bad\" U \"goal\" ]", program)
        model = stormpy.build_model(program, formulas)
        model = stormpy.pomdp.make_canonic(model)
        options = stormpy.pomdp.BeliefExplorationModelCheckerOptionsDouble(False, True)
        options.use_state_elimination_cutoff = False
        options.size_threshold_init = 10
        options.use_clipping = False
        belmc = stormpy.pomdp.BeliefExplorationModelCheckerDouble(model, options)
        result = belmc.check(formulas[0].raw_formula, [])
        print(result.lower_bound)
        print(result.upper_bound)
        print(result.induced_mc_from_scheduler)
        print(result.cutoff_schedulers[0])

    def test_underapprox_mc_maze_rmin(self):
        program = stormpy.parse_prism_program(get_example_path("pomdp", "maze_2.prism"))
        formulas = stormpy.parse_properties_for_prism_program("Rmin=? [ F \"goal\" ]", program)
        model = stormpy.build_model(program, formulas)
        model = stormpy.pomdp.make_canonic(model)
        options = stormpy.pomdp.BeliefExplorationModelCheckerOptionsDouble(False, True)
        options.use_state_elimination_cutoff = False
        options.size_threshold_init = 10
        options.use_clipping = False
        belmc = stormpy.pomdp.BeliefExplorationModelCheckerDouble(model, options)
        result = belmc.check(formulas[0].raw_formula, [])
        print(result.lower_bound)
        print(result.upper_bound)
        print(result.induced_mc_from_scheduler)
        print(result.cutoff_schedulers[0])

    def test_underapprox_mc_cmaze_rmin(self):
        program = stormpy.parse_prism_program(get_example_path("pomdp", "maze-concise.prism"))
        formulas = stormpy.parse_properties_for_prism_program("R{\"steps\"}min=? [F ((x = 2) & (y = 0))]", program)
        model = stormpy.build_model(program, formulas)
        model = stormpy.pomdp.make_canonic(model)
        options = stormpy.pomdp.BeliefExplorationModelCheckerOptionsDouble(False, True)
        options.use_state_elimination_cutoff = False
        options.size_threshold_init = 10
        options.use_clipping = False
        belmc = stormpy.pomdp.BeliefExplorationModelCheckerDouble(model, options)
        result = belmc.check(formulas[0].raw_formula, [])
        print(result.lower_bound)
        print(result.upper_bound)
        print(result.induced_mc_from_scheduler)
        print(result.cutoff_schedulers[0])