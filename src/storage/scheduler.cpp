#include "scheduler.h"
#include "src/helpers.h"

#include "storm/storage/Scheduler.h"

void define_scheduler(py::module& m) {
    using Scheduler = storm::storage::Scheduler<double>;

    py::class_<Scheduler, std::shared_ptr<storm::storage::Scheduler<double>>> scheduler(m, "Scheduler", "A Finite Memory Scheduler");
    scheduler
            .def("__str__", [](Scheduler const& s) {
                std::stringstream str;
                s.printToStream(str);
                return str.str();
            })
    ;

}