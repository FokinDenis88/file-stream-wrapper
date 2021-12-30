#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include <fstream>
#include <locale>
#include <string>
#include <thread>
#include <future>
#include <vector>
#include <type_traits>
#include <bit>

#include <cstdlib>
#include <array>

#include "ErrorsHandlerFileOperations.h"


namespace file {

    // Forward Declaration Section

    

    template<typename FileStreamT = std::ifstream>
    inline void FlushFile(FileStreamT& file_stream);



    //=========================================================================================
    //=========================================================================================

    //// ���������� ������ ��������� ���������� � ���� ����� �������� <<
    //// ���������: ����������� ������� � ������ ������������ �������� ����� ������������������ �� �������.
    //// �� ������� �� ����� �� ������ ��, ��� �������
    //// �� ��������� ��������� � ������ Append ��� ���������� � ����� �����
    //// ���� ������ �������������� � ����� wchar_t, �� � file_content ������ ��������� ������� L"" ��� ���� wchar_t
    //// ����� ������� �������� nullterminator '\0' �� ������������
    //template<typename CharT = char>
    //void WriteTextFile(const std::string& file_path, const std::basic_string<CharT>& file_content,
    //    std::ios_base::openmode open_mode = OpenModeWriteAppend, const std::locale& locale = std::locale()) {
    //    std::basic_ofstream<CharT> write_file_stream{};
    //    OpenFile(write_file_stream, file_path, open_mode);
    //    write_file_stream.imbue(locale);
    //
    //    write_file_stream << file_content;
    //    if (write_file_stream.fail()) { throw write_file_error(); }
    //
    //    // ������ ���������� ��������������� � ����
    //    write_file_stream.flush();
    //    if (!write_file_stream) { throw write_file_error(); }
    //    CloseFile(write_file_stream);
    //}
    //
    //// ���������� ������ ��������� ���������� � ���� ����� �������� <<
    //// ���������: ����������� ������� � ������ ������������ �������� ����� ������������������ �� �������.
    //// �� ������� �� ����� �� ������ ��, ��� �������
    //// �� ��������� ��������� � ������ Append ��� ���������� � ����� �����
    //// ���� ������ �������������� � ����� wchar_t, �� � file_content ������ ��������� ������� L"" ��� ���� wchar_t
    //// ����� ������� �������� nullterminator '\0' �� ������������
    //template<typename CharT = char>
    //void WriteTextFile(const char(&file_path)[], const CharT(&file_content)[], std::ios_base::openmode open_mode = OpenModeWriteAppend,
    //    const std::locale& locale = std::locale()) {
    //    WriteTextFile(std::string(file_path), std::basic_string<CharT>(file_content), open_mode);
    //}
    //
    //// ���������� ������ ��������� ���������� �� ���� ����� �������� >>
    //// ���������: ����������� ������� � ������ ������������ �������� ����� ������������������ �� �������.
    //// �� ������� �� ����� �� ������ ��, ��� �������
    //// ������ �������������� ����� � basic_string<CharT>, ��� nullterminator '\0'
    //template<typename CharT = char>
    //std::basic_string<CharT> ReadTextFile(const std::string& file_path, std::ios_base::openmode open_mode = OpenModeReadFromBegin,
    //    const std::locale& locale = std::locale()) {
    //    std::basic_ifstream<CharT> read_file_stream{};
    //    OpenFile(read_file_stream, file_path, open_mode);
    //    read_file_stream.imbue(locale);
    //
    //    std::basic_string<CharT> file_content{};
    //    read_file_stream >> file_content;
    //    if (read_file_stream.fail()) { throw read_file_error(); }
    //
    //    CloseFile(read_file_stream);
    //    return file_content;
    //}

    //=========================================================================================

    
    
}

#endif // !FILEOPERATIONS_H