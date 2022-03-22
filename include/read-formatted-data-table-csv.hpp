#ifndef READDATATABLECSV_H
#define READDATATABLECSV_H

#include <fstream>
#include <type_traits>
#include <string>
#include <vector>
#include <array>
#include <set>
#include <utility> // std::move indeger_sequence
#include <tuple>
#include <typeinfo>
#include <exception>
#include <stdexcept>

#include "boost/regex/v5/regex.hpp"

#include "file-constants.hpp"
#include "file-service.hpp"

#include "read-binary-file.hpp"
//#include "ReadBinaryFileThread.h"
#include "data-table.hpp"

// TODO: Реализовать Неформатированное чтение и запись файлов csv. Функции должны подходить для любых шаблонов csv файла.

namespace file {

    // Delimiter in csv file on string value in the table field
    const std::set<char> kStringDelimiter{ '"', '\'' };
    // Delimiter between table fields in csv file
    const std::set<char> kFieldDelimiter{ ',' , ';' , ':' , '\t' , 0x20 }; // 0x20 = white space

    const char* const kHeaderCellErrorMessage{ "Empty Header Cell. Not all cells in data table has data." };
    const char* const kDataCellErrorMessage{ "Empty Data Cell. Not all cells in data table has data." };


    // Заменяет запятую
    inline std::string ReplaceComma(const std::string& str) {
        return boost::regex_replace(str, boost::regex(","), ".");
    }

//////// Types of columns are not equal

    // Convert string data to data of field in data table
    template<const size_t Index, typename... ColumnT>
    inline void ConvertDataOnIndexTuple(std::tuple<ColumnT...>& buffer_tuple, std::string(&buffer_str)[]) {
        using TableRow = std::tuple<ColumnT...>;
        if constexpr (std::is_same_v<std::tuple_element_t<Index, TableRow>, std::string>) {
            std::get<Index>(buffer_tuple) = buffer_str[Index];
        }
        else if constexpr (std::is_same_v<std::tuple_element_t<Index, TableRow>, int>) {
            std::get<Index>(buffer_tuple) = std::stoi(buffer_str[Index]);
        }
        else if constexpr (std::is_same_v<std::tuple_element_t<Index, TableRow>, float>) {
            std::get<Index>(buffer_tuple) = std::stof(ReplaceComma(buffer_str[Index]));
        }
        else if constexpr (std::is_same_v<std::tuple_element_t<Index, TableRow>, double>) {
            std::get<Index>(buffer_tuple) = std::stod(ReplaceComma(buffer_str[Index]));
        }
    }

    template<typename... ColumnT, size_t... Indeces>
    inline void ConvertDataTuple(std::tuple<ColumnT...>& buffer_tuple, std::string(&buffer_str)[], std::index_sequence<Indeces...>) {
        (ConvertDataOnIndexTuple<Indeces>(buffer_tuple, buffer_str), ...);
    }

    // Realization of formatted input
    // Реализация чтения текстовой информации из файл через оператор >>
    // Осторожно: Управляющие символы в разных операционных системах могут интерпретироваться по разному.
    // Ты читаешь из файла не всегда то, что записал
    // Delimiter in csv file between columns must be white space
    // Не использовать в качестве разделителя пробел и табуляцию
    template<typename... ColumnT>
    DataTableTuple<ColumnT...> ReadFormattedDataTableCSV(const char* file_path) {
        std::string str_data{ std::move(MakeStringFrmVectorByte(ReadBinaryFile<char>(file_path))) };

        DataTableTuple<ColumnT...> data_table{};
        { // (?<Value>((?<!'|")\b\d+\b(?!'|"))|((?<='|")\b\d+(?:,\d+)?\b(?='|"))|((?<='|")[^'",;:\r\n]+(?='|")))((?<=(?<Field>,|;|:|\t|\s))\k<Value>)*
            const boost::regex words_regex{
                "(?<Value>((?<!'|\")\\b\\d+\\b(?!'|\"))|((?<='|\")\\b\\d+(?:,\\d+)?\\b(?='|\"))|((?<='|\")[^'\",;:\\r\\n]+(?='|\")))((?<=(?<Field>,|;|:|\\t|\\s))\\k<Value>)*"
            };
            const boost::sregex_iterator words_begin{ boost::sregex_iterator(str_data.begin(), str_data.end(), words_regex) };
            const boost::sregex_iterator words_end{ boost::sregex_iterator() };

            boost::sregex_iterator i{ words_begin };
            constexpr size_t columns_count{ sizeof...(ColumnT) };
            size_t last_column_indx{ columns_count - 1 };
            size_t column{ 0 };
            for (; i != words_end && column < columns_count; ++i) {
                data_table.header.emplace_back(std::move((*i).str()));
                ++column;
            }
            if (column == columns_count) { 
                std::tuple<ColumnT...> buffer_tuple{};
                std::string buffer_str[columns_count]{};
                while (i != words_end) {
                    column = 0;
                    for (; i != words_end && column < columns_count; ++i) { // Read whole row
                        buffer_str[column] = i->str();
                        ++column;
                    }
                    if (column == columns_count) { // convert string to properly type
                        ConvertDataTuple(buffer_tuple, buffer_str, std::make_index_sequence<columns_count>{});
                        data_table.data_rows.emplace_back(std::move(buffer_tuple));
                    } else { throw std::runtime_error(kDataCellErrorMessage); }
                }
            } else { throw std::runtime_error(kHeaderCellErrorMessage); }
        }
        return data_table;
    }

