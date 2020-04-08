import stormpy
import numpy as np


class TestMatrixBuilder:
    def test_matrix_builder(self):
        builder = stormpy.SparseMatrixBuilder(force_dimensions=True)
        matrix = builder.build()
        assert matrix.nr_columns == 0
        assert matrix.nr_rows == 0
        assert matrix.nr_entries == 0

        builder_5x5 = stormpy.SparseMatrixBuilder(5, 5, force_dimensions=False)

        builder_5x5.add_next_value(0, 0, 0)
        builder_5x5.add_next_value(0, 1, 0.1)
        builder_5x5.add_next_value(2, 2, 22)
        builder_5x5.add_next_value(2, 3, 23)

        assert builder_5x5.get_last_column() == 3
        assert builder_5x5.get_last_row() == 2

        builder_5x5.add_next_value(3, 2, 32)
        builder_5x5.add_next_value(3, 4, 34)
        builder_5x5.add_next_value(4, 3, 43)

        matrix_5x5 = builder_5x5.build()

        assert matrix_5x5.nr_columns == 5
        assert matrix_5x5.nr_rows == 5
        assert matrix_5x5.nr_entries == 7

        for e in matrix_5x5:
            assert (e.value() == 0.1 and e.column == 1) or e.value() == 0 or (e.value() > 20 and e.column > 1)

    def test_matrix_replace_columns(self):
        builder = stormpy.SparseMatrixBuilder(3, 4, force_dimensions=False)

        builder.add_next_value(0, 0, 0)
        builder.add_next_value(0, 1, 1)
        builder.add_next_value(0, 2, 2)
        builder.add_next_value(0, 3, 3)

        builder.add_next_value(1, 1, 1)
        builder.add_next_value(1, 2, 2)
        builder.add_next_value(1, 3, 3)

        builder.add_next_value(2, 1, 1)
        builder.add_next_value(2, 2, 2)
        builder.add_next_value(2, 3, 3)

        # replace rows
        builder.replace_columns([3, 2, 1], 1)
        matrix = builder.build()

        assert matrix.nr_entries == 10

        # Check if columns where replaced
        for e in matrix:
            assert (e.value() == 0 and e.column == 0) or (e.value() == 3 and e.column == 1) or (
                        e.value() == 2 and e.column == 2) or (e.value() == 1 and e.column == 3)

    def test_matrix_builder_row_grouping(self):

        num_rows = 5
        builder = stormpy.SparseMatrixBuilder(num_rows, 6, has_custom_row_grouping=True, row_groups=2)

        builder.new_row_group(1)
        assert builder.get_current_row_group_count() == 1

        builder.new_row_group(4)
        assert builder.get_current_row_group_count() == 2
        matrix = builder.build()

        assert matrix.get_row_group_start(0) == 1
        assert matrix.get_row_group_end(0) == 4

        assert matrix.get_row_group_start(1) == 4
        assert matrix.get_row_group_end(1) == 5

    def test_matrix_from_numpy(self):
        array = np.array([[0, 2],
                          [3, 4],
                          [0.1, 24],
                          [-0.3, -4]], dtype='float64')

        matrix = stormpy.build_sparse_matrix(array)

        # Check matrix dimension
        assert matrix.nr_rows == array.shape[0]
        assert matrix.nr_columns == array.shape[1]
        assert matrix.nr_entries == 8

        # Check matrix values
        for r in range(array.shape[1]):
            row = matrix.get_row(r)
            for e in row:
                assert (e.value() == array[r, e.column])

    def test_matrix_from_numpy_row_grouping(self):
        array = np.array([[0, 2],
                          [3, 4],
                          [0.1, 24],
                          [-0.3, -4]], dtype='float64')

        matrix = stormpy.build_sparse_matrix(array, row_group_indices=[1, 3])

        # Check matrix dimension
        assert matrix.nr_rows == array.shape[0]
        assert matrix.nr_columns == array.shape[1]
        assert matrix.nr_entries == 8

        # Check matrix values
        for r in range(array.shape[1]):
            row = matrix.get_row(r)
            for e in row:
                assert (e.value() == array[r, e.column])

        # Check row groups
        assert matrix.get_row_group_start(0) == 1
        assert matrix.get_row_group_end(0) == 3

        assert matrix.get_row_group_start(1) == 3
        assert matrix.get_row_group_end(1) == 4
