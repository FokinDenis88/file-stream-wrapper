#ifndef WRITETEXTFILEBIGTHREAD_H
#define WRITETEXTFILEBIGTHREAD_H

#include <fstream>
#include <locale>
#include <type_traits>
#include <string>
#include <thread>

#include "FileConstants.h"
#include "FileService.h"

namespace file {

    // «апускает WriteDataFile в новом потоке. join - будет ли основной поток ждать файловый поток
    template<typename CharT = char>
    void WriteTextFileBigNewThread(const char* file_path, const std::vector<CharT>& file_content,
                                    std::ios_base::openmode open_mode = OpenModeWriteAppend,
                                    const std::locale& locale = std::locale(std::locale().classic(),
                                    new std::codecvt<CharT, char, mbstate_t>),
                                    const bool join = true) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "CharT can be only: char or wchar_t");
        void (*write_func_ptr)(const char*, const std::vector<CharT>&, std::ios_base::openmode, const std::locale&)
            = WriteTextFileBig<CharT>;
        std::thread write_file_thread(write_func_ptr, file_path, std::ref(file_content), open_mode, locale);
        if (join) { write_file_thread.join(); }
        else { write_file_thread.detach(); }
    }

    // «апускает WriteDataFile в новом потоке. join - будет ли основной поток ждать файловый поток
    template<typename CharT = char>
    void WriteTextFileBigNewThread(const std::string& file_path, const std::vector<CharT>& file_content,
                                    std::ios_base::openmode open_mode = OpenModeWriteAppend,
                                    const std::locale& locale = std::locale(std::locale().classic(),
                                    new std::codecvt<CharT, char, mbstate_t>),
                                    const bool join = true) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "CharT can be only: char or wchar_t");
        void (*write_func_ptr)(const std::string&, const std::vector<CharT>&, std::ios_base::openmode, const std::locale&)
            = WriteTextFileBig<CharT>;
        std::thread write_file_thread(write_func_ptr, std::ref(file_path), std::ref(file_content), open_mode, locale);
        if (join) { write_file_thread.join(); }
        else { write_file_thread.detach(); }
    }

}

#endif // !WRITETEXTFILEBIGTHREAD_H_H