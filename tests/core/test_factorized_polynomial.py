import pycarl

import sys
import os
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from package_selector import PackageSelector




class TestFactorizedPolynomial(PackageSelector):


    def test_init(self, package):
        pol1 = package.FactorizedPolynomial(32)
        assert str(pol1) == "32"

    def test_cache(self, package):
        x = pycarl.Variable("x")
        cache = package.FactorizationCache()
        p1 = package.FactorizedPolynomial(x*x+package.Integer(3), cache)
        print(p1.cache())

    def test_derivation(self, package):
        x = pycarl.Variable("x")
        c = package.FactorizedPolynomial(3)
        print(c.derive(x))

        cache = package.FactorizationCache()
        p1 = package.FactorizedPolynomial(x*x+package.Integer(3), cache)
        p2 = package.FactorizedPolynomial(x+package.Integer(1), cache)

        pol = p1 * p2
        derivation = pol.derive(x)
        print("{}': {}".format(pol, derivation))
        d1 = p1.derive(x)
        print("{}': {}".format(p1, d1))
