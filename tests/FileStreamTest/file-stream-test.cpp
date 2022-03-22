#include "pch.h"
#include "CppUnitTest.h"

#include <string>
#include <array>
#include <codecvt>
#include <filesystem>

#include "file-operations.hpp"

#include "bom.hpp"
#include "file-constants.hpp"
#include "file-service.hpp"
#include "read-data-file.hpp"
#include "read-data-file-thread.hpp"
#include "read-serialized-data-file.hpp"
#include "read-serialized-data-file-thread.hpp"
#include "read-text-file.hpp"
#include "read-text-file-big.hpp"
#include "read-text-file-big-thread.hpp"
#include "read-text-file-thread.hpp"
#include "write-data-file.hpp"
#include "write-data-file-thread.hpp"
#include "write-serialized-data-file.hpp"
#include "write-serialized-data-file-thread.hpp"
#include "write-text-file.hpp"
#include "write-text-file-big.hpp"
#include "write-text-file-big-thread.hpp"
#include "write-text-file-thread.hpp"
#include "read-formatted-data-table-csv.hpp"
#include "write-data-table-csv.hpp"
#include "divide-char-vec-to-lines.hpp"
#include "get-file-lines.hpp"
#include "read-binary-file.hpp"
#include "write-binary-file.hpp"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FileStreamTest
{
    using namespace file;
    const std::string kProjectPath = "../";

    TEST_CLASS(FileStreamTest)
    {
    public:
        
        FileStreamTest() {
        }
        ~FileStreamTest() {
        }

        TEST_CLASS_INITIALIZE(InitClassFileStreamTest) {
            // Запись тестового первоначального файла
            std::ofstream write_file_stream(reference_file_path);
            if (write_file_stream) {
                write_file_stream << reference_file_data;
            }
            write_file_stream.close();

            std::wofstream write_file_stream_wchrt(reference_file_path_wchrt);
            if (write_file_stream) {
                write_file_stream_wchrt << reference_file_data_wchrt;
            }
            write_file_stream.close();

            write_file_stream.open(clear_file_path);
            if (write_file_stream) {
                write_file_stream << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$";
            }

            std::ofstream write_file_stream_serial_char(reference_serialized_path_char);
            if (write_file_stream_serial_char) {
                auto data = std::bit_cast<std::array<char, sizeof(data_for_serialization)>>(data_for_serialization);
                data_serialized.reserve(data.size());
                for (const auto& elem : data) {
                    data_serialized.push_back(elem);
                }
                write_file_stream_serial_char.write(&data[0], data.size());
            }
            write_file_stream_serial_char.close();

            data_serialized_binary.resize(data_serialized.size());
            memcpy(&data_serialized_binary[0], &data_serialized[0], sizeof(data_serialized_binary[0])* data_serialized.size());

            {
                auto data = std::bit_cast<std::array<wchar_t, sizeof(data_for_serialization) / sizeof(wchar_t)>>(data_for_serialization);
                data_serialized_wchrt.reserve(data.size());
                for (const auto& elem : data) {
                    data_serialized_wchrt.push_back(elem);
                }
            }

            {
                auto data = std::bit_cast<std::array<std::byte , sizeof(data_for_serialization) / sizeof(std::byte) >> (data_for_serialization);
                data_serialized_byte.reserve(data.size());
                for (const auto& elem : data) {
                    data_serialized_byte.push_back(elem);
                }
            }

            std::ofstream write_file_stream_serial_wchrt(reference_serialized_path_wchrt, std::ios_base::out | std::ios_base::binary |                                                      std::ios_base::trunc);
            if (write_file_stream_serial_wchrt) {
                write_file_stream_serial_wchrt.imbue(std::locale());
                write_file_stream_serial_wchrt.write(reinterpret_cast<const char*>(&data_serialized[0]), data_serialized.size());
            }
            write_file_stream_serial_wchrt.close();
        }
        
        TEST_CLASS_CLEANUP(CleanClassFileStreamTest) {
        }

        TEST_METHOD(OpenFileTest) {
            Assert::IsTrue(static_cast<bool>(OpenFile<std::ifstream>(reference_file_path)));
        }

        TEST_METHOD(CloseFileTest) {
            auto file_stream = OpenFile(reference_file_path);
            CloseFile(file_stream);
        }

        TEST_METHOD(ReadTextFileChar)
        {
            auto res{ ReadTextFile<char>(reference_file_path) };
            Assert::AreEqual(reference_file_data, res);
        }
        /*TEST_METHOD(ReadTextFilewchr)
        {
            Assert::AreEqual(reference_file_data_wchrt, readtextfile<wchar_t>(reference_file_path_wchrt));
        }
        TEST_METHOD(ReadTextFilewchrloc)
        {
            Assert::AreEqual(reference_file_data_wchrt, readtextfile<wchar_t>(reference_file_path_wchrt, 1, std::locale("en_us.utf-8")));
        }*/

        TEST_METHOD(WriteTextFileChar)
        {
            std::string data = std::string("WriteTextInFileCharData");
            WriteTextFile<char>("Files/WriteTextInFileChar.data", data, OpenModeWriteRewrite);
            Assert::AreEqual(data, ReadTextFile<char>("Files/WriteTextInFileChar.data"));
        }
        TEST_METHOD(WriteTextFileWchr)
        {
            std::wstring data2 = std::wstring(L"WriteTextInFileWchartData");
            WriteTextFile<wchar_t>("Files/WriteTextInFileWchart.data", data2, OpenModeWriteRewrite);
            Assert::AreEqual(data2, ReadTextFile<wchar_t>("Files/WriteTextInFileWchart.data"));
        }
        TEST_METHOD(WriteTextFileWchrLoc)
        {
            std::wstring data3 = std::wstring(L"WriteTextInFileWchartLocaleData");
            WriteTextFile<wchar_t>("Files/WriteTextInFileWchartLocale.data", data3, OpenModeWriteRewrite, std::locale("en_US.UTF-8"));
            Assert::AreEqual(data3, ReadTextFile<wchar_t>("Files/WriteTextInFileWchartLocale.data"));
        }

        BEGIN_TEST_METHOD_ATTRIBUTE(ReadTextFileBigChar)
            TEST_METHOD_ATTRIBUTE(L"TIME", L"Long")
        END_TEST_METHOD_ATTRIBUTE()
        TEST_METHOD(ReadTextFileBigChar)
        {
            std::vector<char>file_content{ ReadTextFileBig<char>(reference_bigfile_path) };
            WriteTextFileBig<char>(reference_big_result_char, file_content, OpenModeWriteBinaryRewrite);
            std::vector<char>file_content_result{ ReadTextFileBig<char>(reference_big_result_char) };
            Assert::IsTrue(file_content == file_content_result);
        }

        BEGIN_TEST_METHOD_ATTRIBUTE(ReadTextFileBigWchr)
            TEST_METHOD_ATTRIBUTE(L"TIME", L"Long")
        END_TEST_METHOD_ATTRIBUTE()
        TEST_METHOD(ReadTextFileBigWchr)
        {
            std::vector<wchar_t>file_content{ ReadTextFileBig<wchar_t>(reference_bigfile_path) };
            WriteTextFileBig<wchar_t>(reference_big_result_wchrt, file_content, OpenModeWriteBinaryRewrite);
            std::vector<wchar_t>file_content_result_wchrt{ ReadTextFileBig<wchar_t>(reference_big_result_wchrt) };
            Assert::IsTrue(file_content == file_content_result_wchrt);
        }

        BEGIN_TEST_METHOD_ATTRIBUTE(WriteTextFileBigChar)
            TEST_METHOD_ATTRIBUTE(L"TIME", L"Long")
        END_TEST_METHOD_ATTRIBUTE()
        TEST_METHOD(WriteTextFileBigChar)
        {
            std::vector<char>file_content{ ReadTextFileBig<char>(reference_bigfile_path) };
            WriteTextFileBig<char>(reference_big_result_char, file_content, OpenModeWriteBinaryRewrite);
            Assert::AreEqual((long)SizeOfFile(reference_bigfile_path), (long)SizeOfFile(reference_big_result_char));
        }

        BEGIN_TEST_METHOD_ATTRIBUTE(WriteTextFileBigWchr)
            TEST_METHOD_ATTRIBUTE(L"TIME", L"Long")
        END_TEST_METHOD_ATTRIBUTE()
        TEST_METHOD(WriteTextFileBigWchr)
        {
            std::vector<wchar_t>file_content{ ReadTextFileBig<wchar_t>(reference_bigfile_path) };
            WriteTextFileBig<wchar_t>(reference_big_result_wchrt, file_content, OpenModeWriteBinaryRewrite);
            Assert::AreEqual((long)SizeOfFile(reference_bigfile_path), (long)SizeOfFile(reference_big_result_wchrt));
        }

        TEST_METHOD(ReadTextFileNewThreadChar)
        {
            Assert::AreEqual(reference_file_data, ReadTextFileNewThread<char>(reference_file_path));
        }
        /*TEST_METHOD(ReadTextFileNewThreadWchr)
        {
            Assert::AreEqual(reference_file_data_wchrt, ReadTextFileNewThread<wchar_t>(reference_file_path_wchrt));
        }*/

        //TEST_METHOD(WriteTextFileNewThreadChar)
        //{
        //    std::string data = std::string("WriteTextInFileCharNewThreadData");
        //    WriteTextFileNewThread<char>("Files/WriteTextInFileCharNewThread.data", data, OpenModeWriteRewrite);
        //    Assert::AreEqual(data, ReadTextFileNewThread<char>("Files/WriteTextInFileCharNewThread.data"));
        //}
        //TEST_METHOD(WriteTextFileNewThreadWchr)
        //{
        //    std::wstring data2 = std::wstring(L"WriteTextInFileWchartNewThreadData");
        //    WriteTextFileNewThread<wchar_t>("Files/WriteTextInFileWchartNewThread.data", data2, OpenModeWriteRewrite);
        //    Assert::AreEqual(data2, ReadTextFileNewThread<wchar_t>("Files/WriteTextInFileWchartNewThread.data"));
        //}

        BEGIN_TEST_METHOD_ATTRIBUTE(ReadTextFileBigNewThreadTest)
            TEST_METHOD_ATTRIBUTE(L"TIME", L"Long")
        END_TEST_METHOD_ATTRIBUTE()
        TEST_METHOD(ReadTextFileBigNewThreadTest)
        {
            /*Assert::AreEqual(std::vector<std::wstring>{std::wstring(reference_file_data_wchrt)},
                ReadTextFileBig<wchar_t>(reference_file_path_wchrt));*/
        }

        BEGIN_TEST_METHOD_ATTRIBUTE(WriteTextFileBigNewThreadTest)
            TEST_METHOD_ATTRIBUTE(L"TIME", L"Long")
        END_TEST_METHOD_ATTRIBUTE()
        TEST_METHOD(WriteTextFileBigNewThreadTest)
        {
        }

        TEST_METHOD(WriteSerializedDataFileChar)
        {
            std::string second_func_path = "Files/Test Serialized Char Symbol Adress.dat";
            WriteSerializedDataFile<unsigned char>(test_serialized_path_char, data_serialized, OpenModeWriteBinaryRewrite);
            WriteSerializedDataFile(second_func_path, &data_serialized[0],
                                            data_serialized.size(), OpenModeWriteBinaryRewrite);

            auto out_stream = OpenFile<std::basic_ofstream<unsigned char>>(test_serialized_path_char);
            WriteSerializedDataFile<unsigned char>(test_serialized_stream_path_char, data_serialized, OpenModeWriteBinaryRewrite);
            CloseFile(out_stream);

            auto readed_data = ReadSerializedDataFile(test_serialized_path_char);
            Assert::IsTrue(data_serialized == readed_data);
            auto readed_data_byte = ReadSerializedDataFile<std::byte>(test_serialized_path_char);
            Assert::IsTrue(data_serialized_byte == readed_data_byte);
            auto readed_data2 = ReadSerializedDataFile(second_func_path);
            Assert::IsTrue(data_serialized == readed_data2);
            auto readed_data_symbol = ReadSerializedDataFile(test_serialized_path_char);
            Assert::IsTrue(data_serialized == readed_data_symbol);

            long long reversed_data{};
            memcpy(&reversed_data, &readed_data[0], readed_data.size());
            Assert::AreEqual(data_for_serialization, reversed_data);

            auto inp_stream = OpenFile<std::basic_ifstream<unsigned char>>(test_serialized_stream_path_char);
            std::vector<unsigned char> readed_data_stream = ReadSerializedDataFile(inp_stream);
            CloseFile(inp_stream);
            Assert::IsTrue(data_serialized == readed_data_stream);
        }
        TEST_METHOD(WriteSerializedDataFileWchrt)
        {
            std::string second_func_path = "Files/Test Serialized WChart Symbol Adress.dat";
            WriteSerializedDataFile<wchar_t>(test_serialized_path_wchrt, data_serialized_wchrt, OpenModeWriteBinaryRewrite);
            WriteSerializedDataFile(second_func_path, &data_serialized_wchrt[0],
                                               data_serialized_wchrt.size()*sizeof(wchar_t), OpenModeWriteBinaryRewrite);
            auto readed_data = ReadSerializedDataFile(test_serialized_path_wchrt);
            Assert::IsTrue(data_serialized == readed_data);
            auto readed_data_byte = ReadSerializedDataFile<std::byte>(test_serialized_path_wchrt);
            Assert::IsTrue(data_serialized_byte == readed_data_byte);
            auto readed_data_symbol = ReadSerializedDataFile(second_func_path);
            Assert::IsTrue(data_serialized == readed_data_symbol);
            auto readed_data2 = ReadSerializedDataFile(second_func_path);
            Assert::IsTrue(data_serialized == readed_data2);

            long long reversed_data{};
            memcpy(&reversed_data, &readed_data[0], readed_data.size());
            Assert::AreEqual(data_for_serialization, reversed_data);
        }

        TEST_METHOD(ReadSerializedDataFileChar_Test)
        {
            auto readed_data = ReadSerializedDataFile(reference_serialized_path_char);
            Assert::IsTrue(data_serialized == readed_data);
            auto readed_data_byte = ReadSerializedDataFile<std::byte>(reference_serialized_path_char);
            Assert::IsTrue(data_serialized_byte == readed_data_byte);
            long long reversed_data{};
            memcpy(&reversed_data, &readed_data[0], readed_data.size());
            Assert::AreEqual(data_for_serialization, reversed_data);
            auto inp_stream = OpenFile<std::basic_ifstream<unsigned char>>(reference_serialized_path_char);
            std::vector<unsigned char> readed_data_stream = ReadSerializedDataFile(inp_stream);
            CloseFile(inp_stream);
            Assert::IsTrue(data_serialized == readed_data_stream);
        }

        TEST_METHOD(WriteSerializedDataFileCharThread)
        {
            std::string second_func_path = "Files/Test Serialized Char Symbol Adress.dat";
            WriteSerializedDataFileNewThread<unsigned char>(test_serialized_path_char, data_serialized, OpenModeWriteBinaryRewrite);
            WriteSerializedDataFileNewThread(second_func_path, &data_serialized[0], data_serialized.size(), OpenModeWriteBinaryRewrite);
            auto readed_data = ReadSerializedDataFileNewThread(test_serialized_path_char);
            Assert::IsTrue(data_serialized == readed_data);
            auto readed_data_symbol = ReadSerializedDataFileNewThread(test_serialized_path_char);
            Assert::IsTrue(data_serialized == readed_data_symbol);
            long long reversed_data{};
            memcpy(&reversed_data, &readed_data[0], readed_data.size());
            Assert::AreEqual(data_for_serialization, reversed_data);
            auto readed_data2 = ReadSerializedDataFileNewThread(second_func_path);
            Assert::IsTrue(data_serialized == readed_data2);
        }
        TEST_METHOD(WriteSerializedDataFileWchrtThread)
        {
            std::string second_func_path = "Files/Test Serialized WChart Symbol Adress.dat";
            WriteSerializedDataFileNewThread<wchar_t>(test_serialized_path_wchrt, data_serialized_wchrt, OpenModeWriteBinaryRewrite);
            WriteSerializedDataFileNewThread(second_func_path, &data_serialized_wchrt[0],
                data_serialized_wchrt.size() * sizeof(wchar_t), OpenModeWriteBinaryRewrite);
            auto readed_data = ReadSerializedDataFileNewThread(test_serialized_path_wchrt);
            Assert::IsTrue(data_serialized == readed_data);
            auto readed_data_symbol = ReadSerializedDataFileNewThread(second_func_path);
            Assert::IsTrue(data_serialized == readed_data_symbol);
            long long reversed_data{};
            memcpy(&reversed_data, &readed_data[0], readed_data.size());
            Assert::AreEqual(data_for_serialization, reversed_data);
            auto readed_data2 = ReadSerializedDataFileNewThread(second_func_path);
            Assert::IsTrue(data_serialized == readed_data2);
        }

        TEST_METHOD(ReadSerializedDataFileCharThread)
        {
            auto readed_data = ReadSerializedDataFileNewThread(reference_serialized_path_char);
            Assert::IsTrue(data_serialized == readed_data);
            auto readed_data_byte = ReadSerializedDataFileNewThread<std::byte>(reference_serialized_path_char);
            Assert::IsTrue(data_serialized_byte == readed_data_byte);

            long long reversed_data{};
            memcpy(&reversed_data, &readed_data[0], readed_data.size());
            Assert::AreEqual(data_for_serialization, reversed_data);
        }

        TEST_METHOD(SizeOfFileChar)
        {
            long long size = SizeOfFile(reference_file_path);
            long long reference_value = 217;
            Assert::AreEqual(reference_value, size);
            //Assert::IsTrue(reference_file_data.size() == size);
        }
        TEST_METHOD(SizeOfFileWchr)
        {
            long long reference_value_wchrt = 224;
            long long size_wchrt = SizeOfFile<wchar_t>(reference_file_path_wchrt);
            Assert::AreEqual(reference_value_wchrt, size_wchrt);
        }

        TEST_METHOD(ClearFileContentTest)
        {
            ClearFileContent(clear_file_path);
            Assert::AreEqual(0, static_cast<int>(SizeOfFile(clear_file_path)));
        }

        TEST_METHOD(CopyFileContentTest)
        {
            std::string source_path = "Files/Copy File Source.dat";
            std::string destination_path = "Files/Copy File Destination.dat";
            std::filesystem::copy(source_path, destination_path);
            auto original_data = ReadSerializedDataFile(source_path);
            auto destination_data = ReadSerializedDataFile(destination_path);
            Assert::IsTrue(original_data == destination_data);
        }


        TEST_METHOD(WriteBOMTest)
        {
            std::string file_path{ "Files/WriteBom Test.dat" };
            std::ofstream output_stream(file_path, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
            const BOMEnum bom{ BOMEnum::UTF_8 };
            WriteBOM(output_stream, bom);
            output_stream.close();

            auto readed_data{ ReadSerializedDataFile(file_path) };
            std::vector<char> readed_data_converted(readed_data.size());
            std::memcpy(&readed_data_converted[0], &readed_data[0], readed_data.size() * sizeof(readed_data[0]));
            
            Assert::IsTrue(readed_data_converted == ByteOrderMark[static_cast<size_t>(bom)]);
        }

        TEST_METHOD(ReadBOMTest)
        {
            std::string file_path{ "Files/ReadBOM Test.dat" };
            for (const std::vector<char>& elem : ByteOrderMark) {
                std::fstream read_write_stream(file_path, std::ios_base::in | std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
                read_write_stream.write(&elem[0], elem.size());
                SeekgFilePos(read_write_stream);

                auto readed_data{ ReadSerializedDataFile(file_path) };
                std::vector<char> readed_data_converted(readed_data.size());
                std::memcpy(&readed_data_converted[0], &readed_data[0], readed_data.size() * sizeof(readed_data[0]));

                Assert::IsTrue(readed_data_converted == elem);
                read_write_stream.close();
            }
        }

        TEST_METHOD(ConvertDataToOtherByteOrderTest) {
            std::vector<char> data_reference{ '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07', '\x08' };
            std::vector<char> data_converted_reference{ '\x08', '\x07', '\x06', '\x05', '\x04', '\x03', '\x02', '\x01' };
            auto results_1_byte{ data_reference };
            SwapEndianness(results_1_byte, 1);
            auto results_2_byte{ data_reference };
            SwapEndianness(results_2_byte, 2);
            auto results_4_byte{ data_reference };
            SwapEndianness(results_4_byte, 4);
            auto results_8_byte{ data_reference };
            SwapEndianness(results_8_byte, 8);
            Assert::IsTrue(data_converted_reference == results_8_byte);
        }

        TEST_METHOD(ReadTableCSV_Test) {
            const std::string file_path{ kProjectPath + "Files/my_ann.csv" };
            //const std::string file_path{ "Files/my_ann.csv" };
            ReadFormattedDataTableCSV<double, double>(file_path);
            //ReadFormattedDataTableCSV<std::string>(file_path);
        }

        TEST_METHOD(DivideCharVecToLinesTest) {
            const std::string str{ "\n\r\nsdfak,  sdafdfs\r\nsdfsdf;lkjsdaf\r\n     \tsd \rffsd\r\nsdffsd\n" };
            std::vector<char> test_vect(str.begin(), str.end());
            std::vector<std::vector<char>> results{ DivideCharVecToLines(test_vect) };
        }

        TEST_METHOD(GetFileLines_Test) {
            /*const std::string file_path{ kProjectPath + "Files/my_ann.csv" };
            std::vector<std::string> file_content{ GetFileLines(file_path, true) };*/

            std::vector<std::string> file_content{ GetFileLines(reference_file_path, true) };
            std::string tmp{};
            for (std::string& str : file_content) {
                tmp.append(str);
            }
            const std::string my_reference_data = "Text for Test. Have fun. Mother fucker!123423434534345  345453435121323456546546 456 flkdgdh`dfl3453454354334565463 5463 564 654 465 456 fdgjkldf fkgsjk 456654' '       ";
            Assert::IsTrue(my_reference_data == tmp);
            std::string file_content2{ GetFileInLine(reference_file_path, true) };
            Assert::IsTrue(reference_file_data == file_content2);
        }

        TEST_METHOD(ReadBinaryFile_Char_Test)
        {
            auto readed_data = ReadBinaryFile(reference_serialized_path_char);
            Assert::IsTrue(data_serialized_binary == readed_data);
            long long reversed_data{};
            memcpy(&reversed_data, &readed_data[0], readed_data.size());
            Assert::AreEqual(data_for_serialization, reversed_data);
            auto inp_stream = OpenFile<std::basic_ifstream<char>>(reference_serialized_path_char);
            std::vector<char> readed_data_stream = ReadBinaryFile(inp_stream);
            CloseFile(inp_stream);
            Assert::IsTrue(data_serialized_binary == readed_data_stream);
        }

        TEST_METHOD(WriteBinaryFile_Char_Test)
        {
            /*std::string second_func_path = "Files/Test Serialized Char Symbol Adress.dat";
            WriteBinaryFile<unsigned char>(test_serialized_path_char, data_serialized, OpenModeWriteBinaryRewrite);
            WriteBinaryFile(second_func_path, &data_serialized[0],
                data_serialized.size(), OpenModeWriteBinaryRewrite);

            auto out_stream = OpenFile<std::basic_ofstream<unsigned char>>(test_serialized_path_char);
            WriteBinaryFile<unsigned char>(test_serialized_stream_path_char, data_serialized, OpenModeWriteBinaryRewrite);
            CloseFile(out_stream);

            auto readed_data = ReadBinaryFile(test_serialized_path_char);
            Assert::IsTrue(data_serialized == readed_data);
            auto readed_data_byte = ReadBinaryFile<std::byte>(test_serialized_path_char);
            Assert::IsTrue(data_serialized_byte == readed_data_byte);
            auto readed_data2 = ReadBinaryFile(second_func_path);
            Assert::IsTrue(data_serialized == readed_data2);
            auto readed_data_symbol = ReadBinaryFile(test_serialized_path_char);
            Assert::IsTrue(data_serialized == readed_data_symbol);

            long long reversed_data{};
            memcpy(&reversed_data, &readed_data[0], readed_data.size());
            Assert::AreEqual(data_for_serialization, reversed_data);

            auto inp_stream = OpenFile<std::basic_ifstream<unsigned char>>(test_serialized_stream_path_char);
            std::vector<unsigned char> readed_data_stream = ReadBinaryFile(inp_stream);
            CloseFile(inp_stream);
            Assert::IsTrue(data_serialized == readed_data_stream);*/
        }


    private:
        static const std::string reference_file_path;
        static const std::string reference_file_data; 

        static const std::string clear_file_path;
        static const std::string reference_file_path_wchrt;
        static const std::wstring reference_file_data_wchrt;

        static const std::string reference_bigfile_path;

        const std::string reference_big_result_char = "Files/Война и мир TestChar.txt";
        const std::string reference_big_result_wchrt = "Files/Война и мир TestWchrt.txt";
        static const std::string reference_serialized_path_char;
        static const std::string reference_serialized_path_wchrt;

        static const std::string test_serialized_path_char;
        static const std::string test_serialized_stream_path_char;
        static const std::string test_serialized_path_wchrt;

        static long long data_for_serialization;
        static std::vector<unsigned char> data_serialized;
        static std::vector<char> data_serialized_binary;
        static std::vector<wchar_t> data_serialized_wchrt;
        static std::vector<std::byte> data_serialized_byte;
    };

    const std::string FileStreamTest::reference_file_path = std::string("Files/Reference.data");
    const std::string FileStreamTest::reference_file_data = "Text for Test. Have fun. Mother fucker!\n1234234\n34534345  345453435\n1213\n2\n3\n4\n5\n6546546 456 flkdgdh`dfl\n\n\n\n\n\n34534543543\n34565463 5463 564 654 465\n\n\n\n 456 fdgjkldf fkgsjk 456654' ' \n \n\n     \n\n";
    const std::string FileStreamTest::clear_file_path = std::string("Files/Clear.data");

    const std::string FileStreamTest::reference_file_path_wchrt = std::string("Files/Reference_wchrt.data");
    const std::wstring FileStreamTest::reference_file_data_wchrt = std::wstring(L"Text for Test. Have fun. Mother fucker!\n1234234\n34534345  345453435\n1213\n2\n3\n4\n5\n6546546 456 flkdgdh`dfl\n\n\n\n\n\n34534543543\n34565463 5463 564 654 465\n\n\n\n 456 fdgjkldf fkgsjk 456654' ' \n \n\n     \n\n _wchrt");

    const std::string FileStreamTest::reference_bigfile_path = "Files/Война и мир.txt";

    long long FileStreamTest::data_for_serialization{ 12345678906666666 };
    std::vector<unsigned char> FileStreamTest::data_serialized{};
    std::vector<char> FileStreamTest::data_serialized_binary;
    std::vector<wchar_t> FileStreamTest::data_serialized_wchrt{};
    std::vector<std::byte> FileStreamTest::data_serialized_byte{};

    const std::string FileStreamTest::reference_serialized_path_char = "Files/Reference Serialized Char.dat";
    const std::string FileStreamTest::reference_serialized_path_wchrt = "Files/Reference Serialized Wchrt.dat";

    const std::string FileStreamTest::test_serialized_path_char = "Files/Test Serialized Char.dat";
    const std::string FileStreamTest::test_serialized_stream_path_char = "Files/Test Serialized Stream.dat";
    const std::string FileStreamTest::test_serialized_path_wchrt = "Files/Test Serialized Wchrt.dat";
}

/* std::vector<int> my_vect{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
 std::vector<int> my_vect2{};
 auto sz1 = sizeof std::move(my_vect);
 auto sz2 = sizeof std::move(my_vect2);

 WriteBinaryInFile<std::vector<int>, wchar_t>("WriteBinaryInFile.data", &my_vect[0], sizeof(my_vect[0])*my_vect.size(),
                                     OpenModeWriteBinaryRewrite);
 std::vector<int> vect_readed_from_file = ReadBinaryFromFile<std::vector<int>, wchar_t>("WriteBinaryInFile.data");

 for (int &vec_member : vect_readed_from_file) {
     std::cout << vec_member << ' ';
 }
 std::cout << '\n';*/
