**************
Reward Models
**************

In :doc:`../getting_started`, we mainly looked at probabilities in the Markov models and properties that refer to these probabilities.
In this section, we discuss reward models.

Exploring reward models
------------------------
.. seealso:: `01-reward-models.py <https://github.com/moves-rwth/stormpy/blob/master/examples/reward_models/01-reward-models.py>`_

We consider the die again, but with another property which talks about the expected reward::

    >>> import stormpy
    >>> import stormpy.examples
    >>> import stormpy.examples.files
    >>> program = stormpy.parse_prism_program(stormpy.examples.files.prism_dtmc_die)
    >>> prop = "R=? [F \"done\"]"

    >>> properties = stormpy.parse_properties(prop, program, None)
    >>> model = stormpy.build_model(program, properties)
    >>> assert len(model.reward_models) == 1

The model now has a reward model, as the property talks about rewards.
When :doc:`building_models` from explicit sources, the reward model is always included if it is defined in the source.
We can do model checking analogous to probabilities::


    >>> initial_state = model.initial_states[0]
    >>> result = stormpy.model_checking(model, properties[0])
    >>> print("Result: {}".format(result.at(initial_state)))
    Result: 3.666666666666667


The reward model has a name which we can obtain as follows::

    >>> reward_model_name = list(model.reward_models.keys())[0]
    >>> print(reward_model_name)
    coin_flips

We discuss later how to work with multiple reward models.
Rewards come in multiple fashions, as state rewards, state-action rewards and as transition rewards.
In this example, we only have state-action rewards. These rewards are a vector, over which we can trivially iterate::

    >>> assert not model.reward_models[reward_model_name].has_state_rewards
    >>> assert model.reward_models[reward_model_name].has_state_action_rewards
    >>> assert not model.reward_models[reward_model_name].has_transition_rewards
    >>> for reward in model.reward_models[reward_model_name].state_action_rewards:
    ...    print(reward)
    1.0
    1.0
    1.0
    1.0
    1.0
    1.0
    1.0
    0.0
    0.0
    0.0
    0.0
    0.0
    0.0


