from enum import Enum

import stormpy.core


class SimulatorObservationMode(Enum):
    STATE_LEVEL = 0,
    PROGRAM_LEVEL = 1

class Simulator:
    """
    Base class for simulators.
    """
    def __init__(self, seed=None):
        self._seed = seed
        self._observation_mode = SimulatorObservationMode.STATE_LEVEL

    def step(self, action=None):
        raise NotImplementedError("Abstract superclass")

    def restart(self):
        raise NotImplementedError("Abstract superclass")

    def is_done(self):
        return False

    def set_observation_mode(self, mode):
        if not isinstance(mode, SimulatorObservationMode):
            raise RuntimeError("Observation mode must be a SimulatorObservationMode")
        self._observation_mode = mode


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
        self._state_valuations = None

    def step(self, action=None):
        if action is None:
            if self._model.is_nondeterministic_model:
                raise RuntimeError("Must specify an action in nondeterministic models")
            check = self._engine.step(0)
            assert check
        else:
            if action >= self._model.get_nondeterministic_choices():
                raise RuntimeError(f"Only {self._model.get_nondeterministic_choices()} actions available")
            check = self._engine.step(action)
            assert check
        if self._observation_mode == SimulatorObservationMode.STATE_LEVEL:
            return self._engine.get_current_state()
        elif self._observation_mode == SimulatorObservationMode.PROGRAM_LEVEL:
            return self._state_valuations.get_state(self._engine.get_current_state())

        assert False, "Observation Mode not recognised."

    def restart(self):
        self._engine.reset_to_initial_state()

    def is_done(self):
        return self._model.is_sink_state(self._engine.get_current_state())

    def set_observation_mode(self, mode):
        super().set_observation_mode(mode)
        if self._observation_mode == SimulatorObservationMode.PROGRAM_LEVEL:
            if not self._model.has_state_valuations():
                raise RuntimeError("Program level observations require model with state valuations")
        self._state_valuations = self._model.state_valuations



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
