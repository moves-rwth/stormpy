#include "matrix.h"
#include "storm/storage/SparseMatrix.h"
#include "storm/storage/BitVector.h"

#include "storm/utility/graph.h"
#include "src/helpers.h"

template<typename ValueType> using SparseMatrix = storm::storage::SparseMatrix<ValueType>;
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
