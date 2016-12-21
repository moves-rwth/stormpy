import pycarl

class TestCore:
    def test_init(self):
        var = pycarl.Variable("x")
        monom = pycarl.Monomial(var)
