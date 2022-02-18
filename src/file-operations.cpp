#include "file-operations.hpp"

namespace file {

    // ������ ������ � ���� � XML ����
    // TODO: ����������� ������ ������ � ������� XML
    void WriteXMLDataInFile() {

    }

    // ������ ������ �� ����� � XML ����
    void ReadXMLDataFromFile() {

    }

    // TODO: ����������� ������ ������ � ������� JSON
    void WriteJSONDataInFile() {

    }

    void ReadJSONDataFromFile() {

    }
}








// ������ Tutorial
// https://www.cplusplus.com/doc/tutorial/files/
//In case that an object is destroyed while still associated with an open file, the destructor automatically calls the member function close.
// 
// ������� ������ ������ � ����, ����� ������, ������� ����� ������� �� ����� ��������������� ������ ���������� � ����
// https://en.cppreference.com/w/cpp/io/c/FILE#Binary_and_text_modes
// 
// https://www.tutorialspoint.com/cplusplus/cpp_files_streams.htm
// Both istream and ostream provide member functions for repositioning the file-position pointer. 
// These member functions are seekg ("seek get") for istream and seekp ("seek put") for ostream.
// 
// 
//

// << >> ��� ����� ������� � �������� ���������� ��������� �������� ������. ��� ����������� ������
// 
// getline - ��������� ��������� ����� ��� ������ file_stream.getline();
//


// STATE FLAGS
// bad() - Returns true if a reading or writing operation fails.For example, in the case that we try to write
// to a file that is not open for writing or if the device where we try to write has no space left.

// fail() - Returns true in the same cases as bad(), but also in the case that a format error happens, 
// like when an alphabetical character is extracted when we are trying to read an integer number.

// eof() - Returns true if a file open for reading has reached the end.

// good() - It is the most generic state flag : it returns false in the same cases in which 
// calling any of the previous functions would return true.Note that good and bad are not exact opposites(good checks more state flags at once).
//

// ���������� ����� ���������� ������� � ����� ��������� ������ basic_filebuf. � ����� ���������� ������� ��� ������������� ����� ������ � ������.
//


// TODO: �������� ��������� ������� �������� ������������ ��������
// ��� ����� ����������� � ���� ���������
//template<class CharT> bool CheckFileStreamOperation(const std::basic_ios<CharT>& file_stream,
//    const std::string error_message = "Error in file stream operation") {
//    if (file_stream.fail()) {
//        //std::cout << error_message;
//        return false;
//    }
//    else return true;
//}




//template void WriteDataFile<wchar_t>(const std::string&, const std::basic_string<wchar_t>&, std::ios_base::openmode, const std::locale&);
//template void WriteDataFile<wchar_t>(const char(&)[], const wchar_t(&)[], std::ios_base::openmode, const std::locale&);

//Variant N2 Write File Stream
//template<typename CharT = char>
//void WriteDataFile(const std::string& file_path, const std::basic_string<CharT>& file_content,
//    std::ios_base::openmode open_mode = OpenModeWriteAppend, const std::locale& locale = std::locale()) {
//    std::basic_ofstream<CharT> write_file_stream(file_path, open_mode);
//    write_file_stream.imbue(locale);
//
//    // !write_file_stream - ����������� ��������. ������������ fail()
//    if (write_file_stream && write_file_stream.is_open()) {
//        if (!write_file_stream << file_content) { throw ErrorWriteFile(); }
//
//        // ������ ���������� ��������������� � ����
//        if (!write_file_stream.flush()) { throw ErrorWriteFile(); }
//    } else { throw ErrorOpenFile(); }
//
//    if (write_file_stream.is_open()) {
//        if write_file_stream.close();
//        if (write_file_stream.bad()) { throw ErrorCloseFile(); }
//    }
//}



