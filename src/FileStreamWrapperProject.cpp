// TODO: Может быть записывать игру должен не основной поток игры.
// TODO: Запись игры на лету - как сделать что-то, подобное Dragon Age: Inquizition
// TODO: WriteDataFile и все другие функции используют текущую директорию проекта. Отвязать от нее все функции
//

#include <iostream>
#include <vector>
#include <array>


#include "FileOperations.h"

#include "BOM.h"
#include "FileConstants.h"
#include "FileService.h"
#include "ReadDataFile.h"
#include "ReadDataFileThread.h"
#include "ReadSerializedDataFile.h"
#include "ReadSerializedDataFileThread.h"
#include "ReadTextFile.h"
#include "ReadTextFileBig.h"
#include "ReadTextFileBigThread.h"
#include "ReadTextFileThread.h"
#include "WriteDataFile.h"
#include "WriteDataFileThread.h"
#include "WriteSerializedDataFileThread.h"
#include "WriteSerializedDataFile.h"
#include "WriteTextFile.h"
#include "WriteTextFileBig.h"
#include "WriteTextFileBigThread.h"
#include "WriteTextFileThread.h"


//template
//void WriteDataFile<wchar_t, std::wstring>(const std::string&, const std::wstring&,
//                                            std::ios_base::openmode open_mode, const std::locale& locale);

template <typename ReturnT = void>
ReturnT doShit1(const char (&path)[]) {
}

void doShit2(std::string & path) {
}

