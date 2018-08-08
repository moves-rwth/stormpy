import stormpy
import stormpy.logic
from helpers.helper import get_example_path

import math
from configurations import dft


@dft
class TestBuild:
    def test_load_dft_json(self):
        dft = stormpy.dft.load_dft_json_file(get_example_path("dft", "and.json"))
        assert dft.nr_elements() == 3
        assert dft.nr_be() == 2
        assert dft.nr_dynamic() == 0
        assert not dft.can_have_nondeterminism()

    def test_load_dft_galileo(self):
        dft = stormpy.dft.load_dft_galileo_file(get_example_path("dft", "hecs.dft"))
        assert dft.nr_elements() == 23
        assert dft.nr_be() == 13
        assert dft.nr_dynamic() == 2
        assert not dft.can_have_nondeterminism()

    def test_analyze_mttf(self):
        dft = stormpy.dft.load_dft_json_file(get_example_path("dft", "and.json"))
        formulas = stormpy.parse_properties("T=? [ F \"failed\" ]")
        assert dft.nr_elements() == 3
        results = stormpy.dft.analyze_dft(dft, [formulas[0].raw_formula])
        assert math.isclose(results[0], 3)
