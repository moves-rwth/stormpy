import stormpy
import stormpy.logic
import stormpy.pars
from helpers.helper import get_example_path


class TestModelInstantiator:
    def test_instantiate_dtmc(self):
        program = stormpy.parse_prism_program(get_example_path("pdtmc", "brp16_2.pm"))
        formulas = stormpy.parse_properties_for_prism_program("P=? [ F s=5 ]", program)
        model = stormpy.build_parametric_model(program, formulas)
        parameters = model.collect_probability_parameters()
        instantiator = stormpy.pars.PDtmcInstantiator(model)

        point = {p: stormpy.RationalRF("0.4") for p in parameters}
        instantiated_model = instantiator.instantiate(point)
        assert instantiated_model.nr_states == model.nr_states
        assert not instantiated_model.has_parameters
        assert "0.4" in str(instantiated_model.transition_matrix[1])

        point = {p: stormpy.RationalRF("0.5") for p in parameters}
        instantiated_model2 = instantiator.instantiate(point)
        assert "0.5" in str(instantiated_model2.transition_matrix[1])
