import stormpy
from helpers.helper import get_example_path


class TestMaximalEndComponents:
    def test_create_decomposition(self):
        program = stormpy.parse_prism_program(get_example_path("mdp", "two_dice.nm"))
        model = stormpy.build_model(program)

        decomposition = stormpy.MaximalEndComponentDecomposition_double(model)
        assert decomposition.size == 36
        for mec in decomposition:
            assert mec.size == 1
            for state, choices in mec:
                assert 133 <= state <= 168
                assert len(choices) == 2

    def test_create_decomposition(self):
        program = stormpy.parse_prism_program(get_example_path("mdp", "maze_2.nm"))
        model = stormpy.build_model(program)

        decomposition = stormpy.MaximalEndComponentDecomposition_double(model)
        assert decomposition.size == 2
        for mec in decomposition:
            if mec.size == 1:
                for state, choices in mec:
                    assert state == 14
                    assert 53 in choices
            else:
                assert mec.size == 13
                for state, choices in mec:
                    if state != 10:
                        assert len(choices) == 4
                        for i in range(4):
                            assert (state - 1) * 4 + 1 + i in choices
                    else:
                        assert len(choices) == 3
                        for i in range(3):
                            assert 37 + i in choices
