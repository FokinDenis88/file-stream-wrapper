#ifndef GETFILELINES_H
#define GETFILELINES_H

#include <fstream>
#include <locale>
#include <type_traits>
#include <string>
#include <vector>
#include <utility> // std::move

#include "file-constants.hpp"
#include "file-service.hpp"
//#include "ReadStreamState.h"

namespace file {

    // Erase \r char from readed file lines
    template <typename CharT = char>
    void EraseRChar(std::vector<std::basic_string<CharT>>& readed_data) {
        for (std::basic_string<CharT>& str : readed_data) {
            if (str.back() == '\r') { str.erase(--str.end()); }
        }
    }

    // Бинарное неформатированное чтение файла по строкам
    // If there is sequence of chars \r\n; \r will be stored in returned value
    template <typename CharT = char>
    std::vector<std::basic_string<CharT>>
    GetFileLines(std::basic_ifstream<CharT>& read_file_stream, bool delete_r = false,
                    const std::locale& locale = std::locale()) {
        static_assert(std::is_same<CharT, unsigned char>::value || std::is_same<CharT, char>::value,
                        "CharT can be only: unsigned char or char");
        read_file_stream.imbue(locale);

        SeekgFilePosEnd(read_file_stream);
        const auto end{ Tellg(read_file_stream) };
        SeekgFilePos(read_file_stream);
        std::vector<std::basic_string<CharT>> file_content{};
        {
            std::basic_string<CharT> buffer{};
            while ((Tellg(read_file_stream) != end) && getline(read_file_stream, buffer)) {
                // if \n is last char, there will be error of reading empty line, so needs to (Tellg(read_file_stream) != end)
                // getline Has no check to end of file at the start of its algorithm
                file_content.emplace_back(std::move(buffer)); // file_content.size() == 11
            }
        }
        if (!read_file_stream) { throw ErrorReadFile(); }

        if (delete_r) { EraseRChar(file_content); }
        return file_content;
    }
    

    // Бинарное неформатированное чтение файла по строкам
    // If there is sequence of chars \r\n; \r will be stored in returned value
    template <typename CharT = char>
    std::vector<std::basic_string<CharT>> 
    GetFileLines(const char* file_path, bool delete_r = false, 
                    const std::locale& locale = std::locale()) {
        static_assert(std::is_same<CharT, unsigned char>::value || std::is_same<CharT, char>::value,
                        "CharT can be only: unsigned char or char");
        std::basic_ifstream<CharT> read_file_stream = OpenFile<std::basic_ifstream<CharT>>(file_path, OpenModeReadBinary);
        std::vector<std::basic_string<CharT>> file_content(GetFileLines<CharT>(read_file_stream, delete_r, locale));
        CloseFile(read_file_stream);
        return file_content;
    }

    // Бинарное неформатированное чтение файла по строкам.
    // If there is sequence of chars \r\n; \r will be stored in returned value
    // delete_r option helps to delete additional char
    template <typename CharT = char>
    std::vector<std::basic_string<CharT>>
    GetFileLines(const std::string& file_path, bool delete_r = false,
                    const std::locale& locale = std::locale()) {
        static_assert(std::is_same<CharT, unsigned char>::value || std::is_same<CharT, char>::value,
                        "CharT can be only: unsigned char or char");
        return GetFileLines<CharT>(file_path.c_str(), delete_r, locale);
    }


    // Служебная функция, возвращает символ новой строки в char или wchar_t
    template<typename CharT = char>
    inline CharT NewLineChar() {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value ||
                      std::is_same<CharT, unsigned char>::value, "CharT can be only: char, unsigned char or wchar_t");
        return '\n';
    }

    // Служебная функция, возвращает символ новой строки в char или wchar_t
    template<>
    inline wchar_t NewLineChar<wchar_t>() {
        return L'\n';
    }

    // Бинарное неформатированное чтение файла по строкам и их конкатенация в одну строку
    // If there is sequence of chars \r\n; \r will be stored in returned value
    template <typename CharT = char>
    std::basic_string<CharT>
    GetFileInLine(const char* file_path, bool delete_r = false,
                  const std::locale& locale = std::locale()) {
        static_assert(std::is_same<CharT, unsigned char>::value || std::is_same<CharT, char>::value,
                        "CharT can be only: unsigned char or char");
        std::basic_ifstream<CharT> read_file_stream = OpenFile<std::basic_ifstream<CharT>>(file_path, OpenModeReadBinary);
        std::vector<std::basic_string<CharT>> file_content{ GetFileLines<CharT>(read_file_stream, delete_r, locale) };
        CloseFile(read_file_stream);
        std::basic_string<CharT> one_line{};
        for (auto i = file_content.cbegin(), imax = file_content.cend(); i != imax; ++i) {
            one_line.append(*i);
            one_line += NewLineChar<CharT>();
        }
        //if (file_content.size() > 0) { one_line.pop_back(); }

        return one_line;
    }

    // Бинарное неформатированное чтение файла по строкам и их конкатенация в одну строку
    // If there is sequence of chars \r\n; \r will be stored in returned value
    // delete_r option helps to delete additional char
    template <typename CharT = char>
    std::basic_string<CharT>
    GetFileInLine(const std::string& file_path, bool delete_r = false,
                  const std::locale& locale = std::locale()) {
        static_assert(std::is_same<CharT, unsigned char>::value || std::is_same<CharT, char>::value,
                        "CharT can be only: unsigned char or char");
        return GetFileInLine<CharT>(file_path.c_str(), delete_r, locale);
    }

}

#endif // !GETFILELINES_H