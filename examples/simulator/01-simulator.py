import stormpy
import stormpy.core
import stormpy.simulator

import stormpy.examples
import stormpy.examples.files


def example_simulator_01():
    path = stormpy.examples.files.prism_dtmc_die
    prism_program = stormpy.parse_prism_program(path)

    model = stormpy.build_model(prism_program)
    simulator = stormpy.simulator.create_simulator(model, seed=42)
    final_outcomes = dict()
    for n in range(1000):
        for i in range(100):
            observation = simulator.step()
        if observation not in final_outcomes:
            final_outcomes[observation] = 1
        else:
            final_outcomes[observation] += 1
        simulator.restart()
    print(final_outcomes)

if __name__ == '__main__':
    example_simulator_01()
