import pycarl
from pycarl.formula import Relation
import pycarl.cln.formula, pycarl.gmp.formula
import sys
import os
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from package_selector import PackageSelector

class TestConstraint(PackageSelector):

    def test_init_bool(self, package):
        constraint = package.formula.Constraint(True)
        assert constraint.relation == Relation.EQ
        assert constraint.lhs == 0

    def test_init_var(self, package):
        pycarl.clear_variable_pool()
        var = pycarl.Variable("x")
        bound = package.Rational(3)
        constraint = package.formula.Constraint(var, Relation.GREATER, bound)
        assert constraint.relation == Relation.LESS

    def test_init_pol(self, package):
        pycarl.clear_variable_pool()
        var = pycarl.Variable("x")
        pol = var * var +  package.Integer(2)
        constraint = package.formula.Constraint(pol, Relation.LEQ)
        assert constraint.relation == Relation.LESS
