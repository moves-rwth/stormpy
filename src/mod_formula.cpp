#include "common.h"
#include "helpers.h"

#include "formula/constraint.h"
#include "formula/formula.h"

PYBIND11_PLUGIN(formula) {
	py::module m("formula", "pycarl formula handling");

	// Constraint relies on Rational
	m.import("pycarl.core");

	define_relation(m);
	define_constraint(m);
	define_simple_constraint(m);
	define_formula_type(m);
	define_formula(m);

	return m.ptr();
}
