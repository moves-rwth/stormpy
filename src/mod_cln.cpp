#include "common.h"

#include "core/integer.h"
#include "core/rational.h"
#include "core/variable.h"
#include "core/monomial.h"
#include "core/term.h"
#include "core/polynomial.h"
#include "core/rationalfunction.h"
#include "core/factorizedpolynomial.h"
#include "core/factorizedrationalfunction.h"

#include "core/Interval.h"

PYBIND11_PLUGIN(cln) {
    py::module m("cln");

    define_cln_integer(m);
    define_cln_rational(m);
    define_term(m);
    define_polynomial(m);
    define_factorizationcache(m);
    define_factorizedpolynomial(m);
    define_rationalfunction(m);
    define_factorizedrationalfunction(m);

    define_interval(m);


    return m.ptr();
}
