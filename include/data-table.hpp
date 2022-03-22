#ifndef DATA_TABLE_HPP_
#define DATA_TABLE_HPP_

#include <vector>
#include <array>
#include <string>
#include <tuple>

/// Table with data. Header consists of string value. There can be no header. Data is homogeneous in colomn.
template<typename... ColumnT>
struct DataTableTuple {
    // Tuple of table. Or in other words row.
    using RowType = typename std::tuple<ColumnT...>;
    // All data rows of table
    using AllRowsType = typename std::vector<RowType>;

    // Row with names of columns
    // There can be no header in csv table, only data
    std::vector<std::string> header;
    //header{ std::vector<std::string>(sizeof...(ColumnT)) }

    // Row of table
    AllRowsType data_rows;
    //std::vector<std::tuple<ColumnT...>> data_rows;
};

/// Table with data. Header consists of string value. There can be no header. Data is homogeneous in colomn.
template<size_t kColumnCount, typename ColumnT = std::string>
struct DataTableArray {
    // Tuple of table. Or in other words row.
    using RowType = typename std::array<ColumnT, kColumnCount>;
    // All data rows of table
    using AllRowsType = typename std::vector<RowType>;

    // Row with names of columns
    // There can be no header in csv table, only data
    std::vector<std::string> header;

    // Row of table
    AllRowsType data_rows;
};

#endif // !DATA_TABLE_HPP_
