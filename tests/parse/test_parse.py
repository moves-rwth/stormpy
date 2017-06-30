import pycarl
import pycarl.parse
import pycarl.cln.parse, pycarl.gmp.parse
import sys
import os
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from package_selector import PackageSelector


class TestParse(PackageSelector):
    def test_parse_number(self, package):
        num = pycarl.parse.deserialize("2", package)
        assert str(num) == "2"

    def test_parse_polynomial(self, package):
        pol = pycarl.parse.deserialize("(+ y 1)", package)
        assert str(pol) == "y+1"

    def test_parse_constraint(self, package):
        constraint = pycarl.parse.deserialize("(< x 0)", package)
        assert str(constraint) == "x<0"
        constraint = pycarl.parse.deserialize("(< x 1)", package)
        x = pycarl.Variable("x")
        assert constraint.lhs == x - package.Rational(1)
