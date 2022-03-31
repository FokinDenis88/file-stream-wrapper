#ifndef WRITE_SERIALIZED_DATA_FILE_HPP_
#define WRITE_SERIALIZED_DATA_FILE_HPP_

module;

#include <fstream>
#include <locale>
#include <type_traits>
#include <string>
#include <vector>

export module file.write_serialized_data_file;

import file.constants;
import file.service;
import file.errors_handler;

export {
	/// write, read files, another service operations based on fstream
	namespace file {

		// Size of array, vector for writing arrays in files
		template<typename ElemT>
		inline size_t SizeOfArray(const std::vector<ElemT>& array_p) {
			return sizeof(array_p[0]) * array_p.size();
		}
		// Size of array, vector for writing arrays in files
		template<typename ElemT>
		inline size_t SizeOfArray(const std::vector<ElemT>& array_p, const size_t array_elem_count) {
			return sizeof(array_p[0]) * array_elem_count;
		}


		//--------------------------------��������� �� ������ ���� �������

		template<typename ByteT>
		concept IsByte = std::is_same_v<ByteT, unsigned char> || std::is_same_v<ByteT, std::byte>;

		// �������� ������ ������� � ����: �� ������� �� ����� ������ ��, ��� �������.
		// ����� ������������ ����������� ����� �������� ��� �������� ������. OpenModeWriteBinaryAppend ��� OpenModeWriteBinaryRewrite.
		// first_object_symbol - ��� ��������� �� ������ ������ ��� ������ ������� ������ �������.
		// ��������, ��������� �� ������ ������� �������. reinterpret_cast<ByteT*>(&object[0])
		// object_size - ���������� ����, ������� �������� ������
		template<IsByte ByteT = unsigned char, typename SymbolT>
		inline void WriteSerializedDataFile(std::basic_ofstream<ByteT>& write_file_stream,
										 const SymbolT* const first_object_symbol, const std::streamsize object_size = sizeof(SymbolT),
										 std::ios_base::openmode open_mode = OpenModeWriteBinaryAppend,
										 const std::locale& locale = std::locale()) { // open_mode & locale is defined as default in forward declaration
			static_assert(std::is_same_v<ByteT, unsigned char> || std::is_same_v<ByteT, std::byte>,
						  "ByteT can be only: unsigned char or std::byte");
			write_file_stream.imbue(locale);

			if (object_size > 0) { // wofstream �� ����� ��������, ���� '\x0101' � ������� ����� �� ����� 00 = '\x0001'
				write_file_stream.write(reinterpret_cast<const ByteT*>(first_object_symbol), object_size);
				if (write_file_stream.bad()) { throw ErrorWriteFile(); }

				// ������ ���������� ��������������� � ����
				FlushFile(write_file_stream);
			}
		}

		// �������� ������ ������� � ����: �� ������� �� ����� ������ ��, ��� �������.
		// ����� ������������ ����������� ����� �������� ��� �������� ������. OpenModeWriteBinaryAppend ��� OpenModeWriteBinaryRewrite.
		// first_object_symbol - ��� ��������� �� ������ ������ ��� ������ ������� ������ �������.
		// ��������, ��������� �� ������ ������� �������. reinterpret_cast<ByteT*>(&object[0])
		// object_size - ���������� ����, ������� �������� ������
		template<IsByte ByteT = unsigned char, typename SymbolT>
		inline void WriteSerializedDataFile(const char* file_path, const SymbolT* const first_object_symbol,
										 const std::streamsize object_size = sizeof(SymbolT),
										 std::ios_base::openmode open_mode = OpenModeWriteBinaryAppend,
										 const std::locale& locale = std::locale()) { // open_mode & locale is defined as default in forward declaration
			static_assert(std::is_same_v<ByteT, unsigned char> || std::is_same_v<ByteT, std::byte>,
				"ByteT can be only: unsigned char or std::byte");
			std::basic_ofstream<ByteT> write_file_stream = OpenFile<std::basic_ofstream<ByteT>>(file_path, open_mode);
			WriteSerializedDataFile<ByteT>(write_file_stream, first_object_symbol, object_size, open_mode, locale);
			CloseFile(write_file_stream);
		}

		template<IsByte ByteT = unsigned char, typename SymbolT>
		inline void WriteSerializedDataFile(const std::string& file_path, const SymbolT* const first_object_symbol,
											 const std::streamsize object_size = sizeof(SymbolT),
											 std::ios_base::openmode open_mode = OpenModeWriteBinaryAppend,
											 const std::locale& locale = std::locale()) {
			static_assert(std::is_same_v<ByteT, unsigned char> || std::is_same_v<ByteT, std::byte>,
						  "ByteT can be only: unsigned char or std::byte");
			if (object_size > 0) {
				WriteSerializedDataFile<ByteT>(file_path.c_str(), first_object_symbol, object_size, open_mode, locale);
			}
		}


		//-------------------------------------------������ � ��������� ����

		// TODO: ������ ��������� wchar_t. �������� ���������� ����.

		// ���������� ����� ������������ ������ � ����������� ��� �� ��������� ������� � ������ char � ������� memcpy ��� bit_cast
		template<typename ByteT = unsigned char>
		inline void WriteSerializedDataFile(std::basic_ofstream<unsigned char>& write_file_stream, const std::vector<ByteT>& data,
											 std::ios_base::openmode open_mode = OpenModeWriteBinaryAppend,
											 const std::locale& locale = std::locale()) {
			static_assert(std::is_same<ByteT, unsigned char>::value || std::is_same<ByteT, wchar_t>::value,
				"ByteT can be only: char, wchar_t");
			if (data.size() > 0) {
				WriteSerializedDataFile<unsigned char>(write_file_stream, &data[0], data.size() * sizeof(ByteT), open_mode, locale);
			}
		}
		inline void WriteSerializedDataFile(std::basic_ofstream<std::byte>& write_file_stream, const std::vector<std::byte>& data,
											 std::ios_base::openmode open_mode = OpenModeWriteBinaryAppend,
											 const std::locale& locale = std::locale()) {
			if (data.size() > 0) {
				WriteSerializedDataFile<std::byte>(write_file_stream, &data[0], data.size() * sizeof(std::byte), open_mode, locale);
			}
		}

		// ���������� ����� ������������ ������ � ����������� ��� �� ��������� ������� � ������ char � ������� memcpy ��� bit_cast
		template<typename ByteT = unsigned char>
		inline void WriteSerializedDataFile(const char* file_path, const std::vector<ByteT>& data,
											 std::ios_base::openmode open_mode = OpenModeWriteBinaryAppend,
											 const std::locale& locale = std::locale()) {
			static_assert(std::is_same<ByteT, unsigned char>::value || std::is_same<ByteT, wchar_t>::value
				|| std::is_same<ByteT, std::byte>::value, "ByteT can be only: char, wchar_t or std::byte");
			if (data.size() > 0) {
				if (std::is_same<ByteT, std::byte>::value) { // ���� �� ����� �������������� wchar_t
					WriteSerializedDataFile(file_path, &data[0], data.size() * sizeof(std::byte), open_mode, locale);
				}
				else {
					WriteSerializedDataFile<unsigned char>(file_path, &data[0], data.size() * sizeof(ByteT), open_mode, locale);
				}
			}
		}

		// ���������� ����� ������������ ������ � ����������� ��� �� ��������� ������� � ������ char � ������� memcpy ��� bit_cast
		template<typename ByteT = unsigned char>
		inline void WriteSerializedDataFile(const std::string& file_path, const std::vector<ByteT>& data,
											 std::ios_base::openmode open_mode = OpenModeWriteBinaryAppend,
											 const std::locale& locale = std::locale()) {
			static_assert(std::is_same<ByteT, unsigned char>::value || std::is_same<ByteT, wchar_t>::value
				|| std::is_same<ByteT, std::byte>::value, "ByteT can be only: char, wchar_t or std::byte");
			if (data.size() > 0) {
				if (std::is_same<ByteT, std::byte>::value) { // ���� �� ����� �������������� wchar_t
					WriteSerializedDataFile(file_path.c_str(), &data[0], data.size() * sizeof(ByteT), open_mode, locale);
				}
				else {
					WriteSerializedDataFile<unsigned char>(file_path.c_str(), &data[0], data.size() * sizeof(ByteT), open_mode, locale);
				}
			}
		}
	}
}

#endif // !WRITE_SERIALIZED_DATA_FILE_HPP_
