import pycarl
import sys
import os
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from package_selector import PackageSelector



class TestPolynomial(PackageSelector):

    def test_eq(self, package):
        pycarl.clear_variable_pool()
        var = pycarl.Variable("x")
        self.pol1 = package.Integer(2)*var + package.Integer(3)*var*var
        assert not self.pol1 == package.Rational(1)
        self.pol2 = var + package.Integer(1) - var
        assert self.pol2 == package.Rational(1)
        assert self.pol2 == package.Integer(1)

    def test_init(self, package):
        pycarl.clear_variable_pool()
        var = pycarl.Variable("x")
        pol = package.Polynomial(var)
        assert str(pol) == "x"

    def test_addition(self, package):
        pycarl.clear_variable_pool()
        var = pycarl.Variable("x")
        pol1 = var * var +  package.Integer(2)
        pol2 = var + package.Integer(1)
        pol3 = pol1 + pol2
        polOrig = package.Polynomial(var * var)
        polOrig += var + package.Integer(3)
        assert pol3 == polOrig
