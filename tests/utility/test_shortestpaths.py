import stormpy
import stormpy.logic
from stormpy.storage import BitVector
from stormpy.utility import ShortestPathsGenerator
from helpers.helper import get_example_path

import pytest


@pytest.fixture(scope="module")
def model(program_path=get_example_path("dtmc", "die.pm"), raw_formula="P=? [ F \"one\" ]"):
    program = stormpy.parse_prism_program(program_path)
    formulas = stormpy.parse_formulas_for_prism_program(raw_formula, program)
    return stormpy.build_model(program, formulas[0])


@pytest.fixture
def state(model):
    some_state = 7
    assert model.nr_states > some_state, "test model too small"
    return some_state


@pytest.fixture
def state_list(model):
    some_state_list = [4, 5, 7]
    assert model.nr_states > max(some_state_list), "test model too small"
    return some_state_list


@pytest.fixture
def state_bitvector(model, state_list):
    return BitVector(length=model.nr_states, set_entries=state_list)


@pytest.fixture
def label(model):
    some_label = "one"
    assert some_label in model.labels, "test model does not contain label '" + some_label + "'"
    return some_label


class TestShortestPaths:
    def test_spg_ctor_bitvector_target(self, model, state_bitvector):
        _ = ShortestPathsGenerator(model, state_bitvector)

    def test_spg_ctor_single_state_target(self, model, state):
        _ = ShortestPathsGenerator(model, state)

    def test_spg_ctor_state_list_target(self, model, state_list):
        _ = ShortestPathsGenerator(model, state_list)

    def test_spg_ctor_label_target(self, model, label):
        _ = ShortestPathsGenerator(model, label)


    # TODO: add tests that check actual functionality
