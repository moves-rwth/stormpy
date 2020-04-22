
#include "common.h"

#include "pomdp/tracker.h"
#include "pomdp/qualitative_analysis.h"
#include "pomdp/transformations.h"

PYBIND11_MODULE(pomdp, m) {
    m.doc() = "Functionality for POMDP analysis";

#ifdef STORMPY_DISABLE_SIGNATURE_DOC
    py::options options;
    options.disable_function_signatures();
#endif
    define_tracker(m);
    define_qualitative_policy_search<double>(m, "Double");
    define_qualitative_policy_search_nt(m);
    define_transformations<double>(m, "Double");
}
