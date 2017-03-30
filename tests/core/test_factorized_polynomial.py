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
