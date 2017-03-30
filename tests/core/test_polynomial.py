import pycarl


class TestPolynomial:

    def test_eq(self):
        var = pycarl.Variable("x")
        self.pol1 = 2*var + 3*var*var
        assert not self.pol1 == pycarl.Rational(1)
        self.pol2 = var + 1 - var
        assert self.pol2 == pycarl.Rational(1)