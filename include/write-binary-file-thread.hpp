#ifndef WRITEBINARYFILETHREAD_H
#define WRITEBINARYFILETHREAD_H

#include <fstream>
#include <locale>
#include <type_traits>
#include <string>
#include <thread>

#include "file-constants.hpp"
#include "file-service.hpp"
#include "write-serialized-data-file.hpp"

namespace file {

    // «апускает WriteDataFile в новом потоке. join - будет ли основной поток ждать файловый поток
    template <typename ByteT = char>
    inline void WriteBinaryFileThread(const char* file_path, const void* first_object_symbol, const std::streamsize object_size,
                                    std::ios_base::openmode open_mode = OpenModeWriteBinaryAppend,
                                    const std::locale& locale = std::locale(), const bool join = true) {
        void (*write_func_ptr)(const char*, const void*, const std::streamsize, std::ios_base::openmode, const std::locale&)
                                                                                                        = WriteBinaryFile<ByteT>;
        std::thread write_file_thread(write_func_ptr, file_path, first_object_symbol, object_size, open_mode, locale);
        if (join) { write_file_thread.join(); }
        else { write_file_thread.detach(); }
    }

    // «апускает WriteDataFile в новом потоке. join - будет ли основной поток ждать файловый поток
    template <typename ByteT = char>
    inline void WriteBinaryFileThread(const std::string& file_path, const void* first_object_symbol, const std::streamsize object_size,
                                    std::ios_base::openmode open_mode = OpenModeWriteBinaryAppend,
                                    const std::locale& locale = std::locale(), const bool join = true) {
        void (*write_func_ptr)(const std::string&, const void*, const std::streamsize, std::ios_base::openmode, const std::locale&)
                                                                                                        = WriteBinaryFile<ByteT>;
        std::thread write_file_thread(write_func_ptr, std::ref(file_path), first_object_symbol, object_size, open_mode, locale);
        if (join) { write_file_thread.join(); }
        else { write_file_thread.detach(); }
    }

    // «апускает WriteDataFile в новом потоке. join - будет ли основной поток ждать файловый поток
    template<typename ByteT = char>
    void WriteBinaryFileThread(const char* file_path, const std::vector<ByteT>& data,
                                std::ios_base::openmode open_mode = OpenModeWriteBinaryAppend,
                                const std::locale& locale = std::locale(), const bool join = true) {
        void (*write_func_ptr)(const char*, const std::vector<ByteT>&, std::ios_base::openmode, const std::locale&)
                                                                                            = WriteBinaryFile<ByteT>;
        std::thread write_file_thread(write_func_ptr, file_path, std::ref(data), open_mode, locale);
        if (join) { write_file_thread.join(); }
        else { write_file_thread.detach(); }
    }

    // «апускает WriteDataFile в новом потоке. join - будет ли основной поток ждать файловый поток
    template<typename ByteT = char>
    void WriteBinaryFileThread(const std::string& file_path, const std::vector<ByteT>& data,
                                std::ios_base::openmode open_mode = OpenModeWriteBinaryAppend,
                                const std::locale& locale = std::locale(), const bool join = true) {
        void (*write_func_ptr)(const std::string&, const std::vector<ByteT>&, std::ios_base::openmode, const std::locale&)
                                                                                            = WriteBinaryFile<ByteT>;
        std::thread write_file_thread(write_func_ptr, std::ref(file_path), std::ref(data), open_mode, locale);
        if (join) { write_file_thread.join(); }
        else { write_file_thread.detach(); }
    }
}

#endif // !WRITEBINARYFILETHREAD_H