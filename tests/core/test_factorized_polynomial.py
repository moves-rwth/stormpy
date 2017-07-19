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
        x = pycarl.Variable("x")
        cache = package.FactorizationCache()
        p1 = package.FactorizedPolynomial(x*x+package.Integer(3), cache)
        p1.cache()

    def test_derivation(self, package):
        x = pycarl.Variable("x")
        c = package.FactorizedPolynomial(3)
        derivation = c.derive(x)
        assert(derivation == package.Rational(0))

        cache = package.FactorizationCache()
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
