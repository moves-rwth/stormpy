import stormpy
import stormpy.logic
from stormpy.storage import BitVector
from stormpy.utility import ShortestPathsGenerator
from helpers.helper import get_example_path

import pytest


@pytest.fixture(scope="module")
def test_model(program_path=get_example_path("dtmc", "die.pm"), raw_formula="P=? [ F \"one\" ]"):
    program = stormpy.parse_prism_program(program_path)
    formulas = stormpy.parse_formulas_for_prism_program(raw_formula, program)
    test_model = stormpy.build_model(program, formulas[0])
    return test_model


@pytest.fixture
def test_state(test_model):
    some_state = 7
    assert test_model.nr_states > some_state, "test model too small"
    return some_state


@pytest.fixture
def test_state_list(test_model):
    some_state_list = [4, 5, 7]
    assert test_model.nr_states > max(some_state_list), "test model too small"
    return some_state_list


@pytest.fixture
def test_state_bitvector(test_model, test_state_list):
    return BitVector(length=test_model.nr_states, set_entries=test_state_list)


@pytest.fixture
def test_label(test_model):
    some_label = "one"
    assert some_label in test_model.labels, "test model does not contain label '" + some_label + "'"
    return some_label


class TestShortestPaths:
    def test_spg_ctor_bitvector_target(self, test_model, test_state_bitvector):
        _ = ShortestPathsGenerator(test_model, test_state_bitvector)

    def test_spg_ctor_single_state_target(self, test_model, test_state):
        _ = ShortestPathsGenerator(test_model, test_state)

    def test_spg_ctor_state_list_target(self, test_model, test_state_list):
        _ = ShortestPathsGenerator(test_model, test_state_list)

    def test_spg_ctor_label_target(self, test_model, test_label):
        _ = ShortestPathsGenerator(test_model, test_label)

    # TODO: add tests that check actual functionality
