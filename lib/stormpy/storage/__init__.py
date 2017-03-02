from . import storage
from .storage import *
from . import state,action

class ModelInstantiator:
    def __init__(self, model):
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

    def instantiate(self, point):
        return self._instantiator.instantiate(point)
