from . import pomdp
from .pomdp import *

def make_canonic(model):
    return pomdp._make_canonic_Double(model)