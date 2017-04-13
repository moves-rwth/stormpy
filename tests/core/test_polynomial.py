import pycarl
import sys
import os
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from package_selector import PackageSelector



class TestPolynomial(PackageSelector):

    def test_eq(self, package):
        var = pycarl.Variable("x")
        self.pol1 = package.Integer(2)*var + package.Integer(3)*var*var
        assert not self.pol1 == package.Rational(1)
        self.pol2 = var + package.Integer(1) - var
        assert self.pol2 == package.Rational(1)
        assert self.pol2 == package.Integer(1)