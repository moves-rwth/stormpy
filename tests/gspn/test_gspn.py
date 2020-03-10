import os

import stormpy
from helpers.helper import get_example_path

from configurations import gspn


@gspn
class TestGSPNBuilder:
    def test_build_gspn(self):
        # assert True
        # builder: ~/storm/src/storm-dft/transformations/DFTToGSPNTransformator.cpp
        name = "gspn_test"
        builder = stormpy.gspn.GSPNBuilder()
        builder.set_name(name)
        gspn = builder.build_gspn()
        assert gspn.name() == name


