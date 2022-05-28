#ifndef READTEXTFILETHREAD_H
#define READTEXTFILETHREAD_H

#include <fstream>
#include <locale>
#include <type_traits>
#include <string>
#include <thread>
#include <future>

#include "file-constants.hpp"
#include "file-service.hpp"

namespace file {

    // TODO: Может быть при чтении не нужно делать detach потока. Используется только join?
    // Запускает ReadTextFile в новом потоке. join - будет ли основной поток ждать файловый поток
    template<typename CharT = char>
    void ReadTextFileNewThreadFunc(const char* file_path, std::promise<std::basic_string<CharT>>& result,
                                    std::ios_base::openmode open_mode = OpenModeReadFromBegin,
                                    const std::locale& locale = std::locale()) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "CharT can be only: char or wchar_t");
        result.set_value(ReadTextFile<CharT>(file_path, open_mode, locale));
    }

    // TODO: Может быть при чтении не нужно делать detach потока. Используется только join?
    // Запускает ReadTextFile в новом потоке. join - будет ли основной поток ждать файловый поток
    template<typename CharT = char>
    std::basic_string<CharT> ReadTextFileNewThread(const char* file_path, std::ios_base::openmode open_mode = OpenModeReadFromBegin,
                                                   const std::locale& locale = std::locale(), const bool join = true) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "CharT can be only: char or wchar_t");
        std::promise<std::basic_string<CharT>> result_promise;
        std::future<std::basic_string<CharT>> result_futures = result_promise.get_future();
        std::thread read_file_thread(ReadTextFileNewThreadFunc<CharT>, file_path, std::ref(result_promise), open_mode, locale);
        if (join) { read_file_thread.join(); }
        else { read_file_thread.detach(); }

        result_futures.wait();
        return result_futures.get();
    }

    // Запускает ReadTextFile в новом потоке. join - будет ли основной поток ждать файловый поток
    template<typename CharT = char>
    std::basic_string<CharT> ReadTextFileNewThread(const std::string& file_path, std::ios_base::openmode open_mode = OpenModeReadFromBegin,
                                                   const std::locale& locale = std::locale(), const bool join = true) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "CharT can be only: char or wchar_t");
        std::promise<std::basic_string<CharT>> result_promise;
        std::future<std::basic_string<CharT>> result_futures = result_promise.get_future();
        std::thread read_file_thread(ReadTextFileNewThreadFunc<CharT>, file_path.c_str(), std::ref(result_promise),
            open_mode, locale);
        if (join) { read_file_thread.join(); }
        else { read_file_thread.detach(); }

        result_futures.wait();
        return result_futures.get();
    }

}

#endif // !READTEXTFILETHREAD_H