#ifndef READ_SERIALIZED_DATA_FILE_HPP_
#define READ_SERIALIZED_DATA_FILE_HPP_

#include <fstream>
#include <locale>
#include <type_traits>
#include <string>
#include <vector>

#include "file-constants.hpp"
//#include "file-service.hpp"

namespace file {

    // Бинарное чтение объекта из файла: ты читаешь из файла всегда то, что записал.
    template <typename ByteT = unsigned char>
    std::vector<ByteT> ReadSerializedDataFile(std::basic_ifstream<ByteT>& read_file_stream,
                                              std::ios_base::openmode open_mode = OpenModeReadBinary,
                                              const std::locale& locale = std::locale()) {
        static_assert(std::is_same<ByteT, unsigned char>::value || std::is_same<ByteT, std::byte>::value,
            "CharT can be only: unsigned char or std::byte");
        read_file_stream.imbue(locale);

        auto size = SizeOfFile<ByteT>(read_file_stream);
        std::vector<ByteT> file_content(size);
        if (size > 0) {
            if (read_file_stream.tellg() != 0) { // Переставляем позицию в файле на начало
                read_file_stream.seekg(0, std::ios_base::beg);    if (!(read_file_stream.good())) { throw ErrorFileOperation(); }
            }

            read_file_stream.read(&file_content[0], size);
            if (read_file_stream.bad()) { throw ErrorReadFile(); }
        }

        return file_content;
    }

    // Бинарное чтение объекта из файла: ты читаешь из файла всегда то, что записал.
    template <typename ByteT = unsigned char>
    std::vector<ByteT> ReadSerializedDataFile(const char* file_path, std::ios_base::openmode open_mode = OpenModeReadBinary,
                                              const std::locale& locale = std::locale()) {
        static_assert(std::is_same<ByteT, unsigned char>::value || std::is_same<ByteT, std::byte>::value,
            "CharT can be only: unsigned char or std::byte");
        std::basic_ifstream<ByteT> read_file_stream = OpenFile<std::basic_ifstream<ByteT>>(file_path, open_mode);
        std::vector<ByteT> file_content(ReadSerializedDataFile<ByteT>(read_file_stream, open_mode, locale));
        CloseFile(read_file_stream);
        return file_content;
    }

    // Бинарное чтение объекта из файла: ты читаешь из файла всегда то, что записал.
    template <typename ByteT = unsigned char>
    std::vector<ByteT> ReadSerializedDataFile(const std::string& file_path, std::ios_base::openmode open_mode = OpenModeReadBinary,
                                              const std::locale& locale = std::locale()) {
        static_assert(std::is_same<ByteT, unsigned char>::value || std::is_same<ByteT, std::byte>::value,
            "CharT can be only: unsigned char or std::byte");
        return ReadSerializedDataFile<ByteT>(file_path.c_str(), open_mode, locale);
    }

}

#endif // !READ_SERIALIZED_DATA_FILE_HPP_