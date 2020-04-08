import stormpy.utility
from . import storage
from .storage import *


def build_sparse_matrix(array, row_group_indices=[]):
    """
    Build a sparse matrix from numpy array.

    :param numpy array: The array.
    :param List[double] row_group_indices: List containing the starting row of each row group.
    :return: Sparse matrix.
    """
    num_row = array.shape[0]
    num_col = array.shape[1]

    if 0 < len(row_group_indices):
        builder = storage.SparseMatrixBuilder(rows=num_row, columns=num_col, has_custom_row_grouping=True,
                                              row_groups=len(row_group_indices))
    else:
        builder = storage.SparseMatrixBuilder(rows=num_row, columns=num_col)

    for r in range(num_row):
        if r in row_group_indices:
            builder.new_row_group(r)
        for c in range(num_col):
            builder.add_next_value(r, c, array[r, c])

    return builder.build()
