import stormpy
import stormpy.logic
from helpers.helper import get_example_path

from configurations import pars


@pars
class TestPLA:
    def test_pla(self):
        program = stormpy.parse_prism_program(get_example_path("pdtmc", "brp16_2.pm"))
        prop = "P<=0.84 [F s=5 ]"
        formulas = stormpy.parse_properties_for_prism_program(prop, program)
        model = stormpy.build_parametric_model(program, formulas)
        assert model.nr_states == 613
        assert model.nr_transitions == 803
        assert model.model_type == stormpy.ModelType.DTMC
        assert model.has_parameters
        env = stormpy.Environment()
        checker = stormpy.pars.create_region_checker(env, model, formulas[0].raw_formula)
        parameters = model.collect_probability_parameters()
        assert len(parameters) == 2
        region = stormpy.pars.ParameterRegion("0.7<=pL<=0.9,0.75<=pK<=0.95", parameters)
        result = checker.check_region(env, region)
        assert result == stormpy.pars.RegionResult.ALLSAT
        region = stormpy.pars.ParameterRegion("0.4<=pL<=0.65,0.75<=pK<=0.95", parameters)
        result = checker.check_region(env, region, stormpy.pars.RegionResultHypothesis.UNKNOWN,
                                      stormpy.pars.RegionResult.UNKNOWN, True)
        assert result == stormpy.pars.RegionResult.EXISTSBOTH
        region = stormpy.pars.ParameterRegion("0.1<=pL<=0.73,0.2<=pK<=0.715", parameters)
        result = checker.check_region(env, region)
        assert result == stormpy.pars.RegionResult.ALLVIOLATED
