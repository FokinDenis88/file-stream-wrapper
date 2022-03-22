#ifndef READ_DATA_FILE_HPP
#define READ_DATA_FILE_HPP

#include <fstream>
#include <locale>
#include <type_traits>
#include <string>

#include "file-constants.hpp"
#include "file-service.hpp"

namespace file {

    // Реализация чтения текстовой информации из файл через оператор >>
    // Осторожно: Управляющие символы в разных операционных системах могут интерпретироваться по разному.
    // Ты читаешь из файла не всегда то, что записал
    // Чтение осуществляется сразу в basic_string<CharT>, без nullterminator
    // При вызове потока нужно Прописать все аргументы шаблона. Создать объект для каждого параметра функции
    // std::thread(ReadAllTextFromFile<wchar_t, std::wstring>, std::string("file_path"), OpenModeReadFromBegin, std::locale())
    // По возможности в поток в параметры проводить std::ref обертки на ссылки на аргументы
    template<typename CharT = char, typename ContentT>
    std::vector<ContentT> ReadDataFile(const char* file_path, std::ios_base::openmode open_mode = OpenModeReadFromBegin,
                            const std::locale& locale = std::locale()) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "CharT can be only: char or wchar_t");
        std::basic_ifstream<CharT> read_file_stream = OpenFile<std::basic_ifstream<CharT>>(file_path, open_mode);
        read_file_stream.imbue(locale);

        std::vector<ContentT> file_content{};
        {
            ContentT buffer{};
            while (read_file_stream.good()) {
                read_file_stream >> std::skipws >> buffer;
                file_content.emplace_back(buffer);
                buffer = "";
            }
        }
        if (!read_file_stream) { throw ErrorReadFile(); }

        CloseFile(read_file_stream);
        return file_content;
    }

    // Используется, если путь к файлу - это объект типа std::string
    template<typename CharT = char, typename ContentT>
    std::vector<ContentT> ReadDataFile(const std::string& file_path, std::ios_base::openmode open_mode = OpenModeReadFromBegin,
                          const std::locale& locale = std::locale()) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "CharT can be only: char or wchar_t");
        return ReadDataFile<CharT, ContentT>(file_path.c_str(), open_mode, locale);
    }

}

#endif // !READ_DATA_FILE_HPP