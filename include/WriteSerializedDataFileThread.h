#ifndef WRITESERIALIZEDDATAFILETHREAD_H
#define WRITESERIALIZEDDATAFILETHREAD_H

#include <fstream>
#include <locale>
#include <type_traits>
#include <string>
#include <thread>

#include "FileConstants.h"
#include "FileService.h"
#include "WriteSerializedDataFile.h"

namespace file {

    // «апускает WriteDataFile в новом потоке. join - будет ли основной поток ждать файловый поток
    template <typename ByteT = unsigned char>
    inline void WriteSerializedDataFileNewThread(const char* file_path, const void* first_object_symbol, const std::streamsize object_size,
                                                std::ios_base::openmode open_mode = OpenModeWriteBinaryAppend,
                                                const std::locale& locale = std::locale(), const bool join = true) {
        static_assert(std::is_same<ByteT, unsigned char>::value || std::is_same<ByteT, std::byte>::value,
            "ByteT can be only: char or std::byte");
        void (*write_func_ptr)(const char*, const void*, const std::streamsize, std::ios_base::openmode, const std::locale&)
            = WriteSerializedDataFile<ByteT>;
        std::thread write_file_thread(write_func_ptr, file_path, first_object_symbol, object_size, open_mode, locale);
        if (join) { write_file_thread.join(); }
        else { write_file_thread.detach(); }
    }

    // «апускает WriteDataFile в новом потоке. join - будет ли основной поток ждать файловый поток
    template <typename ByteT = unsigned char>
    inline void WriteSerializedDataFileNewThread(const std::string& file_path, const void* first_object_symbol, const std::streamsize object_size,
                                                std::ios_base::openmode open_mode = OpenModeWriteBinaryAppend,
                                                const std::locale& locale = std::locale(), const bool join = true) {
        static_assert(std::is_same<ByteT, unsigned char>::value || std::is_same<ByteT, std::byte>::value,
            "ByteT can be only: char or std::byte");
        void (*write_func_ptr)(const std::string&, const void*, const std::streamsize, std::ios_base::openmode, const std::locale&)
            = WriteSerializedDataFile<ByteT>;
        std::thread write_file_thread(write_func_ptr, std::ref(file_path), first_object_symbol, object_size, open_mode, locale);
        if (join) { write_file_thread.join(); }
        else { write_file_thread.detach(); }
    }

    // «апускает WriteDataFile в новом потоке. join - будет ли основной поток ждать файловый поток
    template<typename ByteT = unsigned char>
    void WriteSerializedDataFileNewThread(const char* file_path, const std::vector<ByteT>& data,
                                        std::ios_base::openmode open_mode = OpenModeWriteBinaryAppend,
                                        const std::locale& locale = std::locale(), const bool join = true) {
        static_assert(std::is_same<ByteT, unsigned char>::value || std::is_same<ByteT, wchar_t>::value
            || std::is_same<ByteT, std::byte>::value, "ByteT can be only: char, wchar_t or std::byte");
        void (*write_func_ptr)(const char*, const std::vector<ByteT>&, std::ios_base::openmode, const std::locale&)
            = WriteSerializedDataFile<ByteT>;
        std::thread write_file_thread(write_func_ptr, file_path, std::ref(data), open_mode, locale);
        if (join) { write_file_thread.join(); }
        else { write_file_thread.detach(); }
    }

    // «апускает WriteDataFile в новом потоке. join - будет ли основной поток ждать файловый поток
    template<typename ByteT = unsigned char>
    void WriteSerializedDataFileNewThread(const std::string& file_path, const std::vector<ByteT>& data,
                                            std::ios_base::openmode open_mode = OpenModeWriteBinaryAppend,
                                            const std::locale& locale = std::locale(), const bool join = true) {
        static_assert(std::is_same<ByteT, unsigned char>::value || std::is_same<ByteT, wchar_t>::value
            || std::is_same<ByteT, std::byte>::value, "ByteT can be only: char, wchar_t or std::byte");
        void (*write_func_ptr)(const std::string&, const std::vector<ByteT>&, std::ios_base::openmode, const std::locale&)
            = WriteSerializedDataFile<ByteT>;
        std::thread write_file_thread(write_func_ptr, std::ref(file_path), std::ref(data), open_mode, locale);
        if (join) { write_file_thread.join(); }
        else { write_file_thread.detach(); }
    }

}

#endif // !WRITESERIALIZEDDATAFILETHREAD_H