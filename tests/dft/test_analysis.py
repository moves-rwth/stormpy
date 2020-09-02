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

    def test_relevant_events_property(self):
        dft = stormpy.dft.load_dft_json_file(get_example_path("dft", "and.json"))
        properties = stormpy.parse_properties("P=? [ F<=1 \"A_failed\" ]")
        formulas = [p.raw_formula for p in properties]
        relevant_events = stormpy.dft.compute_relevant_events(dft, formulas)
        assert relevant_events.is_relevant("A")
        assert not relevant_events.is_relevant("B")
        assert not relevant_events.is_relevant("C")
        results = stormpy.dft.analyze_dft(dft, formulas, relevant_events = relevant_events)
        assert math.isclose(results[0], 0.1548181217)

    def test_relevant_events_additional(self):
        dft = stormpy.dft.load_dft_json_file(get_example_path("dft", "and.json"))
        properties = stormpy.parse_properties("P=? [ F<=1 \"failed\" ]")
        formulas = [p.raw_formula for p in properties]
        relevant_events = stormpy.dft.compute_relevant_events(dft, formulas, ["B", "C"])
        assert relevant_events.is_relevant("B")
        assert relevant_events.is_relevant("C")
        assert not relevant_events.is_relevant("A")
        results = stormpy.dft.analyze_dft(dft, formulas, relevant_events = relevant_events)
        assert math.isclose(results[0], 0.1548181217)

    def test_transformation(self):
        dft = stormpy.dft.load_dft_galileo_file(get_example_path("dft", "rc2.dft"))
        valid, output = stormpy.dft.is_well_formed(dft)
        assert not valid
        assert "not binary" in output
        dft = stormpy.dft.transform_dft(dft, unique_constant_be=True, binary_fdeps=True)
        valid, output = stormpy.dft.is_well_formed(dft)
        assert valid
        formulas = stormpy.parse_properties("Tmin=? [ F \"failed\" ]")
        results = stormpy.dft.analyze_dft(dft, [formulas[0].raw_formula])
        assert math.isclose(results[0], 6.380930905)

    def test_fdep_conflicts(self):
        dft = stormpy.dft.load_dft_galileo_file(get_example_path("dft", "rc2.dft"))
        dft = stormpy.dft.transform_dft(dft, unique_constant_be=True, binary_fdeps=True)
        has_conflicts = stormpy.dft.compute_dependency_conflicts(dft, use_smt=False, solver_timeout=0)
        assert not has_conflicts
        formulas = stormpy.parse_properties("T=? [ F \"failed\" ]")
        results = stormpy.dft.analyze_dft(dft, [formulas[0].raw_formula])
        assert math.isclose(results[0], 6.380930905)
