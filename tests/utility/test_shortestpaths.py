import stormpy
import stormpy.logic
from stormpy.storage import BitVector
from stormpy.utility import ShortestPathsGenerator
from stormpy.utility import MatrixFormat
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


@pytest.fixture
def transition_matrix(model):
    return model.transition_matrix


@pytest.fixture
def target_prob_map(model, state_list):
    return {i: (1.0 if i in state_list else 0.0) for i in range(model.nr_states)}


@pytest.fixture
def target_prob_list(target_prob_map):
    return [target_prob_map[i] for i in range(max(target_prob_map.keys()))]


@pytest.fixture
def initial_states(model):
    return BitVector(model.nr_states, model.initial_states)


@pytest.fixture
def matrix_format():
    return MatrixFormat.Straight


class TestShortestPaths:
    def test_spg_ctor_bitvector_target(self, model, state_bitvector):
        _ = ShortestPathsGenerator(model, state_bitvector)

    def test_spg_ctor_single_state_target(self, model, state):
        _ = ShortestPathsGenerator(model, state)

    def test_spg_ctor_state_list_target(self, model, state_list):
        _ = ShortestPathsGenerator(model, state_list)

    def test_spg_ctor_label_target(self, model, label):
        _ = ShortestPathsGenerator(model, label)

    def test_spg_ctor_matrix_vector(self, transition_matrix, target_prob_list, initial_states, matrix_format):
        _ = ShortestPathsGenerator(transition_matrix, target_prob_list, initial_states, matrix_format)

    def test_spg_ctor_matrix_map(self, transition_matrix, target_prob_map, initial_states, matrix_format):
        _ = ShortestPathsGenerator(transition_matrix, target_prob_map, initial_states, matrix_format)

    # TODO: add tests that check actual functionality
