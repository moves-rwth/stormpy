#include "dft_state.h"
#include "src/helpers.h"
#include "storm-dft/storage/dft/DFTState.h"
#include "storm-dft/storage/dft/FailableElements.h"


template<typename ValueType> using DFTState = storm::storage::DFTState<ValueType>;
typedef storm::dft::storage::FailableElements Failable;
typedef storm::dft::storage::FailableElements::const_iterator FailableIter;

template<typename ValueType>
void define_dft_state(py::module& m, std::string const& vt_suffix) {

    // DFT state
    py::class_<DFTState<ValueType>, std::shared_ptr<DFTState<ValueType>>>(m, ("DFTState"+vt_suffix).c_str(), "DFT state")
        .def("failed", [](DFTState<ValueType> const& state, size_t id) {
                return state.hasFailed(id);
            }, "Is element failed", py::arg("id"))
        .def("failsafe", [](DFTState<ValueType> const& state, size_t id) {
                return state.isFailsafe(id);
            }, "Is element fail-safe", py::arg("id"))
        .def("dontcare", &DFTState<ValueType>::dontCare, "Is element Don't Care", py::arg("id"))
        .def("invalid", &DFTState<ValueType>::isInvalid, "Is state invalid")
        .def("failable", &DFTState<ValueType>::getFailableElements, "Get failable elements")
        .def("__str__", [](DFTState<ValueType> const& state) {
                return streamToString<storm::storage::BitVector>(state.status());
            })
        .def("to_string", [](std::shared_ptr<DFTState<ValueType>> const& state, storm::storage::DFT<ValueType> const& dft) {
                return dft.getStateString(state);
            }, "Print status", py::arg("dft"))
    ;
}


void define_failable_elements(py::module& m) {

    // Helper iterator for access from python
    struct FailableIterator {
        FailableIterator(Failable const &failable, py::object ref) : failable(failable), ref(ref) { }

        FailableIter next() {
            ++it;
            if (it == failable.end())
                throw py::stop_iteration();
            else
                return it;
        }

        Failable const&failable;
        py::object ref; // keep a reference
        FailableIter it = failable.begin();
    };


    py::class_<Failable, std::shared_ptr<Failable>>(m, "FailableElements", "Failable elements in DFT state")
        .def("__iter__", [](py::object s) { return FailableIterator(s.cast<Failable const&>(), s); })
    ;

    py::class_<FailableIterator>(m, "FailableIterator")
        .def("__iter__", [](FailableIterator &it) -> FailableIterator& { return it; })
        .def("__next__", &FailableIterator::next)
    ;

    py::class_<FailableIter, std::shared_ptr<FailableIter>>(m, "FailableElement", "Failable element")
        .def("due_dependency", &FailableIter::isFailureDueToDependency, "Is failure due to dependency")
        .def("get_fail_be_double", &FailableIter::getFailBE<double>, py::arg("dft"), "Get BE which fails")
        .def("get_fail_be_ratfunc", &FailableIter::getFailBE<storm::RationalFunction>, py::arg("dft"), "Get BE which fails")
    ;

}


template void define_dft_state<double>(py::module& m, std::string const& vt_suffix);
template void define_dft_state<storm::RationalFunction>(py::module& m, std::string const& vt_suffix);
