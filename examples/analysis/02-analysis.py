import stormpy
import stormpy.core

import stormpy.examples
import stormpy.examples.files


def example_analysis_02():
    path = stormpy.examples.files.prism_dtmc_die
    prism_program = stormpy.parse_prism_program(path)

    formula_str = "P=? [F s=7 & d=2]"
    properties = stormpy.parse_properties(formula_str, prism_program)
    model = stormpy.build_model(prism_program, properties)
    env = stormpy.Environment()
    env.solver_environment.set_linear_equation_solver_type(stormpy.EquationSolverType.native)
    env.solver_environment.native_solver_environment.method = stormpy.NativeLinearEquationSolverMethod.power_iteration
    env.solver_environment.native_solver_environment.maximum_iterations = 2
    result = stormpy.model_checking(model, properties[0], environment=env)


if __name__ == '__main__':
    example_analysis_02()