#ifndef FILESERVICE_H
#define FILESERVICE_H

#include <fstream>
#include <vector>
#include <type_traits>

#include "file-constants.hpp"
#include "errors-handler-file-operations.hpp"

namespace file {
    // Forward Declaration Section

    template<typename ByteT>
    inline void WriteSerializedDataFile(const char* file_path, const void* first_object_symbol, const std::streamsize object_size,
                                        std::ios_base::openmode open_mode,
                                        const std::locale& locale);


    // Безопасно открывает файл ассоциированный с файловым потоком. Внимание: Оставлияет поток открытым
    template<typename FileStreamT = std::ifstream>
    inline void OpenFile(FileStreamT& file_stream, const std::string& file_path, std::ios_base::openmode open_mode = OpenModeReadFromBegin) {
        file_stream.open(file_path, open_mode);
        // !write_file_stream - перегружает оператор. Эквивалентен fail()
        if (file_stream.fail() || !(file_stream.is_open())) { throw ErrorOpenFile(); }
    }

    // Безопасно открывает файл ассоциированный с файловым потоком
    template<typename FileStreamT = std::ifstream>
    inline FileStreamT OpenFile(const std::string& file_path, std::ios_base::openmode open_mode = OpenModeReadFromBegin) {
        FileStreamT file_stream{};
        OpenFile<FileStreamT>(file_stream, file_path, open_mode);
        return file_stream;
    }

    // Безопасно закрывает файл ассоциированный с файловым потоком
    template<typename FileStreamT = std::ifstream>
    inline void CloseFile(FileStreamT& file_stream) {
        if (file_stream.is_open()) {
            file_stream.close();
            if (file_stream.bad()) { throw ErrorCloseFile(); }
        }
    }

    // Существует ли файл по пути path
    inline bool FileExists(const std::string& path) {
        std::ifstream read_file(path);
        if (read_file) {
            CloseFile(read_file);
            return true;
        }
        else { return false; }
    }

    // Переставляем позицию чтения в файле. No checks for file_stream is_open.
    template<typename FileStreamT = std::ifstream>
    inline void SeekgFilePos(FileStreamT& file_stream, long pos) {
        if (file_stream.tellg() != pos) { // Переставляем позицию в файле по абсолютному значению позиции в файле
            file_stream.seekg(pos);    if (!(file_stream.good())) { throw ErrorFileOperation(); }
        }
    }

    // Переставляем позицию чтения в файле. No checks for file_stream is_open.
    template<typename FileStreamT = std::ifstream>
    inline void SeekgFilePos(FileStreamT& file_stream, long off = 0, std::ios_base::seekdir dir = std::ios_base::beg) {
        if (file_stream.tellg() != dir + off) { // Переставляем позицию в файле на позицию dir с учетом сдвига off
            file_stream.seekg(off, dir);    if (!(file_stream.good())) { throw ErrorFileOperation(); }
        }
    }
    template<typename FileStreamT = std::ifstream>
    inline void SeekgFilePosEnd(FileStreamT& file_stream, long off = 0, std::ios_base::seekdir dir = std::ios_base::end) {
        if (file_stream.tellg() != dir + off) { // Переставляем позицию в файле на позицию dir с учетом сдвига off
            file_stream.seekg(off, dir);    if (!(file_stream.good())) { throw ErrorFileOperation(); }
        }
    }

    // Переставляем позицию чтения в файле. No checks for file_stream is_open.
    template<typename FileStreamT = std::ifstream>
    inline void SeekpFilePos(FileStreamT& file_stream, long pos) {
        if (file_stream.tellp() != pos) { // Переставляем позицию в файле по абсолютному значению позиции в файле
            file_stream.seekp(pos);    if (!(file_stream.good())) { throw ErrorFileOperation(); }
        }
    }

    // Переставляем позицию чтения в файле. No checks for file_stream is_open.
    template<typename FileStreamT = std::ifstream>
    inline void SeekpFilePos(FileStreamT& file_stream, long off = 0, std::ios_base::seekdir dir = std::ios_base::beg) {
        if (file_stream.tellp() != dir + off) { // Переставляем позицию в файле на позицию dir с учетом сдвига off
            file_stream.seekp(off, dir);    if (!(file_stream.good())) { throw ErrorFileOperation(); }
        }
    }
    // Переставляем позицию чтения в файле. No checks for file_stream is_open.
    template<typename FileStreamT = std::ifstream>
    inline void SeekpFilePosEnd(FileStreamT& file_stream, long off = 0, std::ios_base::seekdir dir = std::ios_base::end) {
        if (file_stream.tellp() != dir + off) { // Переставляем позицию в файле на позицию dir с учетом сдвига off
            file_stream.seekp(off, dir);    if (!(file_stream.good())) { throw ErrorFileOperation(); }
        }
    }

    // Tells get position in file
    template<typename CharT = char, typename FileStreamT = std::ifstream>
    inline std::fpos<typename std::char_traits<CharT>::state_type> 
    Tellg(FileStreamT& file_stream) {
        std::fpos<typename std::char_traits<CharT>::state_type> pos{ file_stream.tellg() };
        if (!file_stream) { throw ErrorFileOperation(); }
        return pos;
    }

    // Tells put position in file
    template<typename CharT = char, typename FileStreamT = std::ofstream>
    inline std::fpos<typename std::char_traits<CharT>::state_type>
    Tellp(FileStreamT& file_stream) {
        std::fpos<typename std::char_traits<CharT>::state_type> pos{ file_stream.tellp() };
        if (!file_stream) { throw ErrorFileOperation(); }
        return pos;
    }

