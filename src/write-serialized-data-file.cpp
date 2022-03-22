#include "write-serialized-data-file.hpp"

#include "file-service.hpp"

// write, read files, another service operations based on fstream
namespace file {

    template<typename ByteT>
    void WriteSerializedDataFile(std::basic_ofstream<ByteT>& write_file_stream,
                                 const void* first_object_symbol, const std::streamsize object_size,
                                 std::ios_base::openmode open_mode,
                                 const std::locale& locale) { // open_mode & locale is defined as default in forward declaration
        static_assert(std::is_same<ByteT, unsigned char>::value || std::is_same<ByteT, std::byte>::value,
                        "ByteT can be only: unsigned char or std::byte");
        write_file_stream.imbue(locale);

        if (object_size > 0) { // wofstream не будет работать, если '\x0101' в старшем байте не будут 00 = '\x0001'
            write_file_stream.write(reinterpret_cast<const ByteT*>(first_object_symbol), object_size);
            if (write_file_stream.bad()) { throw ErrorWriteFile(); }

            // Запись информации непосредственно в файл
            FlushFile(write_file_stream);
        }
    }
    template void WriteSerializedDataFile<unsigned char>(std::basic_ofstream<unsigned char>&, const void*,
                                                         const std::streamsize, std::ios_base::openmode open_mode,
                                                         const std::locale& locale);
    template void WriteSerializedDataFile<std::byte>(std::basic_ofstream<std::byte>&, const void*,
                                                     const std::streamsize, std::ios_base::openmode open_mode,
                                                     const std::locale& locale);


    template<typename ByteT>
    void WriteSerializedDataFile(const char* file_path, const void* first_object_symbol, const std::streamsize object_size,
                                 std::ios_base::openmode open_mode,
                                 const std::locale& locale) { // open_mode & locale is defined as default in forward declaration
        static_assert(std::is_same<ByteT, unsigned char>::value || std::is_same<ByteT, std::byte>::value,
                        "ByteT can be only: unsigned char or std::byte");
        std::basic_ofstream<ByteT> write_file_stream = OpenFile<std::basic_ofstream<ByteT>>(file_path, open_mode);
        WriteSerializedDataFile<ByteT>(write_file_stream, first_object_symbol, object_size, open_mode, locale);
        CloseFile(write_file_stream);
    }
    template void WriteSerializedDataFile<unsigned char>(const char* file_path, const void* first_object_symbol, 
                                                         const std::streamsize object_size,
                                                         std::ios_base::openmode open_mode, const std::locale& locale);
    template void WriteSerializedDataFile<std::byte>(const char* file_path, const void* first_object_symbol,
                                                     const std::streamsize object_size,
                                                     std::ios_base::openmode open_mode, const std::locale& locale);


    template<typename ByteT>
    void WriteSerializedDataFile(const std::string& file_path, const void* first_object_symbol, const std::streamsize object_size,
                                 std::ios_base::openmode open_mode,
                                 const std::locale& locale) {
        static_assert(std::is_same<ByteT, unsigned char>::value || std::is_same<ByteT, std::byte>::value,
                        "ByteT can be only: char or std::byte");
        if (object_size > 0) {
            WriteSerializedDataFile<ByteT>(file_path.c_str(), first_object_symbol, object_size, open_mode, locale);
        }
    }
    template void WriteSerializedDataFile<unsigned char>(const std::string& file_path, const void* first_object_symbol, 
                                                         const std::streamsize object_size, std::ios_base::openmode open_mode,
                                                         const std::locale& locale);
    template void WriteSerializedDataFile<std::byte>(const std::string& file_path, const void* first_object_symbol,
                                                     const std::streamsize object_size, std::ios_base::openmode open_mode,
                                                     const std::locale& locale);


