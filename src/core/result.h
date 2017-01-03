#ifndef PYTHON_CORE_RESULT_H_
#define PYTHON_CORE_RESULT_H_

#include "common.h"

// Class holding the parametric model checking result
class PmcResult {
    public:
        storm::RationalFunction resultFunction;
        std::unordered_set<storm::ArithConstraint<storm::RationalFunction>> constraintsWellFormed;
        std::unordered_set<storm::ArithConstraint<storm::RationalFunction>> constraintsGraphPreserving;

        storm::RationalFunction getResultFunction() const {
            return resultFunction;
        }

        std::unordered_set<storm::ArithConstraint<storm::RationalFunction>> getConstraintsWellFormed() const {
            return constraintsWellFormed;
        }

        std::unordered_set<storm::ArithConstraint<storm::RationalFunction>> getConstraintsGraphPreserving() const {
            return constraintsGraphPreserving;
        }

        std::string toString() {
            std::stringstream stream;
            stream << resultFunction << std::endl;
            stream << "Well formed constraints:" << std::endl;
            for (auto constraint : constraintsWellFormed) {
                stream << constraint << std::endl;
            }
            stream << "Graph preserving constraints:" << std::endl;
            for (auto constraint : constraintsGraphPreserving) {
                stream << constraint << std::endl;
            }
            return stream.str();
        }
};

void define_result(py::module& m);

#endif /* PYTHON_CORE_RESULT_H_ */
