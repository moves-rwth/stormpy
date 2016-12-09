import pycarl

class TestFactorizedPolynomial:

    def test_init(self):
        pol1 = pycarl.FactorizedPolynomial(32)
        assert str(pol1) == "32"
