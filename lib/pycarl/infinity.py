
import math

class Infinity:
    def __init__(self, negated = False):
        self.negated = negated

    def __neg__(self):
        return Infinity(not self.negated)

    def __le__(self, other):
        if isinstance(other, Infinity) and (not other.negated or other.negated == self.negated): return True
        return self.negated

    def __ge__(self, other):
        if isinstance(other, Infinity) and (other.negated or other.negated == self.negated): return True
        return not self.negated

    def __lt__(self, other):
        if isinstance(other, Infinity) and other.negated: return False
        return self.negated

    def __gt__(self, other):
        if isinstance(other, Infinity) and not other.negated: return False
        return not self.negated


    def __eq__(self, other):
        if sys.version_info[1] >= 5:
            return (isinstance(other, Infinity) and other.negated == self.negated) or (not self.negated and other == math.inf)
        else:
            return (isinstance(other, Infinity) and other.negated == self.negated)


    def __str__(self):
        return "-infty" if self.negated else "infty"

    def __repr__(self):
        return "-pycarl.inf" if self.negated else "pycarl.inf"

