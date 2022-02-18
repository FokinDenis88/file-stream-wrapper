#ifndef DATATABLE_H
#define DATATABLE_H

#include <vector>
#include <array>
#include <string>
#include <tuple>

// Table with data. Header consists of string value. There can be no header. Data is homogeneous in colomn.
// Не использовать в качестве разделителя пробел и табуляцию
// У каждого столбца свой тип
template<typename... ColumnT>
struct DataTableTuple {
    // Tuple of table. Or in other words row.
    typedef std::tuple<ColumnT...> RowType;
    // All data rows of table
    typedef std::vector<RowType> AllRowsType;

    // Row with names of columns
    // There can be no header in csv table, only data
    std::vector<std::string> header;
    //header{ std::vector<std::string>(sizeof...(ColumnT)) }

    // Row of table
    AllRowsType data_rows;
    //std::vector<std::tuple<ColumnT...>> data_rows;
};

// Table with data. Header consists of string value. There can be no header. Data is homogeneous in colomn.
// Не использовать в качестве разделителя пробел и табуляцию
// Все столбцы одинакового типа
template<size_t kColumnCount, typename ColumnT = std::string>
struct DataTableArray {
    // Tuple of table. Or in other words row.
    typedef std::array<typename ColumnT, kColumnCount> RowType;
    // All data rows of table
    typedef std::vector<RowType> AllRowsType;

    // Row with names of columns
    // There can be no header in csv table, only data
    std::vector<std::string> header;

    // Row of table
    AllRowsType data_rows;
};

#endif // !DATATABLE_H