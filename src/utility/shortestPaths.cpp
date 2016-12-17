#include "shortestPaths.h"
#include "storm/utility/shortestPaths.h"

#include <string>


void define_ksp(py::module& m) {

    using Path = storm::utility::ksp::Path<double>;
    using state_t = storm::utility::ksp::state_t;

    // (k-shortest) Path
    py::class_<Path>(m, "Path")
        // Fuck all this. FIXME

        //.def(py::init<boost::optional<state_t>, unsigned long, double>()) // does not work (because it's an aggregate initialized struct?)

        // this may or may not be working (i.e., initializing with the values as expected)
        // https://pybind11.readthedocs.io/en/latest/advanced/classes.html#custom-constructors
        .def("__init__", [](Path &instance, state_t preNode, unsigned long preK, double distance) {
                new (&instance) Path { boost::optional<state_t>(preNode), preK, distance };
            }, "predecessorNode"_a, "predecessorK"_a, "distance"_a)

        .def("__init__", [](Path &instance, unsigned long preK, double distance) {
                new (&instance) Path { boost::none, preK, distance };
            }, "predecessorK"_a, "distance"_a)

        // this actually seemed to work, once!, but not anymore
        .def(py::self == py::self)

        .def("__repr__",
            [](Path const& a) {
                // shit this is ugly. but it's a proof of concept -- at least it works
                std::string str = "<Path with";
                if (a.predecessorNode) {
                    str += " predecessorNode: " + std::to_string(a.predecessorNode.get());
                }
                str += " predecessorK " + std::to_string(a.predecessorK);
                str += " distance " + std::to_string(a.distance);
                str += ">";
                return str;
            })

        .def("debug_distance",
            [](Path const& a) {
                return a.distance; // yeah, that's it, that's all i want you to do you lazy fuck. it's just a double. jesus
            })

        .def_readwrite("predecessorNode", &Path::predecessorNode) // does not work (again due to struct??) FIXME
        .def_readwrite("predecessorK", &Path::predecessorK)
        .def_readwrite("distance", &Path::distance)
        ;

    // TODO continue
    /*
    // ShortestPathsGenerator
    py::class_<storm::utility::ksp::ShortestPathsGenerator<double>>(m, "ShortestPathsGenerator")
            ;
    */
}