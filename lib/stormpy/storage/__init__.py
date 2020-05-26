import stormpy.utility
from . import storage
from .storage import *


def build_sparse_matrix(array, row_group_indices=[]):
    """
    Build a sparse matrix from numpy array.

    :param numpy array: The array.
    :param List[double] row_group_indices: List containing the starting row of each row group in ascending order.
    :return: Sparse matrix.
    """

    num_row = array.shape[0]
    num_col = array.shape[1]

    len_group_indices = len(row_group_indices)
    if len_group_indices > 0:
        builder = storage.SparseMatrixBuilder(rows=num_row, columns=num_col, has_custom_row_grouping=True,
                                              row_groups=len_group_indices)
    else:
        builder = storage.SparseMatrixBuilder(rows=num_row, columns=num_col)

    row_group_index = 0
    for r in range(num_row):
        # check whether to start a custom row group
        if row_group_index < len_group_indices and r == row_group_indices[row_group_index]:
            builder.new_row_group(r)
            row_group_index += 1
        # insert values of the current row
        for c in range(num_col):
            builder.add_next_value(r, c, array[r, c])

    return builder.build()

def build_parametric_sparse_matrix(array, row_group_indices=[]):
    """
    Build a sparse matrix from numpy array.

    :param numpy array: The array.
    :param List[double] row_group_indices: List containing the starting row of each row group in ascending order.
    :return: Parametric sparse matrix.
    """

    num_row = array.shape[0]
    num_col = array.shape[1]

    len_group_indices = len(row_group_indices)
    if len_group_indices > 0:
        builder = storage.ParametricSparseMatrixBuilder(rows=num_row, columns=num_col, has_custom_row_grouping=True,
                                              row_groups=len_group_indices)
    else:
        builder = storage.ParametricSparseMatrixBuilder(rows=num_row, columns=num_col)

    row_group_index = 0
    for r in range(num_row):
        # check whether to start a custom row group
        if row_group_index < len_group_indices and r == row_group_indices[row_group_index]:
            builder.new_row_group(r)
            row_group_index += 1
        # insert values of the current row
        for c in range(num_col):
            builder.add_next_value(r, c, array[r, c])

    return builder.build()

