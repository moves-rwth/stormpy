import pycarl

class TestFactorizedRationalFunction:

    def test_init(self):
        pol1 = pycarl.FactorizedPolynomial(32)
        pol2 = pycarl.FactorizedPolynomial(2)
        rat = pycarl.FactorizedRationalFunction(pol1, pol2)
        assert str(rat) == "16"
    
    def test_derivation(self):
        x = pycarl.Variable("x")
        cache = pycarl.FactorizationCache()
        p1 = pycarl.FactorizedPolynomial(x*x+3, cache)
        p2 = pycarl.FactorizedPolynomial(x+1, cache)

        rat = pycarl.FactorizedRationalFunction(p1, p2)
        derivation = rat.derive(x)
        print("{}': {}".format(rat, derivation))
