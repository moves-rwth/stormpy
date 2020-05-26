#include "common.h"

#include "storage/bitvector.h"
#include "storage/model.h"
#include "storage/matrix.h"
#include "storage/distribution.h"
#include "storage/scheduler.h"
#include "storage/prism.h"
#include "storage/jani.h"
#include "storage/state.h"
#include "src/storage/valuation.h"
#include "storage/choiceorigins.h"
#include "storage/labeling.h"
#include "storage/expressions.h"

#include "storm/storage/dd/DdType.h"

PYBIND11_MODULE(storage, m) {
    m.doc() = "Data structures in Storm";

#ifdef STORMPY_DISABLE_SIGNATURE_DOC
    py::options options;
    options.disable_function_signatures();
#endif

    define_bitvector(m);
    define_model(m);
    define_statevaluation(m);
    define_sparse_model(m);
    define_sparse_matrix(m);
    define_symbolic_model<storm::dd::DdType::Sylvan>(m, "Sylvan");
    define_state(m);
    define_prism(m);
    define_jani(m);
    define_jani_transformers(m);
    define_labeling(m);
    define_origins(m);
    define_expressions(m);
    define_scheduler<double>(m, "Double");
    define_distribution<double>(m, "Double");
}
