import pycarl
import stormpy
import stormpy.logic
from helpers.helper import get_example_path


class TestModel:
    def test_instantiate_dtmc(self):
        program = stormpy.parse_prism_program(get_example_path("pdtmc", "brp16_2.pm"))
        formulas = stormpy.parse_properties_for_prism_program("P=? [ F s=5 ]", program)
        model = stormpy.build_parametric_model(program, formulas)
        parameters = model.collect_probability_parameters()
        instantiator = stormpy.ModelInstantiator(model)

        point = {p: 0.4 for p in parameters}
        instantiated_model = instantiator.instantiate(point)
        assert instantiated_model.nr_states == model.nr_states
        assert not instantiated_model.has_parameters

        point = {p: 0.5 for p in parameters}
        instantiated_model2 = instantiator.instantiate(point)
