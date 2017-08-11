from . import _config

if not _config.storm_with_pars:
    raise ImportError("No support for parametric analysis was built in Storm.")

from . import pars
from .pars import *

from stormpy import ModelType, StormError

pars._set_up()


def simplify_model(model, formula):
    """
    Simplify parametric model preserving the given formula by eliminating states with constant outgoing probabilities.
    :param model: Model.
    :param formula: Formula.
    :return: Tuple of simplified model and simplified formula.
    """
    if model.model_type == ModelType.DTMC:
        simplifier = pars._SparseParametricDtmcSimplifier(model)
    elif model.model_type == ModelType.MDP:
        simplifier = pars._SparseParametricMdpSimplifier(model)
    else:
        raise StormError("Model type {} cannot be simplified.".format(model.model_type))
    if not simplifier.simplify(formula):
        raise StormError("Model could not be simplified")
    return simplifier.simplified_model, simplifier.simplified_formula
