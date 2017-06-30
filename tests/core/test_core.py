class TestCore:
    def test_init(self):
        import stormpy
    
    def test_pycarl(self):
        import stormpy
        import pycarl
        import pycarl.cln
        import pycarl.gmp
        import pycarl.formula
        pol1 = pycarl.gmp.FactorizedPolynomial(32)
        pol2 = pycarl.gmp.FactorizedPolynomial(2)
        rat = pycarl.gmp.FactorizedRationalFunction(pol1, pol2)
        assert str(rat) == "16"
