import stormpy
import stormpy.core

import stormpy.examples
import stormpy.examples.files


def example_schedulers_01():
    path = stormpy.examples.files.prism_mdp_coin_2_2
    formula_str = "Pmin=? [F \"finished\" & \"all_coins_equal_1\"]"

    program = stormpy.parse_prism_program(path)
    formulas = stormpy.parse_properties_for_prism_program(formula_str, program)
    model = stormpy.build_model(program, formulas)
    initial_state = model.initial_states[0]
    assert initial_state == 0
    result = stormpy.model_checking(model, formulas[0], extract_scheduler=True)
    assert result.has_scheduler
    scheduler = result.scheduler
    print(scheduler)
    assert scheduler.memoryless
    assert scheduler.deterministic

    for state in model.states:
        choice = scheduler.get_choice(state)
        action = choice.get_deterministic_choice()
        print("In state {} choose action {}".format(state, action))

    dtmc = model.apply_scheduler(scheduler)
    print(dtmc)



if __name__ == '__main__':
    example_schedulers_01()
