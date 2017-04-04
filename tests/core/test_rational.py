import pycarl


class TestRational:

    def test_parse(self):
        r1 = pycarl.Rational("1/2")
        assert pycarl.numerator(r1) == 1
        assert pycarl.denominator(r1) == 2
        r2 = pycarl.Rational("1090843549285935/1125899906842624")
        assert pycarl.numerator(r2) == 1090843549285935
        assert pycarl.denominator(r2) == 1125899906842624
