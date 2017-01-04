import pycarl

class TestFactorizedRationalFunction:

    def test_init(self):
        pol1 = pycarl.FactorizedPolynomial(32)
        pol2 = pycarl.FactorizedPolynomial(2)
        rat = pycarl.FactorizedRationalFunction(pol1, pol2)
        assert str(rat) == "16"
