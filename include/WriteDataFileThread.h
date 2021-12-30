#ifndef WRITEDATAFILETHREAD_H
#define WRITEDATAFILETHREAD_H

#include <fstream>
#include <locale>
#include <type_traits>
#include <string>
#include <thread>

#include "FileConstants.h"
#include "FileService.h"

namespace file {

    // «апускает WriteDataFile в новом потоке. join - будет ли основной поток ждать файловый поток
    template<typename CharT = char, typename ContentT>
    void WriteDataFileNewThread(const char* file_path, const ContentT& file_content, std::ios_base::openmode open_mode = OpenModeWriteAppend,
                                const std::locale& locale = std::locale(), const bool join = true) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "CharT can be only: char or wchar_t");
        void (*write_func_ptr)(const char*, const ContentT&, std::ios_base::openmode, const std::locale&) = WriteDataFile<CharT, ContentT>;
        std::thread write_file_thread(write_func_ptr, file_path, std::ref(file_content), open_mode, locale);
        if (join) { write_file_thread.join(); }
        else { write_file_thread.detach(); }
    }

    // «апускает WriteDataFile в новом потоке. join - будет ли основной поток ждать файловый поток
    template<typename CharT = char, typename ContentT>
    void WriteDataFileNewThread(const std::string& file_path, const ContentT& file_content, std::ios_base::openmode open_mode = OpenModeWriteAppend,
                                const std::locale& locale = std::locale(), const bool join = true) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "CharT can be only: char or wchar_t");
        void (*write_func_ptr)(const std::string&, const ContentT&, std::ios_base::openmode, const std::locale&) = WriteDataFile<CharT, ContentT>;
        std::thread write_file_thread(write_func_ptr, std::ref(file_path), std::ref(file_content), open_mode, locale);
        if (join) { write_file_thread.join(); }
        else { write_file_thread.detach(); }
    }

}

#endif // !WRITEDATAFILETHREAD_H