#ifndef READBINARYFILE_H
#define READBINARYFILE_H

#include <fstream>
#include <locale>
#include <type_traits>
#include <string>
#include <vector>

#include "file-constants.hpp"
#include "file-service.hpp"

namespace file {

    // Бинарное чтение файла: ты читаешь из файла всегда то, что записал.
    template <typename ByteT = char>
    std::vector<ByteT> ReadBinaryFile(std::basic_ifstream<ByteT>& read_file_stream, const std::locale& locale = std::locale()) {
        read_file_stream.imbue(locale);

        auto size = SizeOfFile<ByteT>(read_file_stream);
        std::vector<ByteT> file_content(size);
        if (size > 0) {
            if (Tellg(read_file_stream) != 0) { // Переставляем позицию в файле на начало
                SeekgFilePos(read_file_stream);
            }

            read_file_stream.read(&file_content[0], size);
            if (read_file_stream.bad()) { throw ErrorReadFile(); }
        }

        return file_content;
    }

    // Бинарное чтение файла: ты читаешь из файла всегда то, что записал.
    template <typename ByteT = char>
    std::vector<ByteT> ReadBinaryFile(const char* file_path, const std::locale& locale = std::locale()) {
        std::basic_ifstream<ByteT> read_file_stream = OpenFile<std::basic_ifstream<ByteT>>(file_path, OpenModeReadBinary);
        std::vector<ByteT> file_content(ReadBinaryFile<ByteT>(read_file_stream, locale));
        CloseFile(read_file_stream);
        return file_content;
    }

    // Бинарное чтение файла: ты читаешь из файла всегда то, что записал.
    template <typename ByteT = char>
    std::vector<ByteT> ReadBinaryFile(const std::string& file_path, const std::locale& locale = std::locale()) {
        return ReadBinaryFile<ByteT>(file_path.c_str(), locale);
    }

}

#endif // !READBINARYFILE_H