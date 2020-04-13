import stormpy.core

class Simulator:
    """
    Base class for simulators.
    """
    def __init__(self, seed=None):
        self._seed = seed

    def step(self, action=None):
        raise NotImplementedError("Abstract superclass")

    def restart(self):
        raise NotImplementedError("Abstract superclass")


class SparseSimulator(Simulator):
    """
    Simulator on top of sparse models.
    """

    def __init__(self, model, seed=None):
        super().__init__(seed)
        self._model = model
        self._engine = stormpy.core._DiscreteTimeSparseModelSimulatorDouble(model)
        if seed is not None:
            self._engine.set_seed(seed)

    def step(self, action=None):
        if action is None:
            if self._model.is_nondeterministic_model:
                raise RuntimeError("Must specify an action in nondeterministic models")
            check = self._engine.step(0)
            assert(check)
        else:
            if action >= self._model.get_nondeterministic_choices():
                raise RuntimeError(f"Only {self._model.get_nondeterministic_choices()} actions available")
            check = self._engine.step(action)
            assert(check)
        return self._engine.get_current_state()

    def restart(self):
        self._engine.reset_to_initial_state()


def create_simulator(model, seed = None):
    """
    Factory method for creating a simulator.

    :param model: Some form of model
    :param seed: A seed for reproducibility. If None (default), the seed is internally generated.
    :return: A simulator that can simulate on top of this model
    """
    if isinstance(model, stormpy.storage._ModelBase):
        if model.is_sparse_model:
            return SparseSimulator(model, seed)
    else:
        raise NotImplementedError("Currently, we only support simulators for sparse models.")
