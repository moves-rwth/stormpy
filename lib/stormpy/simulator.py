from enum import Enum

import stormpy.core


class SimulatorObservationMode(Enum):
    STATE_LEVEL = 0,
    PROGRAM_LEVEL = 1

class SimulatorActionMode(Enum):
    INDEX_LEVEL = 0

class Simulator:
    """
    Base class for simulators.
    """
    def __init__(self, seed=None):
        self._seed = seed
        self._observation_mode = SimulatorObservationMode.STATE_LEVEL
        self._action_mode = SimulatorActionMode.INDEX_LEVEL
        self._full_observe = False

    def available_actions(self):
        """
        Returns an iterable over the available actions. The action mode may be used to select how actions are referred to.
        TODO: Support multiple action modes

        :return:
        """
        raise NotImplementedError("Abstract Superclass")

    def step(self, action=None):
        """
        Do a step taking the passed action.

        :param action: The index of the action, for deterministic actions, action may be None.
        :return: The observation (on state or program level).
        """
        raise NotImplementedError("Abstract superclass")

    def restart(self):
        """
        Reset the simulator to the initial state
        """
        raise NotImplementedError("Abstract superclass")

    def is_done(self):
        """
        Is the simulator in a sink state?

        :return: Yes, if the simulator is in a sink state.
        """
        return False

    def set_observation_mode(self, mode):
        """

        :param mode: STATE_LEVEL or PROGRAM_LEVEL
        :type mode:
        """
        if not isinstance(mode, SimulatorObservationMode):
            raise RuntimeError("Observation mode must be a SimulatorObservationMode")
        self._observation_mode = mode

    def set_full_observability(self, value):
        """
        Sets whether the full state space is observable.
        Default inherited from the model, but this method overrides the setting.

        :param value:
        """
        self._full_observe = value


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
        self.set_full_observability(self._model.model_type != stormpy.storage.ModelType.POMDP)

    def available_actions(self):
        return range(self.nr_available_actions())

    def nr_available_actions(self):
        return self._model.get_nr_available_actions(self._engine.get_current_state())

    def _report_state(self):
        if self._observation_mode == SimulatorObservationMode.STATE_LEVEL:
            return self._engine.get_current_state()
        elif self._observation_mode == SimulatorObservationMode.PROGRAM_LEVEL:
            return self._state_valuations.get_json(self._engine.get_current_state())
        assert False, "The observation mode is unexpected"

    def _report_observation(self):
        """
        :return:
        """
        #TODO this should be ensured earlier
        assert self._model.model_type == stormpy.storage.ModelType.POMDP
        if self._observation_mode == SimulatorObservationMode.STATE_LEVEL:
            return self._model.get_observation(self._engine.get_current_state())
        elif self._observation_mode == SimulatorObservationMode.PROGRAM_LEVEL:
            raise NotImplementedError("Program level observations are not implemented in storm")
        assert False, "The observation mode is unexpected"

    def _report_result(self):
        if self._full_observe:
            return self._report_state()
        else:
            return self._report_observation()

    def step(self, action=None):
        if action is None:
            if self._model.is_nondeterministic_model and self.nr_available_actions() > 1:
                raise RuntimeError("Must specify an action in nondeterministic models.")
            check = self._engine.step(0)
            assert check
        else:
            if action >= self.nr_available_actions():
                raise RuntimeError(f"Only {self.nr_available_actions()} actions available")
            check = self._engine.step(action)
            assert check
        return self._report_result()


    def restart(self):
        self._engine.reset_to_initial_state()
        return self._report_result()

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
