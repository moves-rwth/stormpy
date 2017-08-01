import pycarl
import stormpy
import stormpy.logic


class TestFormulas:
    def test_probability_formula(self):
        formula_str = "P=? [F \"one\"]"
        properties = stormpy.parse_properties(formula_str)
        formula = properties[0].raw_formula
        assert type(formula) == stormpy.logic.ProbabilityOperator
        assert len(properties) == 1
        assert str(formula) == formula_str

    def test_reward_formula(self):
        formula_str = "R=? [F \"one\"]"
        properties = stormpy.parse_properties(formula_str)
        formula = properties[0].raw_formula
        assert type(formula) == stormpy.logic.RewardOperator
        assert len(properties) == 1
        assert str(formula) == "R[exp]=? [F \"one\"]"

    def test_formula_list(self):
        formulas = []
        prop = "=? [F \"one\"]"
        forms = stormpy.parse_properties("P" + prop)
        formulas.append(forms[0].raw_formula)
        forms = stormpy.parse_properties("R" + prop)
        formulas.append(forms[0].raw_formula)
        assert len(formulas) == 2
        assert str(formulas[0]) == "P" + prop
        assert str(formulas[1]) == "R[exp]" + prop

    def test_bounds(self):
        prop = "P=? [F \"one\"]"
        formula = stormpy.parse_properties(prop)[0].raw_formula
        assert not formula.has_bound
        prop = "P<0.4 [F \"one\"]"
        formula = stormpy.parse_properties(prop)[0].raw_formula
        assert formula.has_bound
        assert formula.threshold == stormpy.Rational("0.4")
        assert formula.comparison_type == stormpy.logic.ComparisonType.LESS

    # setter not currently implemented (non-trivial due to Expression container)
    def test_set_bounds(self):
        import pycarl
        import pycarl.gmp
        prop = "P<0.4 [F \"one\"]"
        formula = stormpy.parse_properties(prop)[0].raw_formula
        expression_manager = stormpy.ExpressionManager()
        rational = pycarl.gmp.Rational("0.2")
        expression = expression_manager.create_rational(rational)
        formula.set_bound(stormpy.logic.ComparisonType.GEQ, expression)
        assert formula.threshold == pycarl.gmp.Rational("0.2")
        assert formula.comparison_type == stormpy.logic.ComparisonType.GEQ
        assert str(formula) == "P>=1/5 [F \"one\"]"

    def test_subformula(self):
        prop = "P=? [F \"one\"]"
        formula = stormpy.parse_properties(prop)[0].raw_formula
        assert type(formula) == stormpy.logic.ProbabilityOperator
        pathform = formula.subformula
        assert type(pathform) == stormpy.logic.EventuallyFormula
        labelform = pathform.subformula
        assert type(labelform) == stormpy.logic.AtomicLabelFormula
        prop = stormpy.core.Property("label-formula", labelform)
        assert prop.raw_formula == labelform