    template<typename ByteT>
    void WriteSerializedDataFile(std::basic_ofstream<unsigned char>& write_file_stream, const std::vector<ByteT>& data,
                                 std::ios_base::openmode open_mode,
                                 const std::locale& locale) {
        static_assert(std::is_same<ByteT, unsigned char>::value || std::is_same<ByteT, wchar_t>::value,
                        "ByteT can be only: char, wchar_t");
        if (data.size() > 0) {
            WriteSerializedDataFile<unsigned char>(write_file_stream, &data[0], data.size() * sizeof(ByteT), open_mode, locale);
        }
    }
    void WriteSerializedDataFile(std::basic_ofstream<std::byte>& write_file_stream, const std::vector<std::byte>& data,
        std::ios_base::openmode open_mode,
        const std::locale& locale) {
        if (data.size() > 0) {
            WriteSerializedDataFile<std::byte>(write_file_stream, &data[0], data.size() * sizeof(std::byte), open_mode, locale);
        }
    }
    template void WriteSerializedDataFile<unsigned char>(std::basic_ofstream<unsigned char>& write_file_stream, 
                                                         const std::vector<unsigned char>& data,
                                                         std::ios_base::openmode open_mode,
                                                         const std::locale& locale);
    template void WriteSerializedDataFile<wchar_t>(std::basic_ofstream<unsigned char>& write_file_stream,
                                                   const std::vector<wchar_t>& data,
                                                   std::ios_base::openmode open_mode,
                                                   const std::locale& locale);


    template<typename ByteT>
    void WriteSerializedDataFile(const char* file_path, const std::vector<ByteT>& data,
                                 std::ios_base::openmode open_mode,
                                 const std::locale& locale) {
        static_assert(std::is_same<ByteT, unsigned char>::value || std::is_same<ByteT, wchar_t>::value
                        || std::is_same<ByteT, std::byte>::value, "ByteT can be only: char, wchar_t or std::byte");
        if (data.size() > 0) {
            if (std::is_same<ByteT, std::byte>::value) { // Если не нужно конвертировать wchar_t
                WriteSerializedDataFile(file_path, &data[0], data.size() * sizeof(std::byte), open_mode, locale);
            }
            else {
                WriteSerializedDataFile<unsigned char>(file_path, &data[0], data.size() * sizeof(ByteT), open_mode, locale);
            }
        }
    }
    template void WriteSerializedDataFile<unsigned char>(const char* file_path, const std::vector<unsigned char>& data,
                                                         std::ios_base::openmode open_mode,
                                                         const std::locale& locale);
    template void WriteSerializedDataFile<wchar_t>(const char* file_path, const std::vector<wchar_t>& data,
                                                   std::ios_base::openmode open_mode,
                                                   const std::locale& locale);
    template void WriteSerializedDataFile<std::byte>(const char* file_path, const std::vector<std::byte>& data,
                                                     std::ios_base::openmode open_mode,
                                                     const std::locale& locale);


    template<typename ByteT>
    void WriteSerializedDataFile(const std::string& file_path, const std::vector<ByteT>& data,
                                 std::ios_base::openmode open_mode,
                                 const std::locale& locale) {
        static_assert(std::is_same<ByteT, unsigned char>::value || std::is_same<ByteT, wchar_t>::value
                        || std::is_same<ByteT, std::byte>::value, "ByteT can be only: char, wchar_t or std::byte");
        if (data.size() > 0) {
            if (std::is_same<ByteT, std::byte>::value) { // Если не нужно конвертировать wchar_t
                WriteSerializedDataFile(file_path.c_str(), &data[0], data.size() * sizeof(ByteT), open_mode, locale);
            }
            else {
                WriteSerializedDataFile<unsigned char>(file_path.c_str(), &data[0], data.size() * sizeof(ByteT), open_mode, locale);
            }
        }
    }
    template void WriteSerializedDataFile<unsigned char>(const std::string& file_path, const std::vector<unsigned char>& data,
                                                         std::ios_base::openmode open_mode,
                                                         const std::locale& locale);
    template void WriteSerializedDataFile<wchar_t>(const std::string& file_path, const std::vector<wchar_t>& data,
                                                   std::ios_base::openmode open_mode,
                                                   const std::locale& locale);
    template void WriteSerializedDataFile<std::byte>(const std::string& file_path, const std::vector<std::byte>& data,
                                                     std::ios_base::openmode open_mode,
                                                     const std::locale& locale);

}