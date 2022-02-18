#ifndef WRITETEXTFILE_H
#define WRITETEXTFILE_H

#include <fstream>
#include <locale>
#include <type_traits>
#include <string>

#include "file-constants.hpp"
#include "file-service.hpp"

namespace file {

    // ���������� ������ ��������� ���������� � ���� ����� �������� <<
        // ���������: ����������� ������� � ������ ������������ �������� ����� ������������������ �� �������.
        // �� ��������� ��������� � ������ Append ��� ���������� � ����� �����
        // ���� ������ �������������� � ����� wchar_t, �� � file_content ������ ��������� ������� L"" ��� ���� wchar_t
        // ����� ������� �������� nullterminator �� ������������
    template<typename CharT = char>
    void WriteTextFile(const char* file_path, const std::basic_string<CharT>& file_content,
                        std::ios_base::openmode open_mode = OpenModeWriteAppend,
                        const std::locale& locale = std::locale()) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "CharT can be only: char or wchar_t");
        std::basic_ofstream<CharT> write_file_stream = OpenFile<std::basic_ofstream<CharT>>(file_path, open_mode);
        write_file_stream.imbue(locale);

        if (file_content.size() > 0) {
            write_file_stream << file_content; // �� ���������� white space - ������ std::noskipws
            if (write_file_stream.fail()) { throw ErrorWriteFile(); }
        }

        // ������ ���������� ��������������� � ����
        FlushFile(write_file_stream);
        CloseFile(write_file_stream);
    }

    // ������������, ���� ���� � ����� - ��� ������ ���� std::string
    template<typename CharT = char>
    void WriteTextFile(const std::string& file_path, const std::basic_string<CharT>& file_content,
                        std::ios_base::openmode open_mode = OpenModeWriteAppend,
                        const std::locale& locale = std::locale()) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "CharT can be only: char or wchar_t");
        WriteTextFile<CharT>(file_path.c_str(), file_content, open_mode, locale);
    }

}

#endif // !WRITETEXTFILE_H