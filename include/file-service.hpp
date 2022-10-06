#ifndef FILESERVICE_H
#define FILESERVICE_H

#include <fstream>
#include <vector>
#include <type_traits>

#define CORRECT_NAME_FOR_WINDOWS
#ifdef CORRECT_NAME_FOR_WINDOWS
#include <regex>
#include <string>

#include "file-types.hpp"
#endif // CORRECT_NAME_FOR_WINDOWS

#include "file-constants.hpp"
#include "file-types.hpp"
#include "errors-handler-file-operations.hpp"

namespace file {
    //#define CORRECT_NAME_FOR_WINDOWS

    // Forward Declaration Section

    template<typename ByteT>
    inline void WriteSerializedDataFile(const char* file_path, const void* first_object_symbol, const std::streamsize object_size,
                                        std::ios_base::openmode open_mode,
                                        const std::locale& locale);


    ///  Opens file stream
    template<typename FileStreamT = std::ifstream>
    inline void OpenFile(FileStreamT& file_stream, const std::string& file_path, std::ios_base::openmode open_mode = OpenModeReadFromBegin) {
        file_stream.open(file_path, open_mode);
        // !write_file_stream - ����������� ��������. ������������ fail()
        if (file_stream.fail() || !(file_stream.is_open())) { throw errors::OpenFileError(file_path); }
    }

    /// Opens file stream
    template<typename FileStreamT = std::ifstream>
    inline FileStreamT OpenFile(const std::string& file_path, std::ios_base::openmode open_mode = OpenModeReadFromBegin) {
        FileStreamT file_stream{};
        OpenFile<FileStreamT>(file_stream, file_path, open_mode);
        return file_stream;
    }

    ///
    template<typename FileStreamT = std::ifstream>
    inline void CloseFile(FileStreamT& file_stream) {
        if (file_stream.is_open()) {
            file_stream.close();
            if (file_stream.bad()) { throw errors::CloseFileError(); }
        }
    }

    ///  Checks if file exists
    inline bool FileExists(const std::string& path) {
        std::ifstream read_file(path);
        if (read_file) {
            CloseFile(read_file);
            return true;
        }
        else { return false; }
    }

    /// No checks for file_stream is_open.
    template<typename FileStreamT = std::ifstream>
    inline void SeekgFilePos(FileStreamT& file_stream, long pos) {
        if (file_stream.tellg() != pos) { // ������������ ������� � ����� �� ����������� �������� ������� � �����
            file_stream.seekg(pos);    if (!(file_stream.good())) { throw errors::FileOperationError(); }
        }
    }

    /// No checks for file_stream is_open.
    template<typename FileStreamT = std::ifstream>
    inline void SeekgFilePos(FileStreamT& file_stream, long off = 0, std::ios_base::seekdir dir = std::ios_base::beg) {
        if (file_stream.tellg() != dir + off) { // ������������ ������� � ����� �� ������� dir � ������ ������ off
            file_stream.seekg(off, dir);    if (!(file_stream.good())) { throw errors::FileOperationError(); }
        }
    }
    template<typename FileStreamT = std::ifstream>
    inline void SeekgFilePosEnd(FileStreamT& file_stream, long off = 0, std::ios_base::seekdir dir = std::ios_base::end) {
        if (file_stream.tellg() != dir + off) { // ������������ ������� � ����� �� ������� dir � ������ ������ off
            file_stream.seekg(off, dir);    if (!(file_stream.good())) { throw errors::FileOperationError(); }
        }
    }

    /// No checks for file_stream is_open.
    template<typename FileStreamT = std::ifstream>
    inline void SeekpFilePos(FileStreamT& file_stream, long pos) {
        if (file_stream.tellp() != pos) { // ������������ ������� � ����� �� ����������� �������� ������� � �����
            file_stream.seekp(pos);    if (!(file_stream.good())) { throw errors::FileOperationError(); }
        }
    }

    /// No checks for file_stream is_open.
    template<typename FileStreamT = std::ifstream>
    inline void SeekpFilePos(FileStreamT& file_stream, long off = 0, std::ios_base::seekdir dir = std::ios_base::beg) {
        if (file_stream.tellp() != dir + off) { // ������������ ������� � ����� �� ������� dir � ������ ������ off
            file_stream.seekp(off, dir);    if (!(file_stream.good())) { throw errors::FileOperationError(); }
        }
    }
    /// No checks for file_stream is_open.
    template<typename FileStreamT = std::ifstream>
    inline void SeekpFilePosEnd(FileStreamT& file_stream, long off = 0, std::ios_base::seekdir dir = std::ios_base::end) {
        if (file_stream.tellp() != dir + off) { // ������������ ������� � ����� �� ������� dir � ������ ������ off
            file_stream.seekp(off, dir);    if (!(file_stream.good())) { throw errors::FileOperationError(); }
        }
    }

