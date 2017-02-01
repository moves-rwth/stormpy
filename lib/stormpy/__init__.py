from . import core
from .core import *
from . import storage
from .storage import *
from .version import __version__
import stormpy.logic

core._set_up("")


def build_model(program, properties = None):
    """
    Build a model from a symbolic description

    :param PrismProgram program: Prism program to translate into a model.
    :param List[Property] properties: List of properties that should be preserved during the translation. If None, then all properties are preserved.
    """
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
    """
    
    :param PrismProgram program: Prism program with open constants to translate into a parametric model.
    :param List[Property] properties: List of properties that should be preserved during the translation. If None, then all properties are preserved.
    """
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


def compute_prob01_states(model, phi_states, psi_states):
    """
    Compute prob01 states for properties of the form phi_states until psi_states

    :param SparseDTMC model:
    :param BitVector phi_states:
    :param BitVector psi_states:
    """
    if model.model_type != ModelType.DTMC:
        raise ValueError("Prob 01 is only defined for DTMCs -- model must be a DTMC")

    if model.supports_parameters:
        return core._compute_prob01states_rationalfunc(model, phi_states, psi_states)
    else:
        return core._compute_prob01states_double(model, phi_states, psi_states)

def compute_prob01min_states(model, phi_states, psi_states):
    if model.model_type == ModelType.DTMC:
        return compute_prob01_states(model, phi_states, psi_states)
    if model.supports_parameters:
        return core._compute_prob01states_min_rationalfunc(model, phi_states, psi_states)
    else:
        return core._compute_prob01states_min_double(model, phi_states, psi_states)

def compute_prob01max_states(model, phi_states, psi_states):
    if model.model_type == ModelType.DTMC:
        return compute_prob01_states(model, phi_states, psi_states)
    if model.supports_parameters:
        return core._compute_prob01states_max_rationalfunc(model, phi_states, psi_states)
    else:
        return core._compute_prob01states_max_double(model, phi_states, psi_states)
