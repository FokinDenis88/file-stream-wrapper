#ifndef READTEXTFILE_H
#define READTEXTFILE_H

#include <fstream>
#include <locale>
#include <type_traits>
#include <string>

#include "file-constants.hpp"
#include "file-service.hpp"
#include "get-file-lines.hpp"

namespace file {

    //// Служебная функция, возвращает символ новой строки в char или wchar_t
    //template<typename CharT = char>
    //inline CharT NewLineChar() {
    //    static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "CharT can be only: char or wchar_t");
    //    return '\n';
    //}

    //// Служебная функция, возвращает символ новой строки в char или wchar_t
    //template<>
    //inline wchar_t NewLineChar<wchar_t>() {
    //    return L'\n';
    //}

    // Реализация чтения текстовой информации из файл через оператор >>
    // Осторожно: Управляющие символы в разных операционных системах могут интерпретироваться по разному.
    // Ты читаешь из файла не всегда то, что записал
    // Чтение осуществляется без nullterminator
    template<typename CharT = char>
    std::basic_string<CharT> ReadTextFile(const char* file_path, std::ios_base::openmode open_mode = OpenModeReadFromBegin,
                                          const std::locale& locale = std::locale()) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "CharT can be only: char or wchar_t");
        std::basic_ifstream<CharT> read_file_stream = OpenFile<std::basic_ifstream<CharT>>(file_path, open_mode);
        read_file_stream.imbue(locale);

        std::basic_string<CharT> file_content{};

        //GetFileLines(read_file_stream, true, locale);

        if (SizeOfFile<CharT>(read_file_stream) > 0) {
            SeekgFilePosEnd(read_file_stream);
            const auto end{ Tellg(read_file_stream) };
            SeekgFilePos(read_file_stream);
            std::basic_string<CharT> buffer{};
            while ((Tellg(read_file_stream) != end) && std::getline(read_file_stream, buffer)) {
                file_content += buffer;
                file_content += NewLineChar<CharT>();
            }
            // В конец содержимого записался один лишний символ конца строки
            //if (file_content.size() > 0) { file_content.pop_back(); }
            if (read_file_stream.fail()) { throw ErrorReadFile(); }
        }

        CloseFile(read_file_stream);
        return file_content;
    }

    // Используется, если путь к файлу - это объект типа std::string
    // Реализация чтения текстовой информации из файл через оператор >>
    // Осторожно: Управляющие символы в разных операционных системах могут интерпретироваться по разному.
    // Ты читаешь из файла не всегда то, что записал
    // Чтение осуществляется без nullterminator
    template<typename CharT = char>
    std::basic_string<CharT> ReadTextFile(const std::string& file_path, std::ios_base::openmode open_mode = OpenModeReadFromBegin,
                                          const std::locale& locale = std::locale()) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "CharT can be only: char or wchar_t");
        return ReadTextFile<CharT>(file_path.c_str(), open_mode, locale);
    }

}

#endif // !READTEXTFILE_H