import pycarl


class TestClnInteger:

    def test_construction(self):
        i3 = pycarl.ClnInteger("3")
        assert str(i3) == "3"
        i4 = pycarl.ClnInteger(4)
        assert str(i4) == "4"

    def test_eq(self):
        i3 = pycarl.ClnInteger(3)
        assert i3 == 3
        i3b =  pycarl.ClnInteger(3)
        assert i3 == i3b
        in3 = pycarl.ClnInteger(-3)
        assert in3 == -3

    def test_addition(self):
        i3 = pycarl.ClnInteger(3)
        assert i3 + i3 == 6
        assert i3 + 4 == 7
        assert i3 + -3 == 0

    def test_neg(self):
        i3 = pycarl.ClnInteger(3)
        in3 = -i3
        assert in3 == -3
        inn3 = -in3
        assert i3 == inn3



class TestGmpInteger:

    def test_construction(self):
        i3 = pycarl.GmpInteger("3")
        assert str(i3) == "3"
        i4 = pycarl.GmpInteger(4)
        assert str(i4) == "4"

    def test_eq(self):
        i3 = pycarl.GmpInteger(3)
        assert i3 == 3
        i3b =  pycarl.GmpInteger(3)
        assert i3 == i3b
        in3 = pycarl.GmpInteger(-3)
        assert in3 == -3

    def test_addition(self):
        i3 = pycarl.GmpInteger(3)
        assert i3 + i3 == 6
        assert i3 + 4 == 7
        assert i3 + -3 == 0

    def test_neg(self):
        i3 = pycarl.GmpInteger(3)
        in3 = -i3
        assert in3 == -3
        inn3 = -in3
        assert i3 == inn3