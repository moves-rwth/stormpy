#include "formulae.h"
#include "storm/logic/Formulas.h"


void define_formulae(py::module& m) {
 
    py::enum_<storm::logic::ComparisonType>(m, "ComparisonType")
        .value("LESS", storm::logic::ComparisonType::Less)
        .value("LEQ", storm::logic::ComparisonType::LessEqual)
        .value("GREATER", storm::logic::ComparisonType::Greater)
        .value("GEQ", storm::logic::ComparisonType::GreaterEqual)
    ;

    py::class_<storm::logic::Formula, std::shared_ptr<storm::logic::Formula>> formula(m, "Formula", "Generic Storm Formula");
    formula.def("__str__", &storm::logic::Formula::toString)
    ;

    // Path Formulae
    py::class_<storm::logic::PathFormula, std::shared_ptr<storm::logic::PathFormula>> pathFormula(m, "PathFormula", "Formula about the probability of a set of paths in an automaton", formula);
    py::class_<storm::logic::UnaryPathFormula, std::shared_ptr<storm::logic::UnaryPathFormula>> unaryPathFormula(m, "UnaryPathFormula", "Path formula with one operand", pathFormula);
    unaryPathFormula.def_property_readonly("subformula", &storm::logic::UnaryPathFormula::getSubformula, "the subformula");
    py::class_<storm::logic::EventuallyFormula, std::shared_ptr<storm::logic::EventuallyFormula>>(m, "EventuallyFormula", "Formula for eventually", unaryPathFormula);
    py::class_<storm::logic::GloballyFormula, std::shared_ptr<storm::logic::GloballyFormula>>(m, "GloballyFormula", "Formula for globally", unaryPathFormula);
    py::class_<storm::logic::BinaryPathFormula, std::shared_ptr<storm::logic::BinaryPathFormula>> binaryPathFormula(m, "BinaryPathFormula", "Path formula with two operands", pathFormula);
    py::class_<storm::logic::BoundedUntilFormula, std::shared_ptr<storm::logic::BoundedUntilFormula>>(m, "BoundedUntilFormula", "Until Formula with either a step or a time bound.", binaryPathFormula);
    py::class_<storm::logic::ConditionalFormula, std::shared_ptr<storm::logic::ConditionalFormula>>(m, "ConditionalFormula", "Formula with the right hand side being a condition.", formula);
    py::class_<storm::logic::UntilFormula, std::shared_ptr<storm::logic::UntilFormula>>(m, "UntilFormula", "Path Formula for unbounded until", binaryPathFormula);

    // Reward Path Formulae
    //py::class_<storm::logic::RewardPathFormula, std::shared_ptr<storm::logic::RewardPathFormula>(m, "RewardPathFormula", "Formula about the rewards of a set of paths in an automaton", py::base<storm::logic::Formula>());
    py::class_<storm::logic::CumulativeRewardFormula, std::shared_ptr<storm::logic::CumulativeRewardFormula>>(m, "CumulativeRewardFormula", "Summed rewards over a the paths", pathFormula);
    py::class_<storm::logic::InstantaneousRewardFormula, std::shared_ptr<storm::logic::InstantaneousRewardFormula>>(m ,"InstantaneousRewardFormula", "Instantaneous reward", pathFormula);
    py::class_<storm::logic::LongRunAverageRewardFormula, std::shared_ptr<storm::logic::LongRunAverageRewardFormula>>(m, "LongRunAverageRewardFormula", "Long run average reward", pathFormula);
    //py::class_<storm::logic::ReachabilityRewardFormula, std::shared_ptr<storm::logic::ReachabilityRewardFormula>>(m, "ReachabilityRewardFormula", "Reachability reward", py::base<storm::logic::RewardPathFormula>());


    // State Formulae
    py::class_<storm::logic::StateFormula, std::shared_ptr<storm::logic::StateFormula>> stateFormula(m, "StateFormula", "Formula about a state of an automaton", formula);
    py::class_<storm::logic::AtomicExpressionFormula, std::shared_ptr<storm::logic::AtomicExpressionFormula>>(m, "AtomicExpressionFormula", "Formula with an atomic expression", stateFormula);
    py::class_<storm::logic::AtomicLabelFormula, std::shared_ptr<storm::logic::AtomicLabelFormula>>(m, "AtomicLabelFormula", "Formula with an atomic label", stateFormula);
    py::class_<storm::logic::BooleanLiteralFormula, std::shared_ptr<storm::logic::BooleanLiteralFormula>>(m, "BooleanLiteralFormula", "Formula with a boolean literal", stateFormula)
            .def(py::init<bool>(),"truth value"_a);
    py::class_<storm::logic::UnaryStateFormula, std::shared_ptr<storm::logic::UnaryStateFormula>> unaryStateFormula(m, "UnaryStateFormula", "State formula with one operand", stateFormula);
    unaryStateFormula.def_property_readonly("subformula", &storm::logic::UnaryStateFormula::getSubformula, "the subformula");
    py::class_<storm::logic::UnaryBooleanStateFormula, std::shared_ptr<storm::logic::UnaryBooleanStateFormula>>(m, "UnaryBooleanStateFormula", "Unary boolean state formula", unaryStateFormula);
    py::class_<storm::logic::OperatorFormula, std::shared_ptr<storm::logic::OperatorFormula>> operatorFormula(m, "OperatorFormula", "Operator formula", unaryStateFormula);
    operatorFormula.def_property_readonly("has_bound", &storm::logic::OperatorFormula::hasBound, "Flag if formula is bounded")
        .def_property("comparison_type", &storm::logic::OperatorFormula::getComparisonType, &storm::logic::OperatorFormula::setComparisonType, "Comparison type of bound")
        .def_property_readonly("threshold", [](storm::logic::OperatorFormula const& f) {
            if (!f.getThreshold().hasRationalType()) {
                throw std::runtime_error("Can't get non-rational threshold (not implemented)");
            } else {
                return f.getThresholdAs<storm::RationalNumber>();
            }
        }, "Threshold of bound (currently only applicable to rational expressions)")
        .def("set_bound", [](storm::logic::OperatorFormula& f, storm::logic::ComparisonType comparisonType, storm::expressions::Expression const& bound) {
            f.setBound(storm::logic::Bound(comparisonType, bound));
        }, "Set bound", py::arg("comparison_type"), py::arg("bound"))
        // the above method should be sufficient for now; reinstate the following if needed
        //.def_property("_threshold_expression", &storm::logic::OperatorFormula::getThreshold, &storm::logic::OperatorFormula::setThreshold, "Threshold expression")
        //.def_property_readonly("_threshold_as_rational", &storm::logic::OperatorFormula::getThresholdAs<storm::RationalNumber>, "Rational threshold of bound, if applicable")
        //.def_property_readonly("_threshold_expression_has_rational_type", [](storm::logic::OperatorFormula const& f) { return f.getThreshold().hasRationalType(); } , "Check expression type [without needing a Python expression object]")
    ;
    py::class_<storm::logic::TimeOperatorFormula, std::shared_ptr<storm::logic::TimeOperatorFormula>>(m, "TimeOperator", "The time operator", operatorFormula);
    py::class_<storm::logic::LongRunAverageOperatorFormula, std::shared_ptr<storm::logic::LongRunAverageOperatorFormula>>(m, "LongRunAvarageOperator", "Long run average operator", operatorFormula);
    py::class_<storm::logic::ProbabilityOperatorFormula, std::shared_ptr<storm::logic::ProbabilityOperatorFormula>>(m, "ProbabilityOperator", "Probability operator", operatorFormula);
    py::class_<storm::logic::RewardOperatorFormula, std::shared_ptr<storm::logic::RewardOperatorFormula>>(m, "RewardOperator", "Reward operator", operatorFormula);
    py::class_<storm::logic::BinaryStateFormula, std::shared_ptr<storm::logic::BinaryStateFormula>> binaryStateFormula(m, "BinaryStateFormula", "State formula with two operands", stateFormula);
    py::class_<storm::logic::BinaryBooleanStateFormula, std::shared_ptr<storm::logic::BinaryBooleanStateFormula>>(m, "BooleanBinaryStateFormula", "Boolean binary state formula", binaryStateFormula);
}
