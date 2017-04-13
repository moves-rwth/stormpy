import pycarl

import sys
import os
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from package_selector import PackageSelector


class TestClnInteger(PackageSelector):


    def test_construction(self, package):
        i3 = package.Integer("3")
        assert str(i3) == "3"
        i4 = package.Integer(4)
        assert str(i4) == "4"

    def test_eq(self, package):
        i3 = package.Integer(3)
        assert i3 == 3
        i3b =  package.Integer(3)
        assert i3 == i3b
        in3 = package.Integer(-3)
        assert in3 == -3

    def test_addition(self, package):
        i3 = package.Integer(3)
        assert i3 + i3 == 6
        assert i3 + 4 == 7
        assert i3 + -3 == 0

    def test_neg(self, package):
        i3 = package.Integer(3)
        in3 = -i3
        assert in3 == -3
        inn3 = -in3
        assert i3 == inn3


