#ifndef READSERIALIZEDDATAFILETHREAD_H
#define READSERIALIZEDDATAFILETHREAD_H

#include <fstream>
#include <locale>
#include <type_traits>
#include <string>
#include <vector>
#include <thread>
#include <future>

#include "FileConstants.h"
#include "FileService.h"

namespace file {

    // TODO: Может быть при чтении не нужно делать detach потока. Используется только join?
       // Запускает ReadTextFile в новом потоке. join - будет ли основной поток ждать файловый поток
   template<typename ByteT = unsigned char>
   inline void ReadSerializedDataFileNewThreadFunc(const char* file_path, std::promise<std::vector<ByteT>>& result,
                                                   std::ios_base::openmode open_mode = OpenModeReadBinary,
                                                   const std::locale& locale = std::locale()) {
       static_assert(std::is_same<ByteT, unsigned char>::value || std::is_same<ByteT, std::byte>::value,
           "CharT can be only: unsigned char or std::byte");
       result.set_value(ReadSerializedDataFile<ByteT>(file_path, open_mode, locale));
   }

    // TODO: Может быть при чтении не нужно делать detach потока. Используется только join?
    // Запускает ReadTextFile в новом потоке. join - будет ли основной поток ждать файловый поток
    template<typename ByteT = unsigned char>
    inline std::vector<ByteT>
    ReadSerializedDataFileNewThread(const char* file_path, std::ios_base::openmode open_mode = OpenModeReadBinary,
                                    const std::locale& locale = std::locale(), const bool join = true) {
       static_assert(std::is_same<ByteT, unsigned char>::value || std::is_same<ByteT, std::byte>::value,
           "CharT can be only: unsigned char or std::byte");
       std::promise<std::vector<ByteT>> result_promise;
       std::future<std::vector<ByteT>> result_futures = result_promise.get_future();
       std::thread read_file_thread(ReadSerializedDataFileNewThreadFunc<ByteT>, file_path, std::ref(result_promise), open_mode, locale);
       if (join) { read_file_thread.join(); }
       else { read_file_thread.detach(); }

       result_futures.wait();
       return result_futures.get();
   }

    // Запускает ReadTextFile в новом потоке. join - будет ли основной поток ждать файловый поток
    template<typename ByteT = unsigned char>
    inline std::vector<ByteT>
    ReadSerializedDataFileNewThread(const std::string& file_path, std::ios_base::openmode open_mode = OpenModeReadBinary,
                                    const std::locale& locale = std::locale(), const bool join = true) {
       static_assert(std::is_same<ByteT, unsigned char>::value || std::is_same<ByteT, std::byte>::value,
                        "CharT can be only: unsigned char or std::byte");
       return ReadSerializedDataFileNewThread<ByteT>(file_path.c_str(), open_mode, locale, join);
   }

}

#endif // !READSERIALIZEDDATAFILETHREAD_H