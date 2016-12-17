#include "shortestPaths.h"
#include "storm/utility/shortestPaths.h"

#include <sstream>
#include <string>

#include <boost/optional/optional_io.hpp>

void define_ksp(py::module& m) {

    // long types shortened for readability
    using Path                   = storm::utility::ksp::Path<double>;
    using state_t                = storm::utility::ksp::state_t;
    using ShortestPathsGenerator = storm::utility::ksp::ShortestPathsGenerator<double>;
    using Model                  = storm::models::sparse::Model<double>;
    using BitVector              = storm::storage::BitVector;

    py::class_<Path>(m, "Path")
        // overload constructor rather than dealing with boost::optional
        .def("__init__", [](Path &instance, state_t preNode, unsigned long preK, double distance) {
                new (&instance) Path { boost::optional<state_t>(preNode), preK, distance };
            }, "predecessorNode"_a, "predecessorK"_a, "distance"_a)

        .def("__init__", [](Path &instance, unsigned long preK, double distance) {
                new (&instance) Path { boost::none, preK, distance };
            }, "predecessorK"_a, "distance"_a)

        .def(py::self == py::self, "Compares predecessor node and index, ignoring distance")

        .def("__repr__", [](Path const& p) {
                std::ostringstream oss;
                oss << "<Path with predecessorNode: '" << ((p.predecessorNode) ? std::to_string(p.predecessorNode.get()) : "None");
                oss << "' predecessorK: '" << p.predecessorK << "' distance: '" << p.distance << "'>";
                return oss.str();
            })

        .def_readwrite("predecessorNode", &Path::predecessorNode) // TODO (un-)wrap boost::optional so it's usable
        .def_readwrite("predecessorK", &Path::predecessorK)
        .def_readwrite("distance", &Path::distance)
    ;

    py::class_<ShortestPathsGenerator>(m, "ShortestPathsGenerator")
        .def(py::init<Model const&, BitVector>(), "model"_a, "target_bitvector"_a)
        .def(py::init<Model const&, state_t>(), "model"_a, "target_state"_a)
        .def(py::init<Model const&, std::vector<state_t> const&>(), "model"_a, "target_state_list"_a)
        .def(py::init<Model const&, std::string>(), "model"_a, "target_label"_a)
    ;
}