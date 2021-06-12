import stormpy
import stormpy.logic
import stormpy.simulator
from helpers.helper import get_example_path


class TestPrismSimulator:
    def test_negative_values(self):
        prism_program = stormpy.parse_prism_program(get_example_path("dtmc", "negativevals.pm"))
        prism_program = stormpy.preprocess_symbolic_input(prism_program, [], "")[0].as_prism_program()

        simulator = stormpy.simulator.create_simulator(prism_program, seed=42)
        simulator.set_action_mode(stormpy.simulator.SimulatorActionMode.GLOBAL_NAMES)
        state = simulator.restart()
        print(state)

        