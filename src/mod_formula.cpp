#include "common.h"
#include "helpers.h"

#include "formula/formula_type.h"
#include "formula/relation.h"

PYBIND11_PLUGIN(formula) {
	py::module m("formula", "pycarl formula handling");

	// Constraint relies on Rational
	m.import("pycarl.core");

	define_relation(m);
	define_formula_type(m);

	return m.ptr();
}
