#ifndef READTEXTFILEBIG_H
#define READTEXTFILEBIG_H

#include <fstream>
#include <locale>
#include <type_traits>
#include <string>

#include "file-constants.hpp"
#include "file-service.hpp"

namespace file {

    // wchar_t плохо работает, лучше не использовать
    // Реализация чтения БОЛЬШИХ ОБЪЁМОВ текстовой информации из файл через оператор >>
    // Осторожно: Управляющие символы в разных операционных системах могут интерпретироваться по разному.
    // Ты читаешь из файла не всегда то, что записал
    // Чтение осуществляется без nullterminator
    // Чтение wchar_t может искажать содержимое файла. Например, файл длинной 45 байт преобразуется в вектор 23 элементов
    // В 23 элементе будет значение \x00ee - 00 это лишний не нужный байт, который искажает содержимое
    template<typename CharT = char>
    std::vector<CharT> ReadTextFileBig(const char* file_path, std::ios_base::openmode open_mode = OpenModeReadBinary,
                                       const std::locale& locale = std::locale(std::locale().classic(), 
                                       new std::codecvt<CharT, char, mbstate_t>)) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "CharT can be only: char or wchar_t");
        std::basic_ifstream<char> read_file_stream = OpenFile<std::basic_ifstream<char>>(file_path, open_mode);
        read_file_stream.imbue(locale);

        auto size_of_file = SizeOfFile<CharT>(read_file_stream);
        long long file_content_size{};
        { // Если у нас 45 байт, то выделить нужно 23 байта wchar_t
            std::lldiv_t div_res{ div(size_of_file, sizeof(CharT)) };
            if (div_res.rem == 0) { file_content_size = div_res.quot; }
            else { file_content_size = div_res.quot + 1; }
        }
        std::vector<CharT> file_content(file_content_size);
        if (size_of_file > 0) { // Ставим позицию чтения в файле на начало
            SeekgFilePos(read_file_stream);

            read_file_stream.read(reinterpret_cast<char*>(&file_content[0]), size_of_file);
            if (read_file_stream.bad()) { throw ErrorReadFile(); }
        }

        CloseFile(read_file_stream);
        return file_content;
    }

    // wchar_t плохо работает, лучше не использовать
    // Используется, если путь к файлу - это объект типа std::string
    // Реализация чтения текстовой информации из файл через оператор >>
    // Осторожно: Управляющие символы в разных операционных системах могут интерпретироваться по разному.
    // Ты читаешь из файла не всегда то, что записал
    // Чтение осуществляется без nullterminator
    template<typename CharT = char>
    std::vector<CharT> ReadTextFileBig(const std::string& file_path,
                                    std::ios_base::openmode open_mode = OpenModeReadBinary,
                                    const std::locale& locale = std::locale(std::locale().classic(),
                                    new std::codecvt<CharT, char, mbstate_t>)) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "CharT can be only: char or wchar_t");
        return std::move(ReadTextFileBig<CharT>(file_path.c_str(), open_mode, locale));
    }

    // Читает файлы больших размеров. Не работает нормально. Желательно не использовать
    // Реализация чтения БОЛЬШИХ ОБЪЁМОВ текстовой информации из файл через оператор >>
    // Осторожно: Управляющие символы в разных операционных системах могут интерпретироваться по разному.
    // Ты читаешь из файла не всегда то, что записал
    // Чтение осуществляется без nullterminator
    template<typename CharT = char>
    [[deprecated]]
    std::vector<std::basic_string<CharT>> ReadTextFileBigLines(const char* file_path, std::ios_base::openmode open_mode = OpenModeReadFromBegin,
                                                               const std::locale& locale = std::locale()) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "CharT can be only: char or wchar_t");
        std::basic_ifstream<CharT> read_file_stream = OpenFile<std::basic_ifstream<CharT>>(file_path, open_mode);
        read_file_stream.imbue(locale);

        std::vector<std::basic_string<CharT>> file_content{};
        if (SizeOfFile<CharT>(read_file_stream) > 0) {
            {
                std::basic_string<CharT> buffer{};
                std::basic_string<CharT> vector_elem{};
                auto max_string_size = vector_elem.max_size();
                do {
                    while (read_file_stream.good()) {
                        std::getline(read_file_stream, buffer);
                        vector_elem += buffer;
                        vector_elem += NewLineChar<CharT>();
                        int a = 0;
                    } //26719608
                    if (vector_elem.size() != max_string_size) { // если не было переполнения vector_elem
                        // В конец содержимого записался один лишний символ конца строки
                        if (vector_elem.size() > 0) { vector_elem.pop_back(); }
                        file_content.emplace_back(vector_elem);
                    }
                    else { // если было переполнение vector_elem
                        file_content.emplace_back(vector_elem);
                        vector_elem.erase();
                        if (!read_file_stream.eof()) { read_file_stream.clear(); }
                    }
                } while (read_file_stream.good());
            }
            if (read_file_stream.fail()) { throw ErrorReadFile(); }
        }

        CloseFile(read_file_stream);
        return file_content;
    }

}

#endif // !READTEXTFILEBIG_H