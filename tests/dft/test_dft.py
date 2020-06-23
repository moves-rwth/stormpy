import os

import stormpy
import stormpy.logic
from helpers.helper import get_example_path

from configurations import dft


@dft
class TestDft:
    def test_modularisation(self):
        dft = stormpy.dft.load_dft_galileo_file(get_example_path("dft", "hecs.dft"))
        assert dft.nr_elements() == 23
        assert dft.nr_be() == 13
        assert dft.nr_dynamic() == 2
        dfts = dft.modularisation()
        assert len(dfts) == 4
        for ft in dfts:
            assert ft.top_level_element.name in ["n116", "n137", "n120", "n21"]


@dft
class TestDftElement:
    def test_element(self):
        dft = stormpy.dft.load_dft_json_file(get_example_path("dft", "and.json"))
        tle = dft.top_level_element
        assert dft.nr_elements() == 3
        assert dft.nr_be() == 2
        assert dft.nr_dynamic() == 0
        assert tle.id == 2
        assert tle.name == "A"

@dft
class TestDftSymmetries:
    def test_symmetries_small(self):
        dft = stormpy.dft.load_dft_json_file(get_example_path("dft", "and.json"))
        symmetries = dft.symmetries()
        assert len(symmetries.groups) == 1
        for index, group in symmetries.groups.items():
            assert len(group) == 1
            for syms in group:
                assert len(syms) == 2
                for elem in syms:
                    assert elem == 0 or elem == 1

    def test_symmetries(self):
        dft = stormpy.dft.load_dft_galileo_file(get_example_path("dft", "rc.dft"))
        symmetries = dft.symmetries()
        assert len(symmetries.groups) == 1
        for index, group in symmetries.groups.items():
            assert len(group) == 3
            i = 4
            for syms in group:
                assert len(syms) == 2
                for elem in syms:
                    assert elem == i or elem == i+3
                i += 1
