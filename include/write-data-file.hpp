#ifndef WRITEDATAFILE_H
#define WRITEDATAFILE_H

#include <fstream>
#include <locale>
#include <type_traits>
#include <string>

#include "file-constants.hpp"
#include "file-service.hpp"

namespace file {

    // Реализация записи текстовой информации в файл через оператор <<
    // Осторожно: Управляющие символы в разных операционных системах могут интерпретироваться по разному.
    // Ты читаешь из файла не всегда то, что записал
    // По умолчанию открывает в режиме Append для добавление в конец файла
    // Если запись осуществляется с типом wchar_t, то и file_content Должен содержать литерал L"" или быть wchar_t
    // Конец массива символов nullterminator Не записывается
    // При вызове потока нужно Прописать все аргументы шаблона. Создать объект для каждого параметра функции
    // std::thread(WriteDataFile<wchar_t, std::wstring>, std::string("file_path"), std::wstring(L"file_data"), OpenModeWriteRewrite, std::locale())
    // По возможности в поток в параметры проводить std::ref обертки на ссылки на аргументы
    template<typename CharT = char, typename ContentT>
    void WriteDataFile(const char* file_path, const ContentT& file_content, std::ios_base::openmode open_mode = OpenModeWriteAppend,
                       const std::locale& locale = std::locale()) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "CharT can be only: char or wchar_t");
        std::basic_ofstream<CharT> write_file_stream = OpenFile<std::basic_ofstream<CharT>>(file_path, open_mode);
        write_file_stream.imbue(locale);

        // не пропускать white space - пробел std::noskipws
        write_file_stream << file_content;
        if (write_file_stream.fail()) { throw ErrorWriteFile(); }

        // Запись информации непосредственно в файл
        FlushFile(write_file_stream);
        CloseFile(write_file_stream);
    }

    // Используется, если путь к файлу - это объект типа std::string
    template<typename CharT = char, typename ContentT>
    void WriteDataFile(const std::string& file_path, const ContentT& file_content, std::ios_base::openmode open_mode = OpenModeWriteAppend,
                       const std::locale& locale = std::locale()) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "CharT can be only: char or wchar_t");
        WriteDataFile<CharT, ContentT>(file_path.c_str(), file_content, open_mode, locale);
    }

}

#endif // !WRITEDATAFILE_H