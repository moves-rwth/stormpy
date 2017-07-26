import pycarl
from configurations import cln


@cln
class TestGmpConvert():
    def test_convert_integer(self):
        original = pycarl.cln.Integer(23)
        assert isinstance(original, pycarl.cln.Integer)
        converted = pycarl.convert.convert_to_gmp(original)
        assert isinstance(converted, pycarl.gmp.Integer)

    def test_convert_rational(self):
        original = pycarl.cln.Rational(pycarl.cln.Integer(12), pycarl.cln.Integer(-4))
        assert isinstance(original, pycarl.cln.Rational)
        converted = pycarl.convert.convert_to_gmp(original)
        assert isinstance(converted, pycarl.gmp.Rational)

    def test_convert_term(self):
        pycarl.clear_variable_pool()
        var = pycarl.Variable("n")
        rational = pycarl.cln.Integer(-1) / pycarl.cln.Integer(6)
        monomial = pycarl.create_monomial(var, 3)
        original = pycarl.cln.Term(rational, monomial)
        assert isinstance(original, pycarl.cln.Term)
        converted = pycarl.convert.convert_to_gmp(original)
        assert isinstance(converted, pycarl.gmp.Term)

    def test_convert_polynomial(self):
        pycarl.clear_variable_pool()
        var1 = pycarl.Variable("n")
        var2 = pycarl.Variable("o")
        original = pycarl.cln.Polynomial(5) * var2 * var1 + var2 * var2 + pycarl.cln.Integer(3)
        assert isinstance(original, pycarl.cln.Polynomial)
        converted = pycarl.convert.convert_to_gmp(original)
        assert isinstance(converted, pycarl.gmp.Polynomial)

    def test_convert_rational_function(self):
        pycarl.clear_variable_pool()
        var1 = pycarl.Variable("x")
        var2 = pycarl.Variable("y")
        pol1 = var1 * var2 + pycarl.cln.Integer(42)
        pol2 = var1 + pycarl.cln.Integer(-1)
        original = pycarl.cln.RationalFunction(pol1, pol2)
        assert isinstance(original, pycarl.cln.RationalFunction)
        converted = pycarl.convert.convert_to_gmp(original)
        assert isinstance(converted, pycarl.gmp.RationalFunction)

    def test_convert_factorized_polynomial(self):
        pycarl.clear_variable_pool()
        var1 = pycarl.Variable("a")
        var2 = pycarl.Variable("b")
        pol1 = pycarl.cln.create_factorized_polynomial(pycarl.cln.Polynomial(4) * (var2 + pycarl.cln.Integer(-2)))
        pol2 = pycarl.cln.create_factorized_polynomial(pycarl.cln.Polynomial(var1) - 3)
        pol3 = pycarl.cln.create_factorized_polynomial(pycarl.cln.Polynomial(2) * var2)
        original = pol1 * pol2 * pol3
        assert isinstance(original, pycarl.cln.FactorizedPolynomial)
        converted = pycarl.convert.convert_to_gmp(original)
        assert isinstance(converted, pycarl.gmp.FactorizedPolynomial)
        assert len(converted.factorization()) == len(original.factorization())

    def test_convert_factorized_rational_function(self):
        pycarl.clear_variable_pool()
        var1 = pycarl.Variable("a")
        var2 = pycarl.Variable("b")
        pol1 = pycarl.cln.create_factorized_polynomial(pycarl.cln.Polynomial(55) * (var1 + pycarl.cln.Integer(20)))
        pol2 = pycarl.cln.create_factorized_polynomial(pycarl.cln.Polynomial(var2) - 2)
        pol3 = pycarl.cln.create_factorized_polynomial(pycarl.cln.Polynomial(562) * var2)
        original = pycarl.cln.FactorizedRationalFunction(pol1 * pol2, pol2 * pol3)
        assert isinstance(original, pycarl.cln.FactorizedRationalFunction)
        converted = pycarl.convert.convert_to_gmp(original)
        assert isinstance(converted, pycarl.gmp.FactorizedRationalFunction)
        assert len(converted.numerator.factorization()) == len(original.numerator.factorization())
        assert len(converted.denominator.factorization()) == len(original.denominator.factorization())

    def test_convert_constraint(self):
        pycarl.clear_variable_pool()
        var1 = pycarl.Variable("a")
        pol1 = pycarl.cln.Polynomial(2) * var1 * var1 + var1 + pycarl.cln.Integer(4)
        original = pycarl.cln.formula.Constraint(pol1, pycarl.formula.Relation.GREATER)
        assert isinstance(original, pycarl.cln.formula.Constraint)
        converted = pycarl.convert.convert_to_gmp(original)
        assert isinstance(converted, pycarl.gmp.formula.Constraint)
        assert converted.relation == original.relation
