import stormpy
import stormpy.logic
from helpers.helper import get_example_path

import math

class TestBuild:

    def test_import(self):
        import stormpy.dft

    def test_build_dft(self):
        try:
            import stormpy.dft
        except ImportError:
            # No support for DFTs
            return

        model = stormpy.dft.build_sparse_model_from_json_dft(get_example_path("dft", "and.json"))
        formulas = stormpy.parse_properties("T=? [ F \"failed\" ]")
        assert model.nr_states == 4
        assert model.nr_transitions == 5
        assert len(model.initial_states) == 1
        initial_state = model.initial_states[0]
        assert initial_state == 1
        result = stormpy.model_checking(model, formulas[0])
        assert math.isclose(result.at(initial_state), 3)
