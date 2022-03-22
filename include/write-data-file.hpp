#ifndef WRITEDATAFILE_H
#define WRITEDATAFILE_H

#include <fstream>
#include <locale>
#include <type_traits>
#include <string>

#include "file-constants.hpp"
#include "file-service.hpp"

namespace file {

    // ���������� ������ ��������� ���������� � ���� ����� �������� <<
    // ���������: ����������� ������� � ������ ������������ �������� ����� ������������������ �� �������.
    // �� ������� �� ����� �� ������ ��, ��� �������
    // �� ��������� ��������� � ������ Append ��� ���������� � ����� �����
    // ���� ������ �������������� � ����� wchar_t, �� � file_content ������ ��������� ������� L"" ��� ���� wchar_t
    // ����� ������� �������� nullterminator �� ������������
    // ��� ������ ������ ����� ��������� ��� ��������� �������. ������� ������ ��� ������� ��������� �������
    // std::thread(WriteDataFile<wchar_t, std::wstring>, std::string("file_path"), std::wstring(L"file_data"), OpenModeWriteRewrite, std::locale())
    // �� ����������� � ����� � ��������� ��������� std::ref ������� �� ������ �� ���������
    template<typename CharT = char, typename ContentT>
    void WriteDataFile(const char* file_path, const ContentT& file_content, std::ios_base::openmode open_mode = OpenModeWriteAppend,
                       const std::locale& locale = std::locale()) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "CharT can be only: char or wchar_t");
        std::basic_ofstream<CharT> write_file_stream = OpenFile<std::basic_ofstream<CharT>>(file_path, open_mode);
        write_file_stream.imbue(locale);

        // �� ���������� white space - ������ std::noskipws
        write_file_stream << file_content;
        if (write_file_stream.fail()) { throw ErrorWriteFile(); }

        // ������ ���������� ��������������� � ����
        FlushFile(write_file_stream);
        CloseFile(write_file_stream);
    }

    // ������������, ���� ���� � ����� - ��� ������ ���� std::string
    template<typename CharT = char, typename ContentT>
    void WriteDataFile(const std::string& file_path, const ContentT& file_content, std::ios_base::openmode open_mode = OpenModeWriteAppend,
                       const std::locale& locale = std::locale()) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "CharT can be only: char or wchar_t");
        WriteDataFile<CharT, ContentT>(file_path.c_str(), file_content, open_mode, locale);
    }

}

#endif // !WRITEDATAFILE_H