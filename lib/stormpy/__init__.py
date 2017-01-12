from . import core
from .core import *
from . import storage
from .storage import *
from .version import __version__

core._set_up("")


def build_model(program, properties = None):
    if properties:
        formulae = [prop.raw_formula for prop in properties]
    else:
        formulae = []
    intermediate = core._build_sparse_model_from_prism_program(program, formulae)
    assert not intermediate.supports_parameters
    if intermediate.model_type == ModelType.DTMC:
        return intermediate._as_dtmc()
    elif intermediate.model_type == ModelType.MDP:
        return intermediate._as_mdp()
    else:
        raise RuntimeError("Not supported non-parametric model constructed")

def build_parametric_model(program, properties = None):
    if properties:
        formulae = [prop.raw_formula for prop in properties]
    else:
        formulae = []
    intermediate = core._build_sparse_parametric_model_from_prism_program(program, formulae)
    assert intermediate.supports_parameters
    if intermediate.model_type == ModelType.DTMC:
        return intermediate._as_pdtmc()
    elif intermediate.model_type == ModelType.MDP:
        return intermediate._as_pmdp()
    else:
        raise RuntimeError("Not supported parametric model constructed")

def perform_bisimulation(model, property, bisimulation_type):
    if model.supports_parameters:
        return core._perform_parametric_bisimulation(model, property.raw_formula, bisimulation_type)
    else:
        return core._perform_bisimulation(model, property.raw_formula, bisimulation_type)

def model_checking(model, property):
    if model.supports_parameters:
        return core._parametric_model_checking(model, property.raw_formula)
    else:
        return core._model_checking(model, property.raw_formula)
