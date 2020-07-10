from . import pomdp
from .pomdp import *

def make_canonic(model):
    if model.supports_parameters:
        return pomdp._make_canonic_Rf(model)
    else:
        return pomdp._make_canonic_Double(model)