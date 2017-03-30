import pycarl

class TestFactorizedPolynomial:

    def test_init(self):
        pol1 = pycarl.FactorizedPolynomial(32)
        assert str(pol1) == "32"

    def test_cache(self):
        x = pycarl.Variable("x")
        cache = pycarl.FactorizationCache()
        p1 = pycarl.FactorizedPolynomial(x*x+3, cache)
        print(p1.cache())

    def test_derivation(self):
        x = pycarl.Variable("x")
        c = pycarl.FactorizedPolynomial(3)
        print(c.derive(x))

        cache = pycarl.FactorizationCache()
        p1 = pycarl.FactorizedPolynomial(x*x+3, cache)
        p2 = pycarl.FactorizedPolynomial(x+1, cache)

        pol = p1 * p2
        derivation = pol.derive(x)
        print("{}': {}".format(pol, derivation))
        d1 = p1.derive(x)
        print("{}': {}".format(p1, d1))