    /// Tells get position in file
    template<typename CharT = char, typename FileStreamT = std::ifstream>
    inline std::fpos<typename std::char_traits<CharT>::state_type>
    Tellg(FileStreamT& file_stream) {
        if (file_stream && file_stream.is_open()) {
            std::fpos<typename std::char_traits<CharT>::state_type> pos{ file_stream.tellg() };
            bool bad = file_stream.bad();
            bool good = file_stream.good();
            bool eof = file_stream.eof();
            bool fail = file_stream.fail();
            if (!file_stream) { throw errors::FileOperationError(); }
            return pos;
        } else std::fpos<typename std::char_traits<CharT>::state_type>();
    }

    /// Tells put position in file
    template<typename CharT = char, typename FileStreamT = std::ofstream>
    inline std::fpos<typename std::char_traits<CharT>::state_type>
    Tellp(FileStreamT& file_stream) {
        if (file_stream && file_stream.is_open()) {
            std::fpos<typename std::char_traits<CharT>::state_type> pos{ file_stream.tellp() };
            if (!file_stream) { throw errors::FileOperationError(); }
            return pos;
        } else std::fpos<typename std::char_traits<CharT>::state_type>();
    }

    ///
    template<typename FileStreamT>
    inline void FlushFile(FileStreamT& file_stream) { // default value in forward declaring section
        file_stream.flush();
        if (!file_stream) { throw errors::WriteFileError(); }
    }


    ///
    template<typename CharT = char, typename FileStreamT = std::ifstream>
    std::fpos<typename std::char_traits<CharT>::state_type>
    inline SizeOfFile(FileStreamT& file_stream, bool save_file_position = true) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value
            || std::is_same<CharT, unsigned char>::value || std::is_same<CharT, std::byte>::value,
            "CharT can be only: char, unsigned char, wchar_t or std::byte");
        std::fpos<typename std::char_traits<CharT>::state_type> current;
        if (save_file_position) {
            current = file_stream.tellg();               if (!(file_stream.good())) { throw errors::FileOperationError(); }
        }
        // �� �����������, �� �� ������ ������ ������������ ��������� � ������ �����
        if (file_stream.tellg() != 0) {
            file_stream.seekg(0, std::ios_base::beg);    if (!(file_stream.good())) { throw errors::FileOperationError(); }
        }

        std::fpos<typename std::char_traits<CharT>::state_type> begin, end;
        begin = file_stream.tellg();                     if (!(file_stream.good())) { throw errors::FileOperationError(); }
        file_stream.seekg(0, std::ios_base::end);        if (!(file_stream.good())) { throw errors::FileOperationError(); }
        end = file_stream.tellg();                       if (!(file_stream.good())) { throw errors::FileOperationError(); }

        if (save_file_position) {
            file_stream.seekg(current);               if (!(file_stream.good())) { throw errors::FileOperationError(); }
        }

        return end - begin;
    }

    ///
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


    ///
    inline void CopyFile(const std::string& original, const std::string& copy) {
        std::basic_fstream<unsigned char> read_write_file_stream
            = OpenFile<std::basic_fstream<unsigned char>>(original, OpenModeReadBinary);
        if (!FileExists(copy)) {
            auto size = SizeOfFile<unsigned char>(read_write_file_stream);
            std::vector<unsigned char> file_content{};
            if (size > 0) {
                if (read_write_file_stream.tellg() != 0) { // ������������ ������� � ����� �� ������
                    read_write_file_stream.seekg(0, std::ios_base::beg);    if (!(read_write_file_stream.good())) { throw errors::FileOperationError(); }
                }

                file_content.resize(size);
                read_write_file_stream.read(&file_content[0], size);
                if (read_write_file_stream.bad()) { throw errors::ReadFileError(copy); }
            }

            CloseFile(read_write_file_stream);

            OpenFile(read_write_file_stream, copy, OpenModeWriteBinaryRewrite);
            WriteSerializedDataFile<unsigned char>(copy.c_str(), &file_content[0], file_content.size(), OpenModeWriteBinaryAppend, std::locale());
            CloseFile(read_write_file_stream);
        }
    }

    ///
    template<typename CharT = char>
    inline void ClearFileContent(const std::string& file_path) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "CharT can be only: char or wchar_t");
        std::basic_ofstream<CharT> write_file_stream = OpenFile<std::basic_ofstream<CharT>>(file_path, std::ios_base::out | std::ios_base::trunc);
        CloseFile(write_file_stream);
    }

    /// Read one line from file from current position
    template<typename CharT = char, typename FileStreamT = std::basic_ifstream<CharT>>
    inline std::string ReadLineInFile(FileStreamT& file_stream) {
        std::basic_string<CharT> str{};
        std::getline(file_stream, str);
        if (!file_stream) { throw errors::ReadFileError(); }

        return str;
    }

    template<typename CharT = char>
    inline std::basic_string<CharT> MakeStringFrmVectorByte(const std::vector<CharT>& readed_data) {
        std::basic_string<CharT> str_data{};
        str_data.append(readed_data.begin(), readed_data.end());
        return str_data;
    }

