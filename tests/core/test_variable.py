import pycarl

class TestCore:
    def test_init(self):
        var = pycarl.Variable("x")
        assert var.name == "x"	

    def test_product(self):
        x = pycarl.Variable("x")
        y = pycarl.Variable("y")
        product = x * y
    
