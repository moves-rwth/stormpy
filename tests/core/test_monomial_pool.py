import pycarl
import pycarl.cln
import pycarl.gmp

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