int main() {
    try {
        //doShit2("sdf");

        /*WriteDataFile("Files/WriteDataFile.data", "WriteTextInFileData", OpenModeWriteRewrite);
        std::cout << ReadAllTextFromFile("Files/WriteDataFile.data") << '\n';*/

        /*WriteDataFile("Files/WriteDataInFileChar.data", "Text for Test. Have fun. Mother fucker!", OpenModeWriteRewrite);
        std::cout << ReadAllTextFromFile<char>("Files/WriteDataInFileChar.data") << '\n';*/

        /*std::cout << ReadTextFile<char>("Files/!My Reference Text.txt") << '\n';

        WriteSerializedDataFile<char>("sdfds", std::vector<char>{'s','a','s'});*/

        auto str = file::ReadTextFile("Files/Clear.data");

        /* std::wofstream write_file_stream_serial_wchrt("Files/Wchrt.dat");
         long long data_for_serialization = 666546546345654;
         std::vector<wchar_t> data_serialized_wchrt{};
         if (write_file_stream_serial_wchrt) {
             auto data = std::bit_cast<std::array<wchar_t, sizeof(data_for_serialization) / sizeof(wchar_t)>>(data_for_serialization);
             data_serialized_wchrt.reserve(data.size());
             for (const auto& elem : data) {
                 data_serialized_wchrt.push_back(elem);
             }
             write_file_stream_serial_wchrt.write(&data[0], data.size());
             bool bad = write_file_stream_serial_wchrt.bad();
             bool fail = write_file_stream_serial_wchrt.fail();
             bool good = write_file_stream_serial_wchrt.good();
             int a = 0;
         }
         write_file_stream_serial_wchrt.close();*/

        //std::basic_ofstream<wchar_t> write_file_stream_serial_wchrt("Files/Wchrt.dat", std::ios_base::out | std::ios_base::trunc);
        std::wofstream write_file_stream_serial_wchrt("Files/Wchrt.dat", std::ios_base::out | std::ios_base::binary |                                                                             std::ios_base::trunc);

        /*std::wstring stroka{ L"kldsjklfdsjksdlfjklfsdjksdlfjfksd\n" };
        std::vector<wchar_t> vect{};
        for (wchar_t& elem : stroka) {
            vect.push_back(elem);
        }*/

        //std::locale loc(std::locale().classic(), new std::codecvt<char16_t, char8_t, mbstate_t>);
        //std::locale loc(std::locale(), new std::codecvt<wchar_t, wchar_t, mbstate_t>);
        std::locale loc(std::locale(), new std::messages<wchar_t>);
        write_file_stream_serial_wchrt.imbue(loc);
        //std::locale::global(std::locale(""));
        //std::locale loc(std::locale("en_US.UTF-16LE"));
        //write_file_stream_serial_wchrt.imbue(std::locale());
        //std::array<wchar_t, 1> arr{ L'\x01ff' };//, L'2', L'3', L'4' };
        const wchar_t ch = L'\x0100';
        char16_t ch16 = L'\x0100';
        auto sz = sizeof(ch);
        auto lc = write_file_stream_serial_wchrt.getloc();
        auto wd = write_file_stream_serial_wchrt.width();
        /*write_file_stream_serial_wchrt.exceptions(std::ofstream::failbit | std::ofstream::badbit);
        auto excep = write_file_stream_serial_wchrt.exceptions();*/

        const std::wstring wstr = L"\x0102\x0304\x0506\x0708";
        std::wstring wstr_test = L"Test";
        bool bad = write_file_stream_serial_wchrt.bad();
        bool fail = write_file_stream_serial_wchrt.fail();
        bool eof = write_file_stream_serial_wchrt.eof();
        bool good = write_file_stream_serial_wchrt.good();
        if (write_file_stream_serial_wchrt) {
            //write_file_stream_serial_wchrt.write(&vect[0], vect.size());
            //write_file_stream_serial_wchrt.write(&arr[0], 1);

            //write_file_stream_serial_wchrt.write(&arr[0], arr.size());
            //write_file_stream_serial_wchrt << ch;
            //write_file_stream_serial_wchrt.write(reinterpret_cast<const char*>(&ch), sizeof(wchar_t)*1);


            //write_file_stream_serial_wchrt.write(reinterpret_cast<const char*>(&wstr[0]), sizeof(wchar_t) * wstr.size());
            write_file_stream_serial_wchrt.write(&wstr[0], wstr.size());


            /*write_file_stream_serial_wchrt.write(wstr.c_str(), 4);
            write_file_stream_serial_wchrt.write(wstr_test.c_str(), 4);
            write_file_stream_serial_wchrt << wstr;
            write_file_stream_serial_wchrt << wstr_test;*/

            //write_file_stream_serial_wchrt.write(&ch16, 1);
            //write_file_stream_serial_wchrt.write(reinterpret_cast<wchar_t*>(&ch16), 1);
            //write_file_stream_serial_wchrt << std::bit_cast<wchar_t>(ch16);
        }
        bad = write_file_stream_serial_wchrt.bad();
        fail = write_file_stream_serial_wchrt.fail();
        eof = write_file_stream_serial_wchrt.eof();
        good = write_file_stream_serial_wchrt.good();
        write_file_stream_serial_wchrt.close();

        signed char charrr = 'd';
        unsigned char jk = charrr;

        int a = 0;
    }
    // Обработка исключений и вывод сообщения об ошибке в консоль
    catch (file::open_file_error& error) {
        std::cerr << '\n' << error.what();
        return -1;
    }
    catch (const file::close_file_error& error) {
        std::cerr << '\n' << error.what();
        return -1;
    }
    catch (const file::write_file_error& error) {
        std::cerr << '\n' << error.what();
        return -1;
    }
    catch (const file::read_file_error& error) {
        std::cerr << '\n' << error.what();
        return -1;
    }
    catch (const file::file_operation_error& error) {
        std::cerr << '\n' << error.what();
        return -1;
    }
    catch (const std::logic_error& error) {
        std::cerr << error.what() << '\n';
        return -1;
    }
    catch (const std::runtime_error& error) {
        std::cerr << error.what() << '\n';
        return -1;
    }
    catch (...) {
        std::cerr << "Fatal Error\n";
        return -1;
    }

    return 0;
}