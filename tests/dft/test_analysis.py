import stormpy
import stormpy.logic
from helpers.helper import get_example_path

import math
from configurations import dft


@dft
class TestAnalysis:

    def test_analyze_mttf(self):
        dft = stormpy.dft.load_dft_json_file(get_example_path("dft", "and.json"))
        formulas = stormpy.parse_properties("T=? [ F \"failed\" ]")
        assert dft.nr_elements() == 3
        results = stormpy.dft.analyze_dft(dft, [formulas[0].raw_formula])
        assert math.isclose(results[0], 3)
