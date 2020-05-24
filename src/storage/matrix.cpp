#include "matrix.h"
#include "storm/storage/SparseMatrix.h"
#include "storm/storage/BitVector.h"

#include "storm/utility/graph.h"
#include "src/helpers.h"

template<typename ValueType> using SparseMatrix = storm::storage::SparseMatrix<ValueType>;
template<typename ValueType> using SparseMatrixBuilder = storm::storage::SparseMatrixBuilder<ValueType>;
template<typename ValueType> using entry_index = typename storm::storage::SparseMatrix<ValueType>::index_type;
template<typename ValueType> using MatrixEntry = storm::storage::MatrixEntry<entry_index<ValueType>, ValueType>;
using RationalFunction = storm::RationalFunction;
using row_index = unsigned int;

void define_sparse_matrix(py::module& m) {

    // MatrixEntry
    py::class_<MatrixEntry<double>>(m, "SparseMatrixEntry", "Entry of sparse matrix")
        .def("__str__", &streamToString<MatrixEntry<double>>)
        //def_property threw "pointer being freed not allocated" after exiting
        .def("value", &MatrixEntry<double>::getValue, "Value")
        .def("set_value", &MatrixEntry<double>::setValue, py::arg("value"), "Set value")
        .def_property_readonly("column", &MatrixEntry<double>::getColumn, "Column")
    ;

    py::class_<MatrixEntry<RationalFunction>>(m, "ParametricSparseMatrixEntry", "Entry of parametric sparse matrix")
        .def("__str__", &streamToString<MatrixEntry<RationalFunction>>)
        //def_property threw "pointer being freed not allocated" after exiting
        .def("value", &MatrixEntry<RationalFunction>::getValue, "Value")
        .def("set_value", &MatrixEntry<RationalFunction>::setValue, py::arg("value"), "Set value")
        .def_property_readonly("column", &MatrixEntry<RationalFunction>::getColumn, "Column")
    ;

    // SparseMatrixBuilder
    py::class_<SparseMatrixBuilder<double>>(m, "SparseMatrixBuilder", "Builder of sparse matrix")
            .def(py::init<double, double, double, bool, bool, double>(), "rows"_a = 0, "columns"_a = 0, "entries"_a = 0, "force_dimensions"_a = true, "has_custom_row_grouping"_a = false, "row_groups"_a = 0)

            .def("add_next_value", &SparseMatrixBuilder<double>::addNextValue, R"dox(

              Sets the matrix entry at the given row and column to the given value. After all entries have been added,
              calling function build() is mandatory.

              Note: this is a linear setter. That is, it must be called consecutively for each entry, row by row and
              column by column. As multiple entries per column are admitted, consecutive calls to this method are
              admitted to mention the same row-column-pair. If rows are skipped entirely, the corresponding rows are
              treated as empty. If these constraints are not met, an exception is thrown.

              :param double row: The row in which the matrix entry is to be set
              :param double column: The column in which the matrix entry is to be set
              :param double value: The value that is to be set at the specified row and column
            )dox", py::arg("row"), py::arg("column"), py::arg("value"))

            .def("new_row_group", &SparseMatrixBuilder<double>::newRowGroup, py::arg("starting_row"), "Start a new row group in the matrix")
            .def("build", &SparseMatrixBuilder<double>::build, py::arg("overridden_row_count") = 0, py::arg("overridden_column_count") = 0, py::arg("overridden-row_group_count") = 0, "Finalize the sparse matrix")
            .def("get_last_row", &SparseMatrixBuilder<double>::getLastRow, "Get the most recently used row")
            .def("get_current_row_group_count", &SparseMatrixBuilder<double>::getCurrentRowGroupCount, "Get the current row group count")
            .def("get_last_column", &SparseMatrixBuilder<double>::getLastColumn, "the most recently used column")
            .def("replace_columns", &SparseMatrixBuilder<double>::replaceColumns, R"dox(

              Replaces all columns with id >= offset according to replacements.
              Every state with id offset+i is replaced by the id in replacements[i]. Afterwards the columns are sorted.

              :param std::vector<double> const& replacements: replacements Mapping indicating the replacements from offset+i -> value of i
              :param int offset: Offset to add to each id in vector index.
              )dox", py::arg("replacements"), py::arg("offset"))
    ;

    py::class_<SparseMatrixBuilder<RationalFunction>>(m, "ParametricSparseMatrixBuilder", "Builder of parametric sparse matrix")
            .def(py::init<double, double, double, bool, bool, double>(), "rows"_a = 0, "columns"_a = 0, "entries"_a = 0, "force_dimensions"_a = true, "has_custom_row_grouping"_a = false, "row_groups"_a = 0)

            .def("add_next_value", &SparseMatrixBuilder<RationalFunction>::addNextValue, R"dox(

              Sets the matrix entry at the given row and column to the given value. After all entries have been added,
              calling function build() is mandatory.

              Note: this is a linear setter. That is, it must be called consecutively for each entry, row by row and
              column by column. As multiple entries per column are admitted, consecutive calls to this method are
              admitted to mention the same row-column-pair. If rows are skipped entirely, the corresponding rows are
              treated as empty. If these constraints are not met, an exception is thrown.

              :param double row: The row in which the matrix entry is to be set
              :param double column: The column in which the matrix entry is to be set
              :param RationalFunction value: The value that is to be set at the specified row and column
            )dox", py::arg("row"), py::arg("column"), py::arg("value"))

            .def("new_row_group", &SparseMatrixBuilder<RationalFunction>::newRowGroup, py::arg("starting_row"), "Start a new row group in the matrix")
            .def("build", &SparseMatrixBuilder<RationalFunction>::build, py::arg("overridden_row_count") = 0, py::arg("overridden_column_count") = 0, py::arg("overridden-row_group_count") = 0, "Finalize the sparse matrix")
            .def("get_last_row", &SparseMatrixBuilder<RationalFunction>::getLastRow, "Get the most recently used row")
            .def("get_current_row_group_count", &SparseMatrixBuilder<RationalFunction>::getCurrentRowGroupCount, "Get the current row group count")
            .def("get_last_column", &SparseMatrixBuilder<RationalFunction>::getLastColumn, "the most recently used column")
            .def("replace_columns", &SparseMatrixBuilder<RationalFunction>::replaceColumns, R"dox(

              Replaces all columns with id >= offset according to replacements.
              Every state with id offset+i is replaced by the id in replacements[i]. Afterwards the columns are sorted.

              :param std::vector<double> const& replacements: replacements Mapping indicating the replacements from offset+i -> value of i
              :param int offset: Offset to add to each id in vector index.
              )dox", py::arg("replacements"), py::arg("offset"))
    ;

    // SparseMatrix
    py::class_<SparseMatrix<double>>(m, "SparseMatrix", "Sparse matrix")
        .def("__iter__", [](SparseMatrix<double>& matrix) {
                return py::make_iterator(matrix.begin(), matrix.end());
            }, py::keep_alive<0, 1>() /* Essential: keep object alive while iterator exists */)
        .def("__str__", &streamToString<SparseMatrix<double>>)
        .def_property_readonly("nr_rows", &SparseMatrix<double>::getRowCount, "Number of rows")
        .def_property_readonly("nr_columns", &SparseMatrix<double>::getColumnCount, "Number of columns")
        .def_property_readonly("nr_entries", &SparseMatrix<double>::getEntryCount, "Number of non-zero entries")
        .def_property_readonly("_row_group_indices", &SparseMatrix<double>::getRowGroupIndices, "Starting rows of row groups")

        .def("get_row_group_start", [](SparseMatrix<double>& matrix, entry_index<double> row) {return matrix.getRowGroupIndices()[row];})
        .def("get_row_group_end", [](SparseMatrix<double>& matrix, entry_index<double> row) {return matrix.getRowGroupIndices()[row+1];})
        .def_property_readonly("has_trivial_row_grouping", &SparseMatrix<double>::hasTrivialRowGrouping, "Trivial row grouping")
        .def("get_row", [](SparseMatrix<double>& matrix, entry_index<double> row) {
                return matrix.getRows(row, row+1);
            }, py::return_value_policy::reference, py::keep_alive<1, 0>(), py::arg("row"), "Get row")
        .def("get_rows", [](SparseMatrix<double>& matrix, entry_index<double> start, entry_index<double> end) {
                return matrix.getRows(start, end);
            }, py::return_value_policy::reference, py::keep_alive<1, 0>(), py::arg("row_start"), py::arg("row_end"), "Get rows from start to end")
        .def("print_row", [](SparseMatrix<double> const& matrix, entry_index<double> row) {
                std::stringstream stream;
                auto rows = matrix.getRows(row, row+1);
                for (auto transition : rows) {
                    stream << transition << ", ";
                }
                return stream.str();
            }, py::arg("row"), "Print rows from start to end")
        .def("submatrix", [](SparseMatrix<double> const& matrix, storm::storage::BitVector const& rowConstraint, storm::storage::BitVector const& columnConstraint, bool insertDiagonalEntries = false) {
                return matrix.getSubmatrix(true, rowConstraint, columnConstraint, insertDiagonalEntries);
            }, py::arg("row_constraint"), py::arg("column_constraint"), py::arg("insert_diagonal_entries") = false, "Get submatrix")
        // Entry_index lead to problems
        .def("row_iter", [](SparseMatrix<double>& matrix, row_index start, row_index end) {
                return py::make_iterator(matrix.begin(start), matrix.end(end));
            }, py::keep_alive<0, 1>() /* keep object alive while iterator exists */, py::arg("row_start"), py::arg("row_end"), "Get iterator from start to end")

        // (partial) container interface to allow e.g. matrix[7:9]
        .def("__len__", &SparseMatrix<double>::getRowCount)
        .def("__getitem__", [](SparseMatrix<double>& matrix, entry_index<double> i) {
                if (i >= matrix.getRowCount())
                    throw py::index_error();
                return matrix.getRows(i, i+1);
            }, py::return_value_policy::reference, py::keep_alive<1, 0>())
        .def("__getitem__", [](SparseMatrix<double>& matrix, py::slice slice) {
                size_t start, stop, step, slice_length;
                if (!slice.compute(matrix.getRowCount(), &start, &stop, &step, &slice_length))
                    throw py::error_already_set();
                if (step != 1)
                    throw py::value_error(); // not supported
                return matrix.getRows(start, stop);
            }, py::return_value_policy::reference, py::keep_alive<1, 0>())
    ;

    m.def("_topological_sort_double", [](SparseMatrix<double>& matrix, std::vector<uint64_t> initial) { return storm::utility::graph::getTopologicalSort(matrix, initial); }, "matrix"_a, "initial"_a,  "get topological sort w.r.t. a transition matrix");
    m.def("_topological_sort_rf", [](SparseMatrix<storm::RationalFunction>& matrix, std::vector<uint64_t> initial) { return storm::utility::graph::getTopologicalSort(matrix, initial); }, "matrix"_a, "initial"_a,  "get topological sort w.r.t. a transition matrix");


    py::class_<SparseMatrix<RationalFunction>>(m, "ParametricSparseMatrix", "Parametric sparse matrix")
        .def("__iter__", [](SparseMatrix<RationalFunction>& matrix) {
                return py::make_iterator(matrix.begin(), matrix.end());
            }, py::keep_alive<0, 1>() /* Essential: keep object alive while iterator exists */)
        .def("__str__", &streamToString<SparseMatrix<RationalFunction>>)
        .def_property_readonly("nr_rows", &SparseMatrix<RationalFunction>::getRowCount, "Number of rows")
        .def_property_readonly("nr_columns", &SparseMatrix<RationalFunction>::getColumnCount, "Number of columns")
        .def_property_readonly("nr_entries", &SparseMatrix<RationalFunction>::getEntryCount, "Number of non-zero entries")
        .def_property_readonly("_row_group_indices", &SparseMatrix<RationalFunction>::getRowGroupIndices, "Starting rows of row groups")
        .def("get_row_group_start", [](SparseMatrix<RationalFunction>& matrix, entry_index<RationalFunction> row) {return matrix.getRowGroupIndices()[row];})
        .def("get_row_group_end", [](SparseMatrix<RationalFunction>& matrix, entry_index<RationalFunction> row) {return matrix.getRowGroupIndices()[row+1];})
        .def_property_readonly("has_trivial_row_grouping", &SparseMatrix<RationalFunction>::hasTrivialRowGrouping, "Trivial row grouping")
        .def("get_row", [](SparseMatrix<RationalFunction>& matrix, entry_index<RationalFunction> row) {
                return matrix.getRows(row, row+1);
            }, py::return_value_policy::reference, py::keep_alive<1, 0>(), py::arg("row"), "Get row")
        .def("get_rows", [](SparseMatrix<RationalFunction>& matrix, entry_index<storm::RationalFunction> start, entry_index<storm::RationalFunction> end) {
                return matrix.getRows(start, end);
            }, py::return_value_policy::reference, py::keep_alive<1, 0>(), py::arg("row_start"), py::arg("row_end"), "Get rows from start to end")
        .def("print_row", [](SparseMatrix<RationalFunction> const& matrix, entry_index<storm::RationalFunction> row) {
                std::stringstream stream;
                auto rows = matrix.getRows(row, row+1);
                for (auto transition : rows) {
                    stream << transition << ", ";
                }
                return stream.str();
            }, py::arg("row"), "Print row")
        .def("submatrix", [](SparseMatrix<RationalFunction> const& matrix, storm::storage::BitVector const& rowConstraint, storm::storage::BitVector const& columnConstraint, bool insertDiagonalEntries = false) {
                return matrix.getSubmatrix(true, rowConstraint, columnConstraint, insertDiagonalEntries);
            }, py::arg("row_constraint"), py::arg("column_constraint"), py::arg("insert_diagonal_entries") = false, "Get submatrix")
        // Entry_index lead to problems
        .def("row_iter", [](SparseMatrix<RationalFunction>& matrix, row_index start, row_index end) {
                return py::make_iterator(matrix.begin(start), matrix.end(end));
            }, py::keep_alive<0, 1>() /* keep object alive while iterator exists */, py::arg("row_start"), py::arg("row_end"), "Get iterator from start to end")

        // (partial) container interface to allow e.g. matrix[7:9]
        .def("__len__", &SparseMatrix<RationalFunction>::getRowCount)
        .def("__getitem__", [](SparseMatrix<RationalFunction>& matrix, entry_index<RationalFunction> i) {
                if (i >= matrix.getRowCount())
                    throw py::index_error();
                return matrix.getRows(i, i+1);
            }, py::return_value_policy::reference, py::keep_alive<1, 0>())
        .def("__getitem__", [](SparseMatrix<RationalFunction>& matrix, py::slice slice) {
                size_t start, stop, step, slice_length;
                if (!slice.compute(matrix.getRowCount(), &start, &stop, &step, &slice_length))
                    throw py::error_already_set();
                if (step != 1)
                    throw py::value_error(); // not supported
                return matrix.getRows(start, stop);
            }, py::return_value_policy::reference, py::keep_alive<1, 0>())
    ;

    // Rows
    py::class_<SparseMatrix<double>::rows>(m, "SparseMatrixRows", "Set of rows in a sparse matrix")
        .def("__iter__", [](SparseMatrix<double>::rows& rows) {
                return py::make_iterator(rows.begin(), rows.end());
            }, py::keep_alive<0, 1>())
        .def("__str__", &containerToString<SparseMatrix<double>::rows>)
        .def("__len__", &storm::storage::SparseMatrix<double>::rows::getNumberOfEntries)
    ;

    py::class_<SparseMatrix<RationalFunction>::rows>(m, "ParametricSparseMatrixRows", "Set of rows in a parametric sparse matrix")
        .def("__iter__", [](SparseMatrix<RationalFunction>::rows& rows) {
                return py::make_iterator(rows.begin(), rows.end());
            }, py::keep_alive<0, 1>())
        .def("__str__", &containerToString<SparseMatrix<RationalFunction>::rows>)
        .def("__len__", &storm::storage::SparseMatrix<storm::RationalFunction>::rows::getNumberOfEntries)
    ;
}
