#include "common.h"

#include "storage/bitvector.h"
#include "storage/model.h"
#include "storage/matrix.h"
#include "storage/prism.h"
#include "storage/state.h"
#include "storage/labeling.h"
#include "storage/expressions.h"

PYBIND11_MODULE(storage, m) {
    m.doc() = "Data structures in Storm";

#ifdef STORMPY_DISABLE_SIGNATURE_DOC
    py::options options;
    options.disable_function_signatures();
#endif

    define_bitvector(m);
    define_model(m);
    define_sparse_matrix(m);
    define_state(m);
    define_prism(m);
    define_labeling(m);
    define_expressions(m);
}
