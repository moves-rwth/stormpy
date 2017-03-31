#ifndef PYTHON_CORE_RESULT_H_
#define PYTHON_CORE_RESULT_H_

#include "common.h"

// Class holding the parametric model checking result
class PmcResult {
    private:
        std::shared_ptr<storm::modelchecker::CheckResult> checkResult;
        std::unordered_set<storm::ArithConstraint<storm::RationalFunction>> constraintsWellFormed;
        std::unordered_set<storm::ArithConstraint<storm::RationalFunction>> constraintsGraphPreserving;

    public:
        PmcResult(std::shared_ptr<storm::modelchecker::CheckResult> _checkResult) : checkResult(_checkResult) {
        }

        std::shared_ptr<storm::modelchecker::CheckResult> getResult() {
            return checkResult;
        }

        std::unordered_set<storm::ArithConstraint<storm::RationalFunction>> getConstraintsWellFormed() const {
            return constraintsWellFormed;
        }

        void setConstraintsWellFormed(std::unordered_set<storm::ArithConstraint<storm::RationalFunction>> _constraintsWellFormed) {
            this->constraintsWellFormed = _constraintsWellFormed;
        }

        std::unordered_set<storm::ArithConstraint<storm::RationalFunction>> getConstraintsGraphPreserving() const {
            return constraintsGraphPreserving;
        }
        
        void setConstraintsGraphPreserving(std::unordered_set<storm::ArithConstraint<storm::RationalFunction>> _constraintsGraphPreserving) {
            this->constraintsGraphPreserving = _constraintsGraphPreserving;
        }

        std::string toString() {
            std::stringstream stream;
            stream << *checkResult << std::endl;
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
