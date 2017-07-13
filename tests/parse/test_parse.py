import pycarl
import pycarl._config
import pycarl.gmp.formula
import pycarl.cln.formula

if  pycarl._config.CARL_PARSER:
    import pycarl.parse
    import pycarl.cln.parse, pycarl.gmp.parse

import pytest
import sys
import os
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from package_selector import PackageSelector

@pytest.mark.skipif(not pycarl._config.CARL_PARSER,
                    reason="requires carlparser")
class TestParse(PackageSelector):


    def test_parse_number(self, package):
        num = pycarl.parse.deserialize("2", package)
        assert num == package.Rational(2)
        num = pycarl.parse.deserialize("-2", package)
        assert num  == package.Rational(-2)
        num = pycarl.parse.deserialize("(- 2)", package)
        assert num  == package.Rational(-2)

    def test_parse_polynomial(self, package):
        pol = pycarl.parse.deserialize("(+ y 1)", package)
        assert str(pol) == "y+1"

    def test_parse_constraint(self, package):
        constraint = pycarl.parse.deserialize("(< x 0)", package)
        assert str(constraint) == "x<0"
        constraint = pycarl.parse.deserialize("(< x 1)", package)
        x = pycarl.Variable("x")
        assert constraint.lhs == x - package.Rational(1)
        constraint = pycarl.parse.deserialize("(<= (* (- 1) x) 0)", package)
        assert constraint.lhs == -package.Polynomial(x)
