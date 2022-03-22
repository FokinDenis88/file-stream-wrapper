#ifndef READDATAFILETHREAD_H
#define READDATAFILETHREAD_H

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
    // Запускает ReadAllTextFromFile в новом потоке. join - будет ли основной поток ждать файловый поток
    template<typename CharT = char, typename ContentT>
    void ReadDataFileNewThreadFunc(const char* file_path, std::promise<ContentT>& result,
                                    std::ios_base::openmode open_mode = OpenModeReadFromBegin,
                                    const std::locale& locale = std::locale()) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "CharT can be only: char or wchar_t");
        result.set_value(ReadTextFile<CharT, ContentT>(file_path, open_mode, locale));
    }

    // TODO: Может быть при чтении не нужно делать detach потока. Используется только join?
    // Запускает ReadAllTextFromFile в новом потоке. join - будет ли основной поток ждать файловый поток
    template<typename CharT = char, typename ContentT>
    ContentT ReadDataFileNewThread(const char* file_path, std::ios_base::openmode open_mode = OpenModeReadFromBegin,
                                    const std::locale& locale = std::locale(), const bool join = true) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "CharT can be only: char or wchar_t");
        std::promise<ContentT> result_promise;
        std::future<ContentT> result_futures = result_promise.get_future();
        std::thread read_file_thread(ReadDataFileNewThreadFunc<CharT, ContentT>, file_path, std::ref(result_promise), open_mode, locale);
        if (join) { read_file_thread.join(); }
        else { read_file_thread.detach(); }

        result_futures.wait();
        return result_futures.get();
    }

    // Запускает ReadAllTextFromFile в новом потоке. join - будет ли основной поток ждать файловый поток
    template<typename CharT = char, typename ContentT>
    ContentT ReadDataFileNewThread(const std::string& file_path, std::ios_base::openmode open_mode = OpenModeReadFromBegin,
                                    const std::locale& locale = std::locale(), const bool join = true) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "CharT can be only: char or wchar_t");
        std::promise<ContentT> result_promise;
        std::future<ContentT> result_futures = result_promise.get_future();
        std::thread read_file_thread(ReadDataFileNewThreadFunc<CharT, ContentT>, file_path.c_str(), std::ref(result_promise),
            open_mode, locale);
        if (join) { read_file_thread.join(); }
        else { read_file_thread.detach(); }

        result_futures.wait();
        return result_futures.get();
    }

}

#endif // !READDATAFILETHREAD_H