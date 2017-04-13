import pycarl

import sys
import os
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from package_selector import PackageSelector


class TestFactorizedRationalFunction(PackageSelector):

    def test_init(self, package):
        pol1 = package.FactorizedPolynomial(32)
        pol2 = package.FactorizedPolynomial(2)
        rat = package.FactorizedRationalFunction(pol1, pol2)
        assert str(rat) == "16"
    
    def test_derivation(self, package):
        x = pycarl.Variable("x")
        cache = package.FactorizationCache()
        p1 = package.FactorizedPolynomial(x*x+package.Integer(3), cache)
        p2 = package.FactorizedPolynomial(x+package.Integer(1), cache)

        rat = package.FactorizedRationalFunction(p1, p2)
        derivation = rat.derive(x)
        print("{}': {}".format(rat, derivation))
