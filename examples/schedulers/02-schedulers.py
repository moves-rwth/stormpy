import stormpy
import stormpy.core

import stormpy.examples
import stormpy.examples.files


def example_schedulers_02():
    path = stormpy.examples.files.prism_ma_simple
    formula_str = "Tmin=? [ F s=4 ]"

    program = stormpy.parse_prism_program(path, False, True)
    formulas = stormpy.parse_properties_for_prism_program(formula_str, program)
    ma = stormpy.build_model(program, formulas)
    assert ma.model_type == stormpy.ModelType.MA

    # Convert MA to MDP
    mdp, mdp_formulas = stormpy.transform_to_discrete_time_model(ma, formulas)
    assert mdp.model_type == stormpy.ModelType.MDP
    initial_state = mdp.initial_states[0]
    assert initial_state == 0

    result = stormpy.model_checking(mdp, mdp_formulas[0], extract_scheduler=True)
    assert result.has_scheduler
    scheduler = result.scheduler
    print(scheduler)
    assert scheduler.memoryless
    assert scheduler.deterministic

    for state in mdp.states:
        choice = scheduler.get_choice(state)
        action = choice.get_deterministic_choice()
        print("In state {} choose action {}".format(state, action))


if __name__ == '__main__':
    example_schedulers_02()
