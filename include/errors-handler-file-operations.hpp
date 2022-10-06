#ifndef ERRORSHANDLERFILEOPERATIONS_H
#define ERRORSHANDLERFILEOPERATIONS_H

#include <exception>
#include <stdexcept>
#include <type_traits>
#include <iostream>

#include "string"
#include "exception-handling-console.hpp"

namespace errors {
    inline constexpr int kOpenFileErrorCode             { -21 };
    inline constexpr int kCloseFileErrorCode            { -22 };
    inline constexpr int kWriteFileErrorCode            { -23 };
    inline constexpr int kReadFileErrorCode             { -24 };
    inline constexpr int kFileOperationErrorCode        { -25 };
    inline constexpr int kFileStreamIsClosedErrorCode   { -26 };

    inline const std::string kOpenFileErrorName             { "Open File Error" };
    inline const std::string kCloseFileErrorName            { "Close File Error" };
    inline const std::string kWriteFileErrorName            { "Write File Error" };
    inline const std::string kReadFileErrorName             { "Read File Error" };
    inline const std::string kFileOperationErrorName        { "File Operation Error" };
    inline const std::string kFileStreamIsClosedErrorName   { "File Stream Is Closed Error" };

    inline const std::string kOpenFileErrorType             { "OpenFileError" };
    inline const std::string kCloseFileErrorType            { "CloseFileError" };
    inline const std::string kWriteFileErrorType            { "WriteFileError" };
    inline const std::string kReadFileErrorType             { "ReadFileError" };
    inline const std::string kFileOperationErrorType        { "FileOperationError" };
    inline const std::string kFileStreamIsClosedErrorType   { "FileStreamIsClosedError" };

    constexpr char msg_error_open_file[]		{ "Error in opening file." };
    constexpr char msg_error_close_file[]		{ "Error in closing file." };
    constexpr char msg_error_write_file[]		{ "Error in writing file content." };
    constexpr char msg_error_read_file[]		{ "Error in reading file content." };
    constexpr char msg_error_file_operation[]	{ "Error in operation with file." };
    constexpr char msg_error_stream_is_closed[]	{ "File stream is not opened." };

    template<typename CharT>
    concept Chars = std::is_same_v<CharT, char> || std::is_same_v<CharT, wchar_t>;

    //* ������ �������� �����
    template<Chars CharT = char>
    struct OpenFileError : public std::runtime_error {
        OpenFileError()
            : std::runtime_error(msg_error_open_file),
            file_path{} {};
        OpenFileError(const std::basic_string<CharT>& file_path_p)
            : std::runtime_error(msg_error_open_file),
            file_path{ file_path_p } {};

        std::basic_string<CharT> file_path{};
    };

    // ������ �������� �����
    template<Chars CharT = char>
    struct CloseFileError : public std::runtime_error {
        CloseFileError()
            : std::runtime_error(msg_error_close_file),
            file_path{} {};
        CloseFileError(const std::basic_string<CharT>& file_path_p)
            : std::runtime_error(msg_error_close_file),
            file_path{ file_path_p } {};

        std::basic_string<CharT> file_path{};
    };

    // ������ ������ ���������� � ����
    template<Chars CharT = char>
    struct WriteFileError : public std::runtime_error {
        WriteFileError()
            : std::runtime_error(msg_error_write_file),
            file_path{} {};
        WriteFileError(const std::basic_string<CharT>& file_path_p)
            : std::runtime_error(msg_error_write_file),
            file_path{ file_path_p } {};

        std::basic_string<CharT> file_path{};
    };

    // ������ ������ ���������� �� �����
    template<Chars CharT = char>
    struct ReadFileError : public std::runtime_error {
        ReadFileError()
            : std::runtime_error(msg_error_read_file),
            file_path{} {};
        ReadFileError(const std::basic_string<CharT>& file_path_p)
            : std::runtime_error(msg_error_read_file),
            file_path{ file_path_p } {};

        std::basic_string<CharT> file_path{};
    };

    // ������ ��������, ������ � �������
    struct FileOperationError : public std::runtime_error {
        FileOperationError() : std::runtime_error("Error in operation with file.") {};
    };

    /** Stream is not opened for operation. */
    struct FileStreamIsClosedError : public std::runtime_error {
        FileStreamIsClosedError() : std::runtime_error(msg_error_stream_is_closed) {};
    };

    template<Chars CharT = char>
    inline void COut(const std::basic_string<CharT>& text);
    template<>
    inline void COut<char>(const std::basic_string<char>& text) {
        std::cout << text << '\n';
    };
    template<>
    inline void COut<wchar_t>(const std::basic_string<wchar_t>& text) {
        std::wcout << text << L'\n';
    };

    // ��������� ������
    template<Chars CharT = char>
    inline int OpenFileErrorHandle(const OpenFileError<CharT>& error) {
        errors::StandardErrorHandle(error, kOpenFileErrorName, kOpenFileErrorType);
        COut(error.file_path);
        return kOpenFileErrorCode;
    };

    template<Chars CharT = char>
    inline int CloseFileErrorHandle(const CloseFileError<CharT>& error) {
        errors::StandardErrorHandle(error, kCloseFileErrorName, kCloseFileErrorType);
        COut(error.file_path);
        return kCloseFileErrorCode;
    };

    template<Chars CharT = char>
    inline int WriteFileErrorHandle(const WriteFileError<CharT>& error) {
        errors::StandardErrorHandle(error, kWriteFileErrorName, kWriteFileErrorType);
        COut(error.file_path);
        return kWriteFileErrorCode;
    };

    template<Chars CharT = char>
    inline int ReadFileErrorHandle(const ReadFileError<CharT>& error){
        errors::StandardErrorHandle(error, kReadFileErrorName, kReadFileErrorType);
        COut(error.file_path);
        return kReadFileErrorCode;
    };

    int FileOperationErrorHandle();
    int FileStreamIsClosed();
}

#endif // !ERRORSHANDLERFILEOPERATIONS_H
