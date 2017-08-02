from . import storage
from .storage import *


class ModelInstantiator:
    """
    Class for instantiating models.
    """

    def __init__(self, model):
        """
        Constructor.
        :param model: Model.
        """
        if model.model_type == ModelType.MDP:
            self._instantiator = PmdpInstantiator(model)
        elif model.model_type == ModelType.DTMC:
            self._instantiator = PdtmcInstantiator(model)
        elif model.model_type == ModelType.CTMC:
            self._instantiator = PctmcInstantiator(model)
        elif model.model_type == ModelType.MA:
            self._instantiator = PmaInstantiator(model)
        else:
            raise ValueError("Model type {} not supported".format(model.model_type))

    def instantiate(self, valuation):
        """
        Instantiate model with given valuation.
        :param valuation: Valuation from parameter to value.
        :return: Instantiated model.
        """
        return self._instantiator.instantiate(valuation)
