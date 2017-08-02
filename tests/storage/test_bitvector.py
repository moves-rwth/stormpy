import stormpy


class TestBitvector:
    def test_init_default(self):
        bit = stormpy.BitVector()
        assert bit.size() == 0
        assert bit.number_of_set_bits() == 0

    def test_init_length(self):
        bit = stormpy.BitVector(10)
        assert bit.size() == 10
        assert bit.number_of_set_bits() == 0
        bit = stormpy.BitVector(5, True)
        assert bit.size() == 5
        assert bit.number_of_set_bits() == 5

    def test_init_vector(self):
        bit = stormpy.BitVector(5, [2, 3])
        assert bit.size() == 5
        assert bit.number_of_set_bits() == 2
        assert bit.get(0) is False
        assert bit.get(1) is False
        assert bit.get(2) is True
        assert bit.get(3) is True
        assert bit.get(4) is False

    def test_init_bitvector(self):
        bit = stormpy.BitVector(7, [0, 6])
        bit2 = stormpy.BitVector(bit)
        assert bit == bit2
        assert bit2.get(0) is True
        assert bit2.get(6) is True

    def test_negate(self):
        bit = stormpy.BitVector(7, [0, 6])
        bit2 = stormpy.BitVector(bit)
        bit = ~bit
        assert bit.get(0) is False
        assert bit.get(6) is False
        for i in range(bit.size()):
            assert bit.get(i) is not bit2.get(i)
