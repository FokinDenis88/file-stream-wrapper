#ifndef READSERIALIZEDDATAFILE_H
#define READSERIALIZEDDATAFILE_H

#include <fstream>
#include <locale>
#include <type_traits>
#include <string>
#include <vector>

#include "file-constants.hpp"

namespace file {

    // Бинарное чтение объекта из файла: ты читаешь из файла всегда то, что записал.
    template <typename ByteT = unsigned char>
    std::vector<ByteT> ReadSerializedDataFile(std::basic_ifstream<ByteT>& read_file_stream,
                                              std::ios_base::openmode open_mode = OpenModeReadBinary,
                                              const std::locale& locale = std::locale());

    // Бинарное чтение объекта из файла: ты читаешь из файла всегда то, что записал.
    template <typename ByteT = unsigned char>
    std::vector<ByteT> ReadSerializedDataFile(const char* file_path, std::ios_base::openmode open_mode = OpenModeReadBinary,
                                              const std::locale& locale = std::locale());

    // Бинарное чтение объекта из файла: ты читаешь из файла всегда то, что записал.
    template <typename ByteT = unsigned char>
    std::vector<ByteT> ReadSerializedDataFile(const std::string& file_path, std::ios_base::openmode open_mode = OpenModeReadBinary,
                                              const std::locale& locale = std::locale());

}

#endif // !READSERIALIZEDDATAFILE_H