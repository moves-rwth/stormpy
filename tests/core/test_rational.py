import pycarl
import sys
import os
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from package_selector import PackageSelector

class TestRational(PackageSelector):

    def test_parse(self, package):
        r1 = package.Rational("1/2")
        assert package.numerator(r1) == 1
        assert package.denominator(r1) == 2
        r2 = package.Rational("1090843549285935/1125899906842624")
        assert package.numerator(r2) == 1090843549285935
        assert package.denominator(r2) == 1125899906842624
