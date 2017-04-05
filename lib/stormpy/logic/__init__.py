from . import logic
from .logic import *


# this could be done in C++, but I don't know how to raise a Python error there
@property
def get_rational_threshold(self):
    if not self._threshold_expression_has_rational_type:  # wait, that's an expression, not formula??
        raise NotImplementedError("Can't get non-rational threshold (not implemented)")
    else:
        return self._threshold_as_rational

OperatorFormula.threshold = get_rational_threshold
