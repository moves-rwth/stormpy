import stormpy
from stormpy.utility.multiobjective_plotting import prepare_multiobjective_result_for_plotting, plot_convex_pareto_curve_demo
from helpers.helper import get_example_path

from configurations import plotting, numpy_avail

class TestModelChecking:
    def naive_api_double_no_plotting_test(self):
        program = stormpy.parse_prism_program(get_example_path("mdp", "multiobjective1.nm"))
        properties = stormpy.parse_properties_for_prism_program("multi(Pmax=? [ F<=3 s=2 ],R{\"rew\"}max=? [ F s=2 ])", program)
        model = stormpy.build_model(program, properties)
        result = stormpy.model_checking(model,properties[0])
        assert len(result.get_underapproximation().vertices) == 3
        assert len(result.get_overapproximation().vertices) == 6

    @plotting
    @numpy_avail
    def naive_api_double_with_plotting_test(self):
        import matplotlib.pyplot as plt

        program = stormpy.parse_prism_program(get_example_path("mdp", "multiobjective1.nm"))
        properties = stormpy.parse_properties_for_prism_program("multi(Pmax=? [ F<=3 s=2 ],R{\"rew\"}max=? [ F s=2 ])", program)
        model = stormpy.build_model(program, properties)
        result = stormpy.model_checking(model,properties[0])
        lower_left = [0,0]
        upper_right = [1,20]
        formula = properties[0].raw_formula
        underapprox_points, overapprox_points = prepare_multiobjective_result_for_plotting(result, lower_left, upper_right, formula)
        fig, ax = plt.subplots()
        plot_convex_pareto_curve_demo(ax, underapprox_points, overapprox_points, lower_left, upper_right)
        ax.set_xlabel(formula.subformulas[0])
        ax.set_ylabel(formula.subformulas[1])
        #plt.show()