    // Запись информации непосредственно в файл. Перенос из буфера в файл
    template<typename FileStreamT>
    inline void FlushFile(FileStreamT& file_stream) { // default value in forward declaring section
        file_stream.flush();
        if (!file_stream) { throw ErrorWriteFile(); }
    }


    // Вычисление размера файла.
    // return функции можно свободно конвертировать в int или long.
    // Внимание! По умолчанию Оставляет позицию в файле - в конце строки
    template<typename CharT = char, typename FileStreamT = std::ifstream>
    std::fpos<typename std::char_traits<CharT>::state_type>
    inline SizeOfFile(FileStreamT& file_stream, bool save_file_position = true) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value
            || std::is_same<CharT, unsigned char>::value || std::is_same<CharT, std::byte>::value,
            "CharT can be only: char, unsigned char, wchar_t or std::byte");
        std::fpos<std::char_traits<CharT>::state_type> current;
        if (save_file_position) {
            current = file_stream.tellg();               if (!(file_stream.good())) { throw ErrorFileOperation(); }
        }
        // Не обязательно, но на всякий случай переставляем указатель в начало файла
        if (file_stream.tellg() != 0) {
            file_stream.seekg(0, std::ios_base::beg);    if (!(file_stream.good())) { throw ErrorFileOperation(); }
        }

        std::fpos<std::char_traits<CharT>::state_type> begin, end;
        begin = file_stream.tellg();                     if (!(file_stream.good())) { throw ErrorFileOperation(); }
        file_stream.seekg(0, std::ios_base::end);        if (!(file_stream.good())) { throw ErrorFileOperation(); }
        end = file_stream.tellg();                       if (!(file_stream.good())) { throw ErrorFileOperation(); }

        if (save_file_position) {
            file_stream.seekg(current);               if (!(file_stream.good())) { throw ErrorFileOperation(); }
        }

        return end - begin;
    }

    // Вычисление размера файла.
    // return функции можно свободно конвертировать в int или long.
    template<typename CharT = char>
    std::fpos<typename std::char_traits<CharT>::state_type>
    inline SizeOfFile(const std::string& file_path) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value
            || std::is_same<CharT, unsigned char>::value || std::is_same<CharT, std::byte>::value,
            "CharT can be only: char, unsigned char, wchar_t or std::byte");
        std::basic_ifstream<CharT> read_file_stream = OpenFile<std::basic_ifstream<CharT>>(file_path, std::ios_base::in | std::ios_base::binary);

        auto size = SizeOfFile<CharT, std::basic_ifstream<CharT>>(read_file_stream, false);

        CloseFile(read_file_stream);
        return size;
    }


    // Копирование файла из пути original в файл copy
    inline void CopyFile(const std::string& original, const std::string& copy) {
        std::basic_fstream<unsigned char> read_write_file_stream
            = OpenFile<std::basic_fstream<unsigned char>>(original, OpenModeReadBinary);
        if (!FileExists(copy)) {
            auto size = SizeOfFile<unsigned char>(read_write_file_stream);
            std::vector<unsigned char> file_content{};
            if (size > 0) {
                if (read_write_file_stream.tellg() != 0) { // Переставляем позицию в файле на начало
                    read_write_file_stream.seekg(0, std::ios_base::beg);    if (!(read_write_file_stream.good())) { throw ErrorFileOperation(); }
                }

                file_content.resize(size);
                read_write_file_stream.read(&file_content[0], size);
                if (read_write_file_stream.bad()) { throw ErrorReadFile(); }
            }

            CloseFile(read_write_file_stream);

            OpenFile(read_write_file_stream, copy, OpenModeWriteBinaryRewrite);
            WriteSerializedDataFile<unsigned char>(copy.c_str(), &file_content[0], file_content.size(), OpenModeWriteBinaryAppend, std::locale());
            CloseFile(read_write_file_stream);
        }
    }

    // Очистка файла от содержимого и закрытие файла
    template<typename CharT = char>
    inline void ClearFileContent(const std::string& file_path) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "CharT can be only: char or wchar_t");
        std::basic_ofstream<CharT> write_file_stream = OpenFile<std::basic_ofstream<CharT>>(file_path, std::ios_base::out | std::ios_base::trunc);
        CloseFile(write_file_stream);
    }

    // Read one line from file from current position
    template<typename CharT = char, typename FileStreamT = std::basic_ifstream<CharT>>
    inline std::string ReadLineInFile(FileStreamT& file_stream) {
        std::basic_string<CharT> str{};
        std::getline(file_stream, str);
        if (!file_stream) { throw ErrorReadFile(); }

        return str;
    }

    template<typename CharT = char>
    inline std::basic_string<CharT> MakeStringFrmVectorByte(const std::vector<CharT>& readed_data) {
        std::basic_string<CharT> str_data{};
        str_data.append(readed_data.begin(), readed_data.end());
        return str_data;
    }

    //template<typename CharT = char>
    //inline std::basic_string<CharT> MakeStringFrmVectorByte(const std::vector<CharT>&& readed_data) {
    //    return MakeStringFrmVectorByte<CharT>(readed_data);
    //    /*std::basic_string<CharT> str_data{};
    //    str_data.append(readed_data.begin(), readed_data.end());
    //    return str_data;*/
    //}

}

#endif // !FILESERVICE_H