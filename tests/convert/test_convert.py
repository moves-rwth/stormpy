import pytest

import pycarl
import pycarl.cln
import pycarl.gmp
import pycarl.convert

import sys
import os
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from package_selector import PackageSelector

@pytest.mark.parametrize("convert_package,converter", [
        (pycarl.cln,pycarl.convert.cln_converter),
        (pycarl.gmp,pycarl.convert.gmp_converter)
    ], ids=["cln_converter", "gmp_converter"])
class TestConvert(PackageSelector):

    def test_rational(self, package, convert_package, converter):
        r1 = package.Rational(package.Integer(1), package.Integer(2))
        assert isinstance(r1, package.Rational)
        cr1 = converter.convert_rational(r1)
        assert isinstance(cr1, convert_package.Rational)

