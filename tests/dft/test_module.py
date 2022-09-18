import os

import stormpy
from helpers.helper import get_example_path

from configurations import dft


@dft
class TestModule:
    def test_module(self):
        dft = stormpy.dft.load_dft_galileo_file(get_example_path("dft", "hecs.dft"))
        assert dft.nr_elements() == 23
        assert dft.nr_be() == 13
        assert dft.nr_dynamic() == 2
        module = dft.modules();
        assert dft.get_element(module.representative()).name == "n0"
        assert len(module.submodules()) == 3
        assert len(module.elements()) == 2
        for submodule in module.submodules():
            assert dft.get_element(submodule.representative()).name in ["n137", "n120", "n21"]
            assert len(submodule.elements()) in [3, 4, 14]
            assert len(submodule.submodules()) == 0

    def test_module_json(self):
        dft = stormpy.dft.load_dft_galileo_file(get_example_path("dft", "hecs.dft"))
        json = stormpy.dft.modules_json(dft)
        assert json['representative']['name'] == "n0"
        assert len(json['elements']) == 2
        assert len(json['submodules']) == 3
        for submodule in json["submodules"]:
            assert submodule['representative']['name'] in ["n137", "n120", "n21"]
            assert len(submodule['elements']) in [3, 4, 14]
            assert len(submodule['submodules']) == 0
