#include "common.h"
#include "helpers.h"

#include "typed_formula/constraint.h"
#include "typed_formula/formula.h"

PYBIND11_PLUGIN(formula) {
	py::module m("formula", "pycarl formula handling");

	// Constraint relies on Rational
	m.import("pycarl.core");
    m.import("pycarl.formula");

	define_constraint(m);
	define_simple_constraint(m);
	define_formula(m);

	return m.ptr();
}
