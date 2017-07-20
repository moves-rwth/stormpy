import pycarl
import sys
import os
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from package_selector import PackageSelector


class TestTerm(PackageSelector):

    def test_init(self, package):
        pycarl.clear_variable_pool()
        var = pycarl.Variable("x")
        rational = package.Rational(0.25)
        monomial = pycarl.create_monomial(var, 1)
        term = package.Term(rational, monomial)
        assert term.coeff == package.Rational(0.25)
        assert term.monomial == monomial
        assert str(term) == "1/4*x"

    def test_eq(self, package):
        pycarl.clear_variable_pool()
        var = pycarl.Variable("x")
        term1 = package.Integer(4) * var
        term1 *= var
        term2 = package.Term(2, pycarl.create_monomial(var, 2))
        assert term1 / 2 == term2

    def test_multiplication(self, package):
        pycarl.clear_variable_pool()
        var = pycarl.Variable("x")
        term1 = package.Integer(2) * var * var
        term2 = package.Integer(3) * var

        term3 = term1 * term2
        termOrig = package.Term(6, var*var*var)
        assert term3 == termOrig
