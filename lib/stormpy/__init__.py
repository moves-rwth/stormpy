from . import core
from .core import *
from . import storage
from .storage import *
from ._config import *
from .logic import *
from .exceptions import *

from pycarl import Variable  # needed for building parametric models

__version__ = "unknown"
try:
    from _version import __version__
except ImportError:
    # We're running in a tree that doesn't have a _version.py, so we don't know what our version is.
    pass

core._set_up("")


def build_model(symbolic_description, properties=None):
    """
    Build a model from a symbolic description.

    :param symbolic_description: Symbolic model description to translate into a model.
    :param List[Property] properties: List of properties that should be preserved during the translation. If None, then all properties are preserved.
    :return: Model in sparse representation.
    :rtype: SparseDtmc or SparseMdp
    """
    if not symbolic_description.undefined_constants_are_graph_preserving:
        raise StormError("Program still contains undefined constants")

    if properties:
        formulae = [prop.raw_formula for prop in properties]
        intermediate = core._build_sparse_model_from_prism_program(symbolic_description, formulae)
    else:
        intermediate = core._build_sparse_model_from_prism_program(symbolic_description)
    assert not intermediate.supports_parameters
    if intermediate.model_type == ModelType.DTMC:
        return intermediate._as_dtmc()
    elif intermediate.model_type == ModelType.MDP:
        return intermediate._as_mdp()
    elif intermediate.model_type == ModelType.CTMC:
        return intermediate._as_ctmc()
    elif intermediate.model_type == ModelType.MA:
        return intermediate._as_ma()
    else:
        raise StormError("Not supported non-parametric model constructed")


def build_parametric_model(symbolic_description, properties=None):
    """
    Build a parametric model from a symbolic description.
    
    :param symbolic_description: Symbolic model description to translate into a model.
    :param List[Property] properties: List of properties that should be preserved during the translation. If None, then all properties are preserved.
    :return: Parametric model in sparse representation.
    :rtype: SparseParametricDtmc or SparseParametricMdp
    """
    if not symbolic_description.undefined_constants_are_graph_preserving:
        raise StormError("Program still contains undefined constants")

    if properties:
        formulae = [prop.raw_formula for prop in properties]
    else:
        formulae = []
    intermediate = core._build_sparse_parametric_model_from_prism_program(symbolic_description, formulae)
    assert intermediate.supports_parameters
    if intermediate.model_type == ModelType.DTMC:
        return intermediate._as_pdtmc()
    elif intermediate.model_type == ModelType.MDP:
        return intermediate._as_pmdp()
    elif intermediate.model_type == ModelType.CTMC:
        return intermediate._as_pctmc()
    elif intermediate.model_type == ModelType.MA:
        return intermediate._as_pma()
    else:
        raise StormError("Not supported parametric model constructed")


def build_model_from_drn(file):
    """
    Build a model from the explicit DRN representation.

    :param String file: DRN file containing the model.
    :return: Model in sparse representation.
    :rtype: SparseDtmc or SparseMdp or SparseCTMC or SparseMA
    """
    intermediate = core._build_sparse_model_from_drn(file)
    assert not intermediate.supports_parameters
    if intermediate.model_type == ModelType.DTMC:
        return intermediate._as_dtmc()
    elif intermediate.model_type == ModelType.MDP:
        return intermediate._as_mdp()
    elif intermediate.model_type == ModelType.CTMC:
        return intermediate._as_ctmc()
    elif intermediate.model_type == ModelType.MA:
        return intermediate._as_ma()
    else:
        raise StormError("Not supported non-parametric model constructed")


def build_parametric_model_from_drn(file):
    """
    Build a parametric model from the explicit DRN representation.

    :param String file: DRN file containing the model.
    :return: Parametric model in sparse representation.
    :rtype: SparseParametricDtmc or SparseParametricMdp or SparseParametricCTMC or SparseParametricMA
    """
    intermediate = core._build_sparse_parametric_model_from_drn(file)
    assert intermediate.supports_parameters
    if intermediate.model_type == ModelType.DTMC:
        return intermediate._as_pdtmc()
    elif intermediate.model_type == ModelType.MDP:
        return intermediate._as_pmdp()
    elif intermediate.model_type == ModelType.CTMC:
        return intermediate._as_pctmc()
    elif intermediate.model_type == ModelType.MA:
        return intermediate._as_pma()
    else:
        raise StormError("Not supported parametric model constructed")


def perform_bisimulation(model, properties, bisimulation_type):
    """
    Perform bisimulation on model.
    :param model: Model.
    :param properties: Properties to preserve during bisimulation.
    :param bisimulation_type: Type of bisimulation (weak or strong).
    :return: Model after bisimulation.
    """
    formulae = [prop.raw_formula for prop in properties]
    if model.supports_parameters:
        return core._perform_parametric_bisimulation(model, formulae, bisimulation_type)
    else:
        return core._perform_bisimulation(model, formulae, bisimulation_type)


def model_checking(model, property, only_initial_states=False):
    """
    Perform model checking on model for property.
    :param model: Model.
    :param property: Property to check for.
    :param only_initial_states: If True, only results for initial states are computed.
                                If False, results for all states are computed.
    :return: Model checking result.
    :rtype: CheckResult
    """
    if isinstance(property, Property):
        formula = property.raw_formula
    else:
        formula = property

    if model.supports_parameters:
        task = core.ParametricCheckTask(formula, only_initial_states)
        return core._parametric_model_checking_sparse_engine(model, task)
    else:
        task = core.CheckTask(formula, only_initial_states)
        return core._model_checking_sparse_engine(model, task)


def prob01min_states(model, eventually_formula):
    assert type(eventually_formula) == logic.EventuallyFormula
    labelform = eventually_formula.subformula
    labelprop = core.Property("label-prop", labelform)
    phiStates = BitVector(model.nr_states, True)
    psiStates = model_checking(model, labelprop).get_truth_values()
    return compute_prob01min_states(model, phiStates, psiStates)


def prob01max_states(model, eventually_formula):
    assert type(eventually_formula) == logic.EventuallyFormula
    labelform = eventually_formula.subformula
    labelprop = core.Property("label-prop", labelform)
    phiStates = BitVector(model.nr_states, True)
    psiStates = model_checking(model, labelprop).get_truth_values()
    return compute_prob01min_states(model, phiStates, psiStates)


def compute_prob01_states(model, phi_states, psi_states):
    """
    Compute prob01 states for properties of the form phi_states until psi_states

    :param SparseDTMC model:
    :param BitVector phi_states:
    :param BitVector psi_states: Target states
    """
    if model.model_type != ModelType.DTMC:
        raise StormError("Prob 01 is only defined for DTMCs -- model must be a DTMC")

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
