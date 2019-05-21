#include "expressions.h"
#include "src/helpers.h"

#include "storm/storage/expressions/ExpressionManager.h"
#include "storm/storage/expressions/Expression.h"
#include "storm-parsers/parser/ExpressionParser.h"

//Define python bindings
void define_expressions(py::module& m) {
    using Expression = storm::expressions::Expression;
    using Variable = storm::expressions::Variable;



    // ExpressionManager
    py::class_<storm::expressions::ExpressionManager, std::shared_ptr<storm::expressions::ExpressionManager>>(m, "ExpressionManager", "Manages variables for expressions")
        .def(py::init(), "Constructor")
        .def("create_boolean", &storm::expressions::ExpressionManager::boolean, py::arg("boolean"), "Create expression from boolean")
        .def("create_integer", &storm::expressions::ExpressionManager::integer, py::arg("integer"), "Create expression from integer number")
        .def("create_rational", [](storm::expressions::ExpressionManager const& manager, storm::RationalNumber number) {
                return manager.rational(number);
            }, py::arg("rational"), "Create expression from rational number")
        .def("create_boolean_variable", &storm::expressions::ExpressionManager::declareBooleanVariable, "create Boolean variable", py::arg("name"), py::arg("auxiliary") = false)
        .def("create_integer_variable", &storm::expressions::ExpressionManager::declareIntegerVariable, "create Integer variable", py::arg("name"), py::arg("auxiliary") = false)
        .def("create_rational_variable", &storm::expressions::ExpressionManager::declareRationalVariable, "create Rational variable", py::arg("name"), py::arg("auxiliary") = false)

            ;

    // Variable
    py::class_<storm::expressions::Variable, std::shared_ptr<storm::expressions::Variable>>(m, "Variable", "Represents a variable")
        .def_property_readonly("name", &storm::expressions::Variable::getName, "Variable name")
        .def("has_boolean_type", &storm::expressions::Variable::hasBooleanType, "Check if the variable is of boolean type")
        .def("has_integer_type", &storm::expressions::Variable::hasIntegerType, "Check if the variable is of integer type")
        .def("has_rational_type", &storm::expressions::Variable::hasRationalType, "Check if the variable is of rational type")
        .def("has_numerical_type", &storm::expressions::Variable::hasNumericalType, "Check if the variable is of numerical type")
        .def("has_bitvector_type", &storm::expressions::Variable::hasBitVectorType, "Check if the variable is of bitvector type")
        .def("get_expression", &storm::expressions::Variable::getExpression, "Get expression from variable")
    ;

    // Expression
    py::class_<storm::expressions::Expression, std::shared_ptr<storm::expressions::Expression>>(m, "Expression", "Holds an expression")
        .def(py::init<Expression>(), "other_expression"_a)
        .def("contains_variables", &storm::expressions::Expression::containsVariables, "Check if the expression contains variables.")
        .def("contains_variable", &storm::expressions::Expression::containsVariable, "Check if the expression contains any of the given variables.", py::arg("variables"))
        .def("get_variables" , &storm::expressions::Expression::getVariables, "Get the variables")
            .def("is_literal", &storm::expressions::Expression::isLiteral, "Check if the expression is a literal")
        .def("has_boolean_type", &storm::expressions::Expression::hasBooleanType, "Check if the expression is a boolean")
        .def("has_integer_type", &storm::expressions::Expression::hasIntegerType, "Check if the expression is an integer")
        .def("has_rational_type", &storm::expressions::Expression::hasRationalType, "Check if the expression is a rational")
        .def_property_readonly("type", &storm::expressions::Expression::getType, "Get the Type")
        .def_property_readonly("manager", &storm::expressions::Expression::getManager, "Get the manager")
        .def("substitute", [](Expression const& expr, std::map<Variable, Expression> const& map) { return expr.substitute(map);}, "substitution_map"_a)
        .def("__str__", &storm::expressions::Expression::toString, "To string")

        .def_static("Plus", [](Expression const& lhs, Expression const& rhs) {return lhs + rhs;})
        .def_static("Minus", [](Expression const& lhs, Expression const& rhs) {return lhs - rhs;})
        .def_static("Multiply", [](Expression const& lhs, Expression const& rhs) {return lhs * rhs;})
        .def_static("And", [](Expression const& lhs, Expression const& rhs) {return lhs && rhs;})
        .def_static("Or", [](Expression const& lhs, Expression const& rhs) {return lhs || rhs;})
        .def_static("Geq", [](Expression const& lhs, Expression const& rhs) {return lhs >= rhs;})
        .def_static("Eq", [](Expression const& lhs, Expression const& rhs) {return lhs == rhs;})
        .def_static("Neq", [](Expression const& lhs, Expression const& rhs) {return lhs != rhs;})
        .def_static("Greater", [](Expression const& lhs, Expression const& rhs) {return lhs > rhs;})
        .def_static("Less", [](Expression const& lhs, Expression const& rhs) {return lhs < rhs;})
        .def_static("Leq", [](Expression const& lhs, Expression const& rhs) {return lhs <= rhs;})
        .def_static("Implies", [](Expression const& lhs, Expression const& rhs) {return storm::expressions::implies(lhs, rhs);})
        .def_static("Iff", [](Expression const& lhs, Expression const& rhs) {return storm::expressions::iff(lhs, rhs);})
            ;

    py::class_<storm::parser::ExpressionParser>(m, "ExpressionParser", "Parser for storm-expressions")
            .def(py::init<storm::expressions::ExpressionManager const&>(), "Expression Manager to use", py::arg("expression_manager"))
            .def("set_identifier_mapping", [](storm::parser::ExpressionParser& p, std::unordered_map<std::string, storm::expressions::Expression> const& identifierMapping) {p.setIdentifierMapping(identifierMapping);}, "sets identifiers")
            .def("parse", &storm::parser::ExpressionParser::parseFromString, "parse")
            ;


    py::class_<storm::expressions::Type>(m, "ExpressionType", "The type of an expression")
            .def_property_readonly("is_boolean", &storm::expressions::Type::isBooleanType)
            .def_property_readonly("is_integer", &storm::expressions::Type::isIntegerType)
            .def_property_readonly("is_rational", &storm::expressions::Type::isRationalType)
            .def("__str__", &storm::expressions::Type::getStringRepresentation);

}