// ������ ���
//// ������������� ��� ������ ������ � ���� ������ ������ ������� ������������ WriteDataFile.
//// ���������: ����������� ������� � ������ ������������ �������� ����� ������������������ �� �������.
//// �� ������� �� ����� �� ������ ��, ��� �������
//// ���� ������ �������������� � ����� wchar_t, �� � file_content ������ ��������� ������� L"" ��� ���� wchar_t
//// ����� ������� �������� nullterminator '\0' �� ������������
//template<typename CharT = char>
//void WriteTextInFileCharTArray(const std::string& file_path, const std::basic_string<CharT>& file_content,
//    std::ios_base::openmode open_mode = OpenModeWriteAppend, const std::locale& locale = std::locale()) {
//    std::basic_ofstream<CharT> write_file_stream{};
//    OpenFile(write_file_stream, file_path, open_mode);
//    write_file_stream.imbue(locale);
//
//    // write - ���������� ������ �������� ������� ���������, ���������� �������� ������� ���������
//    write_file_stream.write(file_content.data(), file_content.size());
//    if (write_file_stream.bad()) { throw ErrorWriteFile(); }
//
//    // ������ ���������� ��������������� � ����
//    write_file_stream.flush();
//    if (!write_file_stream) { throw ErrorWriteFile(); }
//    CloseFile(write_file_stream);
//}
//
//// ������������� ��� ������ ������ � ���� ������ ������ ������� ������������ WriteDataFile.
//// ���������: ����������� ������� � ������ ������������ �������� ����� ������������������ �� �������.
//// �� ������� �� ����� �� ������ ��, ��� �������
//// ���� ������ �������������� � ����� wchar_t, �� � file_content ������ ��������� ������� L"" ��� ���� wchar_t
//// ����� ������� �������� nullterminator '\0' �� ������������
//// This function is not overloaded for the types signed char or unsigned char
//template<typename CharT = char>
//void WriteTextInFileCharTArray(const char(&file_path)[], const CharT(&file_content)[], std::ios_base::openmode open_mode = OpenModeWriteAppend,
//    const std::locale& locale = std::locale()) {
//    WriteTextInFileCharTArray(std::string(file_path), std::basic_string<CharT>(file_content), open_mode);
//}

// ���������� ������ ���������� �����. ������������� ��� ������ ������ � ���� ������ ������ ������� ������������ ReadAllTextFromFile.
// ���������: ����������� ������� � ������ ������������ �������� ����� ������������������ �� �������.
// �� ������� �� ����� �� ������ ��, ��� �������
// ������ �������������� ����� � basic_string<CharT>, ��� nullterminator '\0'
//template<typename CharT = char>
//std::basic_string<CharT> ReadTextFromFileCharTArray(const std::string& file_path, std::ios_base::openmode open_mode = OpenModeReadFromBegin,
//    const std::locale& locale = std::locale()) {
//    std::basic_ifstream<CharT> read_file_stream{};
//    OpenFile(read_file_stream, file_path, open_mode);
//    read_file_stream.imbue(locale);
//
//    std::basic_string<CharT> file_content{};
//    while (!(read_file_stream.eof())) {
//        std::getline(read_file_stream, file_content);
//        if (read_file_stream.fail()) { throw ErrorReadFile(); }
//    }
//
//    CloseFile(read_file_stream);
//    return file_content;
//    // ������� ������ ������ � ����, ����� ������, ������� ����� ������� �� ����� ��������������� ������ ���������� � ����
//    // https://en.cppreference.com/w/cpp/io/c/FILE#Binary_and_text_modes
//}



// ������ ����� � ������� getline
//template<typename CharT = char, typename ReturnT = std::basic_string<CharT>>
//ReturnT ReadAllTextFromFile(const char* file_path, std::ios_base::openmode open_mode = OpenModeReadFromBegin,
//    const std::locale& locale = std::locale()) {
//    std::basic_ifstream<CharT> read_file_stream = OpenFile<std::basic_ifstream<CharT>>(file_path, open_mode);
//    read_file_stream.imbue(locale);
//
//    ReturnT file_content{};
//    {
//        ReturnT buffer{};
//        while (read_file_stream.good()) {
//            std::getline(read_file_stream, buffer);
//            file_content += buffer;
//            if (!read_file_stream.eof()) { file_content += CharT('\n'); }
//        }
//    }
//    if (read_file_stream.fail()) { throw ErrorReadFile(); }
//
//    CloseFile(read_file_stream);
//    return file_content;
//}