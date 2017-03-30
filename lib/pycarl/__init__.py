from . import core
from .core import *

__version__ = "unknown"
try:
    from _version import __version__
except ImportError:
    # We're running in a tree that doesn't have a _version.py, so we don't know what our version is.
    pass


def numerator(x):
    if type(x) == core.RationalFunction or type(x) == core.Rational or type(x) == core.FactorizedRationalFunction:
        return x.numerator
    else:
        return x

def denominator(x):
    if type(x) == core.RationalFunction or type(x) == core.Rational or type(x) == core.FactorizedRationalFunction:
        return x.denominator
    else:
        return 1



