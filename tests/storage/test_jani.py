import stormpy
import stormpy.logic
from helpers.helper import get_example_path
import pytest


class TestJani:

    def test_information_collection(self):
        model, properties = stormpy.parse_jani_model(get_example_path("dtmc", "brp.jani"))
        information = stormpy.collect_information(model)
        assert information.nr_automata == 5
        assert information.nr_edges == 31
        assert information.nr_variables == 18