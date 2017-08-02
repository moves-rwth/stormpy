import stormpy
import stormpy.info
import stormpy.logic
from helpers.helper import get_example_path


class TestParametric:
    def test_constraints_collector(self):
        from pycarl.formula import FormulaType, Relation
        if stormpy.info.storm_ratfunc_use_cln():
            import pycarl.cln.formula
        else:
            import pycarl.gmp.formula
        program = stormpy.parse_prism_program(get_example_path("pdtmc", "brp16_2.pm"))
        prop = "P=? [F s=5]"
        formulas = stormpy.parse_properties_for_prism_program(prop, program)
        model = stormpy.build_parametric_model(program, formulas)
        collector = stormpy.ConstraintCollector(model)
        constraints_well_formed = collector.wellformed_constraints
        for formula in constraints_well_formed:
            assert formula.type == FormulaType.CONSTRAINT
            constraint = formula.get_constraint()
            assert constraint.relation == Relation.LEQ
        constraints_graph_preserving = collector.graph_preserving_constraints
        for formula in constraints_graph_preserving:
            assert formula.type == FormulaType.CONSTRAINT
            constraint = formula.get_constraint()
            assert constraint.relation == Relation.NEQ

    def test_derivatives(self):
        program = stormpy.parse_prism_program(get_example_path("pdtmc", "brp16_2.pm"))
        prop = "P<=0.84 [F s=5 ]"
        formulas = stormpy.parse_properties_for_prism_program(prop, program)
        model = stormpy.build_parametric_model(program, formulas)
        assert model.nr_states == 613
        assert model.nr_transitions == 803
        assert model.model_type == stormpy.ModelType.DTMC
        assert model.has_parameters
        parameters = model.collect_probability_parameters()
        assert len(parameters) == 2
        derivatives = stormpy.pars.gather_derivatives(model, list(parameters)[0])
        assert len(derivatives) == 0
