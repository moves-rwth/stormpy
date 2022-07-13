import stormpy
from helpers.helper import get_example_path

import math
from configurations import dft


@dft
class TestSimulator:

    def test_random_steps(self):
        dft = stormpy.dft.load_dft_json_file(get_example_path("dft", "and.json"))
        symmetries = dft.symmetries()
        relevant_events = stormpy.dft.compute_relevant_events(dft, [])
        dft.set_relevant_events(relevant_events, False)
        info = dft.state_generation_info(symmetries)
        generator = stormpy.dft.RandomGenerator.create(5)
        simulator = stormpy.dft.DFTSimulator_double(dft, info, generator)
        res, time = simulator.random_step()
        assert res == stormpy.dft.SimulationResult.SUCCESSFUL
        assert time > 0
        res, time = simulator.random_step()
        assert res == stormpy.dft.SimulationResult.SUCCESSFUL
        assert time > 0
        res, time = simulator.random_step()
        assert res == stormpy.dft.SimulationResult.UNSUCCESSFUL
        assert time <= 0
        res, time = simulator.random_step()
        assert res == stormpy.dft.SimulationResult.UNSUCCESSFUL
        assert time <= 0

    def test_simulate_trace(self):
        dft = stormpy.dft.load_dft_json_file(get_example_path("dft", "and.json"))
        symmetries = dft.symmetries()
        relevant_events = stormpy.dft.compute_relevant_events(dft, [])
        dft.set_relevant_events(relevant_events, False)
        info = dft.state_generation_info(symmetries)
        generator = stormpy.dft.RandomGenerator.create(5)
        simulator = stormpy.dft.DFTSimulator_double(dft, info, generator)
        res = simulator.simulate_trace(2)
        assert res == stormpy.dft.SimulationResult.SUCCESSFUL
        res = simulator.simulate_trace(2)
        assert res == stormpy.dft.SimulationResult.UNSUCCESSFUL
        res = simulator.simulate_trace(2)
        assert res == stormpy.dft.SimulationResult.UNSUCCESSFUL

    def test_simulate_trace(self):
        dft = stormpy.dft.load_dft_galileo_file(get_example_path("dft", "rc2.dft"))
        dft = stormpy.dft.prepare_for_analysis(dft)
        symmetries = dft.symmetries()
        relevant_events = stormpy.dft.compute_relevant_events(dft, [])
        dft.set_relevant_events(relevant_events, False)
        info = dft.state_generation_info(symmetries)
        generator = stormpy.dft.RandomGenerator.create(5)
        simulator = stormpy.dft.DFTSimulator_double(dft, info, generator)
        res = simulator.simulate_trace(2)
        assert res == stormpy.dft.SimulationResult.UNSUCCESSFUL
        res = simulator.simulate_trace(2)
        assert res == stormpy.dft.SimulationResult.SUCCESSFUL
        res = simulator.simulate_trace(2)
        assert res == stormpy.dft.SimulationResult.UNSUCCESSFUL

    def test_steps(self):
        dft = stormpy.dft.load_dft_json_file(get_example_path("dft", "and.json"))
        symmetries = dft.symmetries()
        relevant_events = stormpy.dft.compute_relevant_events(dft, [])
        dft.set_relevant_events(relevant_events, False)
        info = dft.state_generation_info(symmetries)
        generator = stormpy.dft.RandomGenerator.create(5)
        simulator = stormpy.dft.DFTSimulator_double(dft, info, generator)

        failable_check = ["B", "C"]
        a = dft.get_element_by_name("A").id
        b = dft.get_element_by_name("B").id
        c = dft.get_element_by_name("C").id

        state = simulator.current()
        assert state.operational(a)
        assert not state.failed(a)
        assert state.operational(b)
        assert not state.failed(b)
        assert state.operational(c)
        assert not state.failed(c)

        # Let C fail
        failable = state.failable()
        for f in failable:
            fail_be = f.get_fail_be_double(dft)
            assert fail_be[0].name in failable_check
            if fail_be[0].name == "C":
                next_fail = f
        res = simulator.step(next_fail)
        assert res == stormpy.dft.SimulationResult.SUCCESSFUL
        state = simulator.current()
        assert state.operational(a)
        assert not state.failed(a)
        assert state.operational(b)
        assert not state.failed(b)
        assert not state.operational(c)
        assert state.failed(c)

        # Let B fail
        state = simulator.current()
        failable = state.failable()
        for f in failable:
            fail_be = f.get_fail_be_double(dft)
            assert fail_be[0].name == "B"
            next_fail = f
        res = simulator.step(next_fail)
        assert res == stormpy.dft.SimulationResult.SUCCESSFUL
        state = simulator.current()
        assert not state.operational(a)
        assert state.failed(a)
        assert not state.operational(b)
        assert state.failed(b)
        assert not state.operational(c)
        assert state.failed(c)

        failable = state.failable()
        for f in failable:
            assert False # no failable elements

    def test_steps_dependency(self):
        dft = stormpy.dft.load_dft_galileo_file(get_example_path("dft", "fdep.dft"))
        dft = stormpy.dft.prepare_for_analysis(dft)
        symmetries = dft.symmetries()
        relevant_events = stormpy.dft.compute_relevant_events(dft, [])
        dft.set_relevant_events(relevant_events, False)
        info = dft.state_generation_info(symmetries)
        generator = stormpy.dft.RandomGenerator.create(5)
        simulator = stormpy.dft.DFTSimulator_double(dft, info, generator)

        p = dft.get_element_by_name("P").id
        b = dft.get_element_by_name("B").id
        power = dft.get_element_by_name("B_Power").id

        state = simulator.current()
        assert state.operational(p)
        assert state.operational(b)
        assert state.operational(power)

        # Let B_Power fail
        failable = state.failable()
        for f in failable:
            fail_be = f.get_fail_be_double(dft)
            assert fail_be[0].name in ["B", "P", "B_Power"]
            if fail_be[0].name == "B_Power":
                next_fail = f
        res = simulator.step(next_fail)
        assert res == stormpy.dft.SimulationResult.SUCCESSFUL
        state = simulator.current()
        assert state.operational(p)
        assert state.operational(b)
        assert state.failed(power)

        # Let B fail
        failable = state.failable()
        for f in failable:
            fail_be = f.get_fail_be_double(dft)
            assert fail_be[0].name in ["B", "P"]
            if fail_be[0].name == "B":
                next_fail = f
        res = simulator.step(next_fail)
        assert res == stormpy.dft.SimulationResult.SUCCESSFUL
        state = simulator.current()
        assert state.failed(p)
        assert state.failed(b)
        assert state.failed(power)

        failable = state.failable()
        for f in failable:
            assert False  # no failable elements
