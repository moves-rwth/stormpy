import pycarl

import sys
import os
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from package_selector import PackageSelector


class TestFactorizedPolynomial(PackageSelector):

    def test_init(self, package):
        pol1 = package.FactorizedPolynomial(32)
        assert pol1 == 32
        assert str(pol1) == "32"

    def test_cache(self, package):
        pycarl.clear_variable_pool()
        x = pycarl.Variable("x")
        cache = package.factorization_cache
        p1 = package.FactorizedPolynomial(x*x+package.Integer(3), cache)
        p1.cache()

    def test_factorization(self, package):
        pycarl.clear_variable_pool()
        var1 = pycarl.Variable("a")
        var2 = pycarl.Variable("b")
        cache = package.factorization_cache
        pol1 = package.FactorizedPolynomial(package.Polynomial(5) * (var1 + package.Integer(1)), cache)
        pol2 = package.FactorizedPolynomial(package.Polynomial(var1) + 1, cache)
        pol3 = package.FactorizedPolynomial(package.Polynomial(3) * var2, cache)
        res = pol1 * pol2 * pol3
        factorization = res.factorization()

        assert res.coefficient() == 15
        assert res.constant_part() == 0
        res1 = package.FactorizedPolynomial(package.Polynomial(var1) + 1, cache)
        res2 = package.FactorizedPolynomial(package.Polynomial(var2), cache)
        for factor in factorization:
            assert (factor[0] == res1 and factor[1] == 2) or (factor[0] == res2 and factor[1] == 1)
        assert len(factorization) == 2

    def test_derivation(self, package):
        pycarl.clear_variable_pool()
        x = pycarl.Variable("x")
        c = package.FactorizedPolynomial(3)
        derivation = c.derive(x)
        assert(derivation == package.Rational(0))

        cache = package.factorization_cache
        p1 = package.FactorizedPolynomial(x*x+package.Integer(3), cache)
        p2 = package.FactorizedPolynomial(x+package.Integer(1), cache)

        pol = p1 * p2
        derivation = pol.derive(x)
        expected = package.FactorizedPolynomial(package.Integer(3)*x*x+package.Integer(2)*x+package.Integer(3), cache)
        assert derivation == expected

        derivation = p1.derive(x)
        p3 = package.Polynomial(package.Integer(2)*x)
        expected = package.FactorizedPolynomial(p3, cache)
        assert derivation == expected