#ifdef CORRECT_NAME_FOR_WINDOWS
    ///     \/:*?"<>|
    const std::string kWindowsRestrictedCharInFileName{ "\\/:*?\"<>|" };
    const std::wstring kWindowsRestrictedCharInFileNameW{ L"\\/:*?\"<>|" };

    const std::string kRestrictedCharsPattern{ "[" + kWindowsRestrictedCharInFileName + "\n\r]" };
    const std::wstring kRestrictedCharsPatternW{ L"[" + kWindowsRestrictedCharInFileNameW + L"\n\r]" };

    const std::string kSpaceInEndPattern{ "\\s+\\z" };
    const std::wstring kSpaceInEndPatternW{ L"\\s+\\z" };

    /// Checks if path has restricted chars
    /// Returns if there is restricted chars
    template<CharsType CharT = wchar_t>
    inline bool HasRestrictedCharsForOS(const std::basic_string<CharT>& file_name);

    template<>
    inline bool HasRestrictedCharsForOS<char>(const std::string& file_name) {
        if (file_name.empty()) {
            std::regex pattern{ kRestrictedCharsPattern };
            if (std::regex_search(file_name.begin(), file_name.end(), pattern)) {
                return true;
            }

            pattern = kSpaceInEndPattern;
            if (std::regex_search(file_name.begin(), file_name.end(), pattern)) {
                return true;
            } else {
                return false;
            }
        }
    }

    template<>
    inline bool HasRestrictedCharsForOS<wchar_t>(const std::wstring& file_name) {
        if (file_name.empty()) {
            std::wregex pattern{ kRestrictedCharsPatternW };
            if (std::regex_search(file_name.begin(), file_name.end(), pattern)) {
                return true;
            }

            pattern = kSpaceInEndPatternW;
            if (std::regex_search(file_name.begin(), file_name.end(), pattern)) {
                return true;
            } else {
                return false;
            }
        }
    }

    /// Replaces restricted characters
    /// Restricted characters in Windows   \/:*?"<>|
    template<CharsType CharT = wchar_t>
    inline std::basic_string<CharT>
        CorrectFileNameForOS(const std::basic_string<CharT>& name, const std::basic_string<CharT>& replacement,
                             const std::basic_string<CharT>& strange_characters);

    /// Replaces restricted characters
    /// Restricted characters in Windows   \/:*?"<>|
    template<>
    inline std::basic_string<char>
        CorrectFileNameForOS<char>(const std::string& name, const std::string& replacement,
                                   const std::string& strange_characters) {
        std::string correct_name{};
        const std::regex pattern{ "[" + kWindowsRestrictedCharInFileName + strange_characters + "\n\r]" };
        correct_name = std::regex_replace(name, pattern, replacement);
        correct_name = std::regex_replace(correct_name, std::regex{ kSpaceInEndPattern }, "");    // Delete white spaces in the back of file name
        return correct_name;
    }

    template<>
    inline std::basic_string<wchar_t>
        CorrectFileNameForOS<wchar_t>(const std::wstring& name, const std::wstring& replacement,
                                      const std::wstring& strange_characters) {
        std::wstring correct_name{};
        const std::wregex pattern{ L"[" + kWindowsRestrictedCharInFileNameW + strange_characters + L"\n\r]" };
        correct_name = std::regex_replace(name, pattern, replacement);
        correct_name = std::regex_replace(correct_name, std::wregex{ kSpaceInEndPatternW }, L"");    // Delete white spaces in the back of file name
        return correct_name;
    }

    // https://www.mtu.edu/umc/services/websites/writing/characters-avoid/
    // const std::string kCharsToAvoidInOSPaths{ "\\/:*?\"<>|" };

#endif // CORRECT_NAME_FOR_WINDOWS


} // !namespace file

#endif // !FILESERVICE_H
