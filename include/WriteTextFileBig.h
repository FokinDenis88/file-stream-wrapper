#ifndef WRITETEXTFILEBIG_H
#define WRITETEXTFILEBIG_H

#include <fstream>
#include <locale>
#include <type_traits>
#include <string>

#include "FileConstants.h"
#include "FileService.h"

namespace file {

    // ���������� ������ ������� ��ڨ��� ��������� ���������� � ���� ����� �������� <<
    // ���������� ���������� �������
    // ���������: ����������� ������� � ������ ������������ �������� ����� ������������������ �� �������.
    // �� ��������� ��������� � ������ Append ��� ���������� � ����� �����
    // ���� ������ �������������� � ����� wchar_t, �� � file_content ������ ��������� ������� L"" ��� ���� wchar_t
    // ����� ������� �������� nullterminator �� ������������
    template<typename CharT = char>
    void WriteTextFileBig(const char* file_path, const std::vector<CharT>& file_content,
                            std::ios_base::openmode open_mode = OpenModeWriteBinaryAppend,
                            const std::locale& locale = std::locale(std::locale().classic(),
                            new std::codecvt<CharT, char, mbstate_t>)) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "CharT can be only: char or wchar_t");
        std::basic_ofstream<char> write_file_stream = OpenFile<std::basic_ofstream<char>>(file_path, open_mode);
        write_file_stream.imbue(locale);

        auto size = file_content.size();
        if (size > 0) { // wofstream �� ����� ��������, ���� '\x0101' � ������� ����� �� ����� 00 = '\x0001'
            write_file_stream.write(reinterpret_cast<const char*>(&file_content[0]), size * sizeof(CharT));
            if (write_file_stream.bad()) { throw write_file_error(); }

            // ������ ���������� ��������������� � ����
            FlushFile(write_file_stream);
        }
        CloseFile(write_file_stream);
    }

    // ������������, ���� ���� � ����� - ��� ������ ���� std::string
    // ���������� ���������� �������
    // ���������� ������ ������� ��ڨ��� ��������� ���������� � ���� ����� �������� <<
    // ���������: ����������� ������� � ������ ������������ �������� ����� ������������������ �� �������.
    // �� ��������� ��������� � ������ Append ��� ���������� � ����� �����
    // ���� ������ �������������� � ����� wchar_t, �� � file_content ������ ��������� ������� L"" ��� ���� wchar_t
    // ����� ������� �������� nullterminator �� ������������
    template<typename CharT = char>
    void WriteTextFileBig(const std::string& file_path, const std::vector<CharT>& file_content,
                            std::ios_base::openmode open_mode = OpenModeWriteBinaryAppend,
                            const std::locale& locale = std::locale(std::locale().classic(),
                            new std::codecvt<CharT, char, mbstate_t>)) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "CharT can be only: char or wchar_t");
        WriteTextFileBig<CharT>(file_path.c_str(), file_content, open_mode, locale);
    }

}

#endif // !WRITETEXTFILEBIG_H