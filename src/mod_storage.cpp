#include "common.h"

#include "storage/bitvector.h"
#include "storage/model.h"
#include "storage/matrix.h"
#include "storage/state.h"
#include "storage/labeling.h"

PYBIND11_PLUGIN(storage) {
    py::module m("storage");
    
#ifdef STORMPY_DISABLE_SIGNATURE_DOC
    py::options options;
    options.disable_function_signatures();
#endif
    
    define_bitvector(m);
    define_model(m);
    define_sparse_matrix(m);
    define_state(m);
    define_labeling(m);
    return m.ptr();
}
