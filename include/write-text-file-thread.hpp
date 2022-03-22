#ifndef WRITETEXTFILETHREAD_H
#define WRITETEXTFILETHREAD_H

#include <fstream>
#include <locale>
#include <type_traits>
#include <string>
#include <thread>

#include "file-constants.hpp"
#include "file-service.hpp"

namespace file {

    // «апускает WriteDataFile в новом потоке. join - будет ли основной поток ждать файловый поток
    template<typename CharT = char>
    void WriteTextFileNewThread(const char* file_path, const std::basic_string<CharT>& file_content,
                                std::ios_base::openmode open_mode = OpenModeWriteAppend,
                                const std::locale& locale = std::locale(), const bool join = true) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "CharT can be only: char or wchar_t");
        void (*write_func_ptr)(const char*, const std::basic_string<CharT>&, std::ios_base::openmode, const std::locale&)
            = WriteTextFile<CharT>;
        std::thread write_file_thread(write_func_ptr, file_path, std::ref(file_content), open_mode, locale);
        if (join) { write_file_thread.join(); }
        else { write_file_thread.detach(); }
    }

    // «апускает WriteDataFile в новом потоке. join - будет ли основной поток ждать файловый поток
    template<typename CharT = char>
    void WriteTextFileNewThread(const std::string& file_path, const std::basic_string<CharT>& file_content,
                                std::ios_base::openmode open_mode = OpenModeWriteAppend,
                                const std::locale& locale = std::locale(), const bool join = true) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "CharT can be only: char or wchar_t");
        void (*write_func_ptr)(const std::string&, const std::basic_string<CharT>&, std::ios_base::openmode, const std::locale&)
            = WriteTextFile<CharT>;
        std::thread write_file_thread(write_func_ptr, std::ref(file_path), std::ref(file_content), open_mode, locale);
        if (join) { write_file_thread.join(); }
        else { write_file_thread.detach(); }
    }

}

#endif // !WRITETEXTFILETHREAD_H