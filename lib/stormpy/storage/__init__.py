from . import storage
from .storage import *
from . import state,action

class ModelInstantiator:
    def __init__(self, model):
        if model.model_type == ModelType.MDP:
            self._instantiator = PmdpInstantiator(model)
        elif model.model_type == ModelType.DTMC:
            self._instantiator = PdtmcInstantiator(model)
        else:
            raise ValueError("Only DTMCs and MDPs supported")

    def instantiate(self, point):
        return self._instantiator.instantiate(point)