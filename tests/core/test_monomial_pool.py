import pycarl
from configurations import PackageSelector, cln


def test_clear(PackageSelector):
    pycarl.clear_variable_pool()
    pycarl.clear_monomial_pool()
    var = pycarl.Variable("i")
    pol = package.Polynomial(var)

    pycarl.clear_variable_pool()
    pycarl.clear_monomial_pool()
    var = pycarl.Variable("i")
    pol = package.Polynomial(var)

    pycarl.clear_variable_pool()
    pycarl.clear_monomial_pool()
    var = pycarl.Variable("i")
    pol = package.Polynomial(var)


@cln
def test_clear():
    pycarl.clear_variable_pool()
    pycarl.clear_monomial_pool()
    var = pycarl.Variable("i")
    pol = pycarl.cln.Polynomial(var)

    pycarl.clear_variable_pool()
    pycarl.clear_monomial_pool()
    var = pycarl.Variable("i")
    pol = pycarl.gmp.Polynomial(var)

    pycarl.clear_variable_pool()
    pycarl.clear_monomial_pool()
    var = pycarl.Variable("i")
    pol = pycarl.cln.Polynomial(var)
