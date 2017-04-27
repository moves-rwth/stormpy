from . import gmp
from .gmp import *



def numerator(x):
    if type(x) == gmp.RationalFunction or  type(x) == gmp.Rational or type(x) == gmp.FactorizedRationalFunction:
        return x.numerator
    else:
        return x

def denominator(x):
    if type(x) == gmp.RationalFunction or type(x) == gmp.Rational or type(x) == gmp.FactorizedRationalFunction:
        return x.denominator
    else:
        return 1

