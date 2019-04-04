import stormpy
import stormpy.logic
from helpers.helper import get_example_path
import pytest


class TestScheduler:

    def test_scheduler_mdp(self):
        program = stormpy.parse_prism_program(get_example_path("mdp", "coin2-2.nm"))
        formulas = stormpy.parse_properties_for_prism_program("Pmin=? [ F \"finished\" & \"all_coins_equal_1\"]", program)
        model = stormpy.build_model(program, formulas)
        assert model.nr_states == 272
        assert model.nr_transitions == 492
        assert len(model.initial_states) == 1
        initial_state = model.initial_states[0]
        assert initial_state == 0
        result = stormpy.model_checking(model, formulas[0], extract_scheduler=True)
        assert result.has_scheduler
        scheduler = result.scheduler
        assert scheduler.memoryless
        assert scheduler.memory_size == 1
        assert scheduler.deterministic
        for state in model.states:
            choice = scheduler.get_choice(state)
            assert choice.defined
            assert choice.deterministic
            action = choice.get_deterministic_choice()
            assert 0 <= action
            assert action < len(state.actions)