    // Используется, если путь к файлу - это объект типа std::string
    template<typename... ColumnT>
    DataTableTuple<ColumnT...> ReadFormattedDataTableCSV(const std::string& file_path) {
        return ReadFormattedDataTableCSV<ColumnT...>(file_path.c_str());
    }

////// Types of columns are equal

    // Convert string data to data of field in data table
    template<size_t kColumnCount, typename ColumnT = std::string>
    inline void ConvertDataOnIndexArray(std::array<ColumnT, kColumnCount>& buffer_array, std::string(&buffer_str)[], const size_t i) {
        if constexpr (std::is_same_v<ColumnT, std::string>) {
            buffer_array[i] = buffer_str[i];
        }
        else if constexpr (std::is_same_v<ColumnT, int>) {
            buffer_array[i] = std::stoi(buffer_str[i]);
        }
        else if constexpr (std::is_same_v<ColumnT, float>) {
            buffer_array[i] = std::stof(ReplaceComma(buffer_str[i]));
        }
        else if constexpr (std::is_same_v<ColumnT, double>) {
            buffer_array[i] = std::stod(ReplaceComma(buffer_str[i]));
        }
    }

    template<size_t kColumnCount, typename ColumnT = std::string>
    inline void ConvertDataArray(std::array<ColumnT, kColumnCount>& buffer_array, std::string(&buffer_str)[]) {
        for (size_t i = 0; i < kColumnCount; ++i) {
            ConvertDataOnIndexArray<kColumnCount, ColumnT>(buffer_array, buffer_str, i);
        }
    }

    // Realization of formatted input
    // Реализация чтения текстовой информации из файл через оператор >>
    // Осторожно: Управляющие символы в разных операционных системах могут интерпретироваться по разному.
    // Ты читаешь из файла не всегда то, что записал
    // Delimiter in csv file between columns must be white space
    // Не использовать в качестве разделителя пробел и табуляцию
    template<size_t kColumnCount, typename ColumnT = std::string>
    DataTableArray<kColumnCount, ColumnT> ReadFormattedDataTableCSV(const char* file_path) {
        std::string str_data{ std::move(MakeStringFrmVectorByte(ReadBinaryFile<char>(file_path))) };

        DataTableArray<kColumnCount, ColumnT> data_table{};
        { // (?<Value>((?<!'|")\b\d+\b(?!'|"))|((?<='|")\b\d+(?:,\d+)?\b(?='|"))|((?<='|")[^'",;:\r\n]+(?='|")))((?<=(?<Field>,|;|:|\t|\s))\k<Value>)*
            const boost::regex words_regex{
                "(?<Value>((?<!'|\")\\b\\d+\\b(?!'|\"))|((?<='|\")\\b\\d+(?:,\\d+)?\\b(?='|\"))|((?<='|\")[^'\",;:\\r\\n]+(?='|\")))((?<=(?<Field>,|;|:|\\t|\\s))\\k<Value>)*"
            };
            const boost::sregex_iterator words_begin{ boost::sregex_iterator(str_data.begin(), str_data.end(), words_regex) };
            const boost::sregex_iterator words_end{ boost::sregex_iterator() };

            boost::sregex_iterator i{ words_begin };
            constexpr size_t columns_count{ kColumnCount };
            size_t last_column_indx{ columns_count - 1 };
            size_t column{ 0 };
            for (; i != words_end && column < columns_count; ++i) {
                data_table.header.emplace_back(std::move((*i).str()));
                ++column;
            }
            if (column == columns_count) {
                std::array<ColumnT, kColumnCount> buffer_array{};
                std::string buffer_str[columns_count]{};
                while (i != words_end) {
                    column = 0;
                    for (; i != words_end && column < columns_count; ++i) { // Read whole row
                        buffer_str[column] = i->str();
                        ++column;
                    }
                    if (column == columns_count) { // convert string to properly type
                        ConvertDataArray(buffer_array, buffer_str);
                        data_table.data_rows.emplace_back(std::move(buffer_array));
                    }
                    else { throw std::runtime_error(kDataCellErrorMessage); }
                }
            }
            else { throw std::runtime_error(kHeaderCellErrorMessage); }
        }
        return data_table;
    }

    // Используется, если путь к файлу - это объект типа std::string
    template<size_t kColumnCount, typename ColumnT = std::string>
    DataTableArray<kColumnCount, ColumnT> ReadFormattedDataTableCSV(const std::string& file_path) {
        return ReadFormattedDataTableCSV<kColumnCount, ColumnT>(file_path.c_str());
    }

// List of Tables
    namespace tables {
        //typedef DataTableTuple<int, float> TableZero;
    }
// !List of Tables

}

#endif // !READDATATABLECSV_H