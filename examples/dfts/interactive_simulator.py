import stormpy
import stormpy.dft

import argparse
import logging
import pathlib


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


def get_status(state, dft):
    if state.invalid():
        return "State is invalid because a SEQ is violated"
    state_tle = state.failed(dft.top_level_element.id)
    s = "DFT is {}".format("Failed" if state_tle else "Operational")
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
        s += "\n\t{0: <20}: {1}".format(elem.name, status)
    return s


def get_failables(state, dft):
    failables = dict()
    for f in state.failable():
        fail_be = f.get_fail_be_double(dft)
        failables[fail_be[0].name] = f
    return failables


def get_user_fail_be(failables):
    while True:
        inp = input("Select index of BE to fail next (use 'q' to abort)\n")
        if inp == 'q':
            return None
        try:
            inp_number = int(inp)
        except ValueError:
            logging.error("The input is not a number.")
        else:
            if 0 <= inp_number < len(failables):
                chosen_name = failables[inp_number]
                assert chosen_name in failables
                return chosen_name
            else:
                logging.error("Selected index is not valid.")


def main():
    parser = argparse.ArgumentParser(description='Interactive simulator for DFTs.')

    parser.add_argument('infile', help='Input Galileo file', type=str)

    parser.add_argument('--verbose', '-v', help='print more output', action="store_true")
    args = parser.parse_args()

    logging.basicConfig(format='%(levelname)s: %(message)s', level=logging.DEBUG if args.verbose else logging.INFO)

    # Load DFT from file
    if pathlib.Path(args.infile).suffix == ".json":
        dft = stormpy.dft.load_dft_json_file(args.infile)
    else:
        dft = stormpy.dft.load_dft_galileo_file(args.infile)
    logging.info("Loaded DFT with {} elements, {} BEs and {} dynamic elements.".format(dft.nr_elements(), dft.nr_be(), dft.nr_dynamic()))
    dft = stormpy.dft.prepare_for_analysis(dft)

    # Create simulator
    simulator = get_simulator(dft, seed=42)

    # Get initial state
    state = simulator.current()
    logging.info("Initial status:")
    logging.info(get_status(state, dft))

    while True:
        failables = get_failables(state, dft)
        if len(failables.keys()) == 0:
            logging.info("All BEs have failed -> will terminate")
            return
        failable_names = list(failables.keys())
        logging.info("Failable BEs:\n" + "\n".join(["\t{}: {}".format(i, failable_names[i]) for i in range(len(failable_names))]))
        chosen = get_user_fail_be(failable_names)
        if chosen is None:
            # Exit
            return
        logging.debug("Let {} fail".format(chosen))
        res = simulator.step(failables[chosen])
        logging.debug("Simulation was {}".format(res))
        state = simulator.current()
        logging.info("Status after failure of {}:".format(chosen))
        logging.info(get_status(state, dft))


if __name__ == "__main__":
    main()
