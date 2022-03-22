#ifndef READTEXTFILEBIG_H
#define READTEXTFILEBIG_H

#include <fstream>
#include <locale>
#include <type_traits>
#include <string>

#include "file-constants.hpp"
#include "file-service.hpp"

namespace file {

    // wchar_t ����� ��������, ����� �� ������������
    // ���������� ������ ������� ��ڨ��� ��������� ���������� �� ���� ����� �������� >>
    // ���������: ����������� ������� � ������ ������������ �������� ����� ������������������ �� �������.
    // �� ������� �� ����� �� ������ ��, ��� �������
    // ������ �������������� ��� nullterminator
    // ������ wchar_t ����� �������� ���������� �����. ��������, ���� ������� 45 ���� ������������� � ������ 23 ���������
    // � 23 �������� ����� �������� \x00ee - 00 ��� ������ �� ������ ����, ������� �������� ����������
    template<typename CharT = char>
    std::vector<CharT> ReadTextFileBig(const char* file_path, std::ios_base::openmode open_mode = OpenModeReadBinary,
                                       const std::locale& locale = std::locale(std::locale().classic(), 
                                       new std::codecvt<CharT, char, mbstate_t>)) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "CharT can be only: char or wchar_t");
        std::basic_ifstream<char> read_file_stream = OpenFile<std::basic_ifstream<char>>(file_path, open_mode);
        read_file_stream.imbue(locale);

        auto size_of_file = SizeOfFile<CharT>(read_file_stream);
        long long file_content_size{};
        { // ���� � ��� 45 ����, �� �������� ����� 23 ����� wchar_t
            std::lldiv_t div_res{ div(size_of_file, sizeof(CharT)) };
            if (div_res.rem == 0) { file_content_size = div_res.quot; }
            else { file_content_size = div_res.quot + 1; }
        }
        std::vector<CharT> file_content(file_content_size);
        if (size_of_file > 0) { // ������ ������� ������ � ����� �� ������
            SeekgFilePos(read_file_stream);

            read_file_stream.read(reinterpret_cast<char*>(&file_content[0]), size_of_file);
            if (read_file_stream.bad()) { throw ErrorReadFile(); }
        }

        CloseFile(read_file_stream);
        return file_content;
    }

    // wchar_t ����� ��������, ����� �� ������������
    // ������������, ���� ���� � ����� - ��� ������ ���� std::string
    // ���������� ������ ��������� ���������� �� ���� ����� �������� >>
    // ���������: ����������� ������� � ������ ������������ �������� ����� ������������������ �� �������.
    // �� ������� �� ����� �� ������ ��, ��� �������
    // ������ �������������� ��� nullterminator
    template<typename CharT = char>
    std::vector<CharT> ReadTextFileBig(const std::string& file_path,
                                    std::ios_base::openmode open_mode = OpenModeReadBinary,
                                    const std::locale& locale = std::locale(std::locale().classic(),
                                    new std::codecvt<CharT, char, mbstate_t>)) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "CharT can be only: char or wchar_t");
        return std::move(ReadTextFileBig<CharT>(file_path.c_str(), open_mode, locale));
    }

    // ������ ����� ������� ��������. �� �������� ���������. ���������� �� ������������
    // ���������� ������ ������� ��ڨ��� ��������� ���������� �� ���� ����� �������� >>
    // ���������: ����������� ������� � ������ ������������ �������� ����� ������������������ �� �������.
    // �� ������� �� ����� �� ������ ��, ��� �������
    // ������ �������������� ��� nullterminator
    template<typename CharT = char>
    [[deprecated]]
    std::vector<std::basic_string<CharT>> ReadTextFileBigLines(const char* file_path, std::ios_base::openmode open_mode = OpenModeReadFromBegin,
                                                               const std::locale& locale = std::locale()) {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, wchar_t>::value, "CharT can be only: char or wchar_t");
        std::basic_ifstream<CharT> read_file_stream = OpenFile<std::basic_ifstream<CharT>>(file_path, open_mode);
        read_file_stream.imbue(locale);

        std::vector<std::basic_string<CharT>> file_content{};
        if (SizeOfFile<CharT>(read_file_stream) > 0) {
            {
                std::basic_string<CharT> buffer{};
                std::basic_string<CharT> vector_elem{};
                auto max_string_size = vector_elem.max_size();
                do {
                    while (read_file_stream.good()) {
                        std::getline(read_file_stream, buffer);
                        vector_elem += buffer;
                        vector_elem += NewLineChar<CharT>();
                        int a = 0;
                    } //26719608
                    if (vector_elem.size() != max_string_size) { // ���� �� ���� ������������ vector_elem
                        // � ����� ����������� ��������� ���� ������ ������ ����� ������
                        if (vector_elem.size() > 0) { vector_elem.pop_back(); }
                        file_content.emplace_back(vector_elem);
                    }
                    else { // ���� ���� ������������ vector_elem
                        file_content.emplace_back(vector_elem);
                        vector_elem.erase();
                        if (!read_file_stream.eof()) { read_file_stream.clear(); }
                    }
                } while (read_file_stream.good());
            }
            if (read_file_stream.fail()) { throw ErrorReadFile(); }
        }

        CloseFile(read_file_stream);
        return file_content;
    }

}

#endif // !READTEXTFILEBIG_H