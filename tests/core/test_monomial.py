import pycarl


class TestCore:
    def test_init(self):
        var = pycarl.Variable("x")
        exp = 1
        monom = pycarl.core.create_monomial(var, exp)
