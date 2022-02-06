import stormpy
import stormpy.dft

import stormpy.examples
import stormpy.examples.files


def get_simulator(dft, seed=42):
    # Compute all symmetries
    symmetries = dft.symmetries()
    # Set only top event as relevant
    relevant_events = stormpy.dft.compute_relevant_events(dft, [])
    dft.set_relevant_events(relevant_events, False)
    # Create information for state space generation
    info = dft.state_generation_info(symmetries)
    # Initialize random generator
    generator = stormpy.dft.RandomGenerator.create(seed)
    # Create simulator
    return stormpy.dft.DFTSimulator_double(dft, info, generator)


def print_status(state, dft):
    if state.invalid():
        print("State is invalid because a SEQ is violated")
        return
    state_tle = state.failed(dft.top_level_element.id)
    print("DFT is {}".format("Failed" if state_tle else "Operational"))
    for i in range(dft.nr_elements()):
        # Order of checks is important!
        if state.operational(i):
            status = "Operational"
        elif state.dontcare(i):
            status = "Don't Care"
        elif state.failsafe(i):
            status = "FailSafe"
        elif state.failed(i):
            status = "Failed"
        else:
            status = "Unknown"
        elem = dft.get_element(i)
        if elem.type == stormpy.dft.DFTElementType.SPARE:
            cur_used = state.spare_uses(i)
            if cur_used == i:
                status += ", not using anything"
            else:
                elem_used = dft.get_element(cur_used)
                status += ", currently using {}".format(elem_used.name)
        print("\t{}: {}".format(elem.name, status))


def get_failables(state, dft):
    failables = dict()
    for f in state.failable():
        fail_be = f.get_fail_be_double(dft)
        failables[fail_be[0].name] = f
    return failables


def analyze_via_simulation(simulator, timebound, no_simulations=1000):
    success = 0
    for i in range(no_simulations):
        res = simulator.simulate_trace(timebound)
        if res == stormpy.dft.SimulationResult.SUCCESSFUL:
            success += 1
    return success / no_simulations


def example_dft_simulator_02():
    # Load DFT from Galileo
    path = stormpy.examples.files.dft_galileo_hecs
    dft = stormpy.dft.load_dft_galileo_file(path)
    print("DFT with {} elements, {} BEs and {} dynamic elements.".format(dft.nr_elements(), dft.nr_be(),
                                                                         dft.nr_dynamic()))

    # Create simulator
    simulator = get_simulator(dft, seed=42)

    # Get initial state
    state = simulator.current()
    print_status(state, dft)

    choices = ["n15", "n118", "n134", "n23", "n7"]
    terminate = False

    i = 0
    while not terminate:
        failables = get_failables(state, dft)
        print("Failable BEs: {}".format(", ".join(failables.keys())))
        chosen = choices[i]
        i += 1
        assert chosen in failables
        print("Let {} fail".format(chosen))
        res = simulator.step(failables[chosen])
        print("Simulation was {}".format(res))
        state = simulator.current()
        print_status(state, dft)
        terminate = i >= len(choices)

    # Analyze DFT via simulation
    timebound = 5
    no_runs = 1000
    print("Perform {} simulation runs for timebound {}".format(no_runs, timebound))
    result = analyze_via_simulation(simulator, timebound, no_simulations=no_runs)
    print("Result for time bound {}: {}".format(timebound, result))


if __name__ == '__main__':
    example_dft_simulator_02()
