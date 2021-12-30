#ifndef WRITEBINARYFILE_H
#define WRITEBINARYFILE_H

#include <fstream>
#include <locale>
#include <type_traits>
#include <string>
#include <vector>

#include "FileConstants.h"
#include "FileService.h"

namespace file {

    // �������� ������ ������� � ����: �� ������� �� ����� ������ ��, ��� �������.
    // ����� ������������ ����������� ����� �������� ��� �������� ������. OpenModeWriteBinaryAppend ��� OpenModeWriteBinaryRewrite.
    // first_object_symbol - ��� ��������� �� ������ ������ ��� ������ ������� ������ �������.
    // ��������, ��������� �� ������ ������� �������. reinterpret_cast<ByteT*>(&object[0])
    // object_size - ���������� ����, ������� �������� ������
    template<typename ByteT = char>
    void WriteBinaryFile(std::basic_ofstream<ByteT>& write_file_stream,
                        const void* first_object_symbol, const std::streamsize object_size,
                        std::ios_base::openmode open_mode = OpenModeWriteBinaryAppend,
                        const std::locale& locale = std::locale()) { // open_mode & locale is defined as default in forward declaration
        write_file_stream.imbue(locale);

        if (object_size > 0) { // wofstream �� ����� ��������, ���� '\x0101' � ������� ����� �� ����� 00 = '\x0001'
            write_file_stream.write(reinterpret_cast<const ByteT*>(first_object_symbol), object_size);
            if (write_file_stream.bad()) { throw write_file_error(); }

            // ������ ���������� ��������������� � ����
            FlushFile(write_file_stream);
        }
    }

    // �������� ������ ������� � ����: �� ������� �� ����� ������ ��, ��� �������.
    // ����� ������������ ����������� ����� �������� ��� �������� ������. OpenModeWriteBinaryAppend ��� OpenModeWriteBinaryRewrite.
    // first_object_symbol - ��� ��������� �� ������ ������ ��� ������ ������� ������ �������.
    // ��������, ��������� �� ������ ������� �������. reinterpret_cast<ByteT*>(&object[0])
    // object_size - ���������� ����, ������� �������� ������
    template<typename ByteT = char>
    void WriteBinaryFile(const char* file_path, const void* first_object_symbol, const std::streamsize object_size,
                        std::ios_base::openmode open_mode = OpenModeWriteBinaryAppend,
                        const std::locale& locale = std::locale()) { // open_mode & locale is defined as default in forward declaration
        std::basic_ofstream<ByteT> write_file_stream = OpenFile<std::basic_ofstream<ByteT>>(file_path, open_mode);
        WriteBinaryFile<ByteT>(write_file_stream, first_object_symbol, object_size, open_mode, locale);
        CloseFile(write_file_stream);
    }

    template<typename ByteT = char>
    void WriteBinaryFile(const std::string& file_path, const void* first_object_symbol, const std::streamsize object_size,
                        std::ios_base::openmode open_mode = OpenModeWriteBinaryAppend,
                        const std::locale& locale = std::locale()) {
        if (object_size > 0) {
            WriteBinaryFile<ByteT>(file_path.c_str(), first_object_symbol, object_size, open_mode, locale);
        }
    }



    // ���������� ����� ������������ ������ � ����������� ��� �� ��������� ������� � ������ char � ������� memcpy ��� bit_cast
    template<typename ByteT = char>
    void WriteBinaryFile(std::basic_ofstream<ByteT>& write_file_stream, const std::vector<ByteT>& data,
                        std::ios_base::openmode open_mode = OpenModeWriteBinaryAppend,
                        const std::locale& locale = std::locale()) {
        if (data.size() > 0) {
            WriteBinaryFile<ByteT>(write_file_stream, &data[0], data.size() * sizeof(ByteT), open_mode, locale);
        }
    }
    
    // ���������� ����� ������������ ������ � ����������� ��� �� ��������� ������� � ������ char � ������� memcpy ��� bit_cast
    template<typename ByteT = char>
    void WriteBinaryFile(const char* file_path, const std::vector<ByteT>& data,
                        std::ios_base::openmode open_mode = OpenModeWriteBinaryAppend,
                        const std::locale& locale = std::locale()) {
        if (data.size() > 0) {
            WriteBinaryFile<ByteT>(file_path, &data[0], data.size() * sizeof(ByteT), open_mode, locale);
        }
    }

    // ���������� ����� ������������ ������ � ����������� ��� �� ��������� ������� � ������ char � ������� memcpy ��� bit_cast
    template<typename ByteT = char>
    void WriteBinaryFile(const std::string& file_path, const std::vector<ByteT>& data,
                        std::ios_base::openmode open_mode = OpenModeWriteBinaryAppend,
                        const std::locale& locale = std::locale()) {
        if (data.size() > 0) {
            WriteBinaryFile<ByteT>(file_path.c_str(), &data[0], data.size() * sizeof(ByteT), open_mode, locale);
        }
    }

}

#endif // !WRITEBINARYFILE_H