import stormpy
import stormpy.logic
from helpers.helper import get_example_path
import pytest

class TestPrism:
    def test_prism_to_jani(self):
        program = stormpy.parse_prism_program(get_example_path("dtmc", "die.pm"))
        orig_properties = stormpy.parse_properties_for_prism_program("P=? [F \"two\"]", program)
        assert len(orig_properties) == 1
        jani_model, new_properties = program.to_jani(orig_properties)
        assert len(new_properties) == len(orig_properties)
