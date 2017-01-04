import pycarl

class TestCore:
    def test_init(self):
        var = pycarl.Variable("x")

    def test_product(self):
        x = pycarl.Variable("x")
        y = pycarl.Variable("y")
        product = x * y 
    
