#ifndef BOM_HPP_
#define BOM_HPP_

module;

#include <fstream>
#include <vector>
#include <bit>

export module file.bom;

export {
	namespace file {
		// Enum ��� ByteOrderMark
		enum class BOMEnum : size_t {
			UTF_32BE = 0, // Big endian             0
			UTF_32LE, // Little endian          1
			UTF_16BE, //                        2
			UTF_16LE, //                        3
			UTF_8,    //                        4
			No_BOM    //                        5
		};

		//Byte Order Mark (BOM)
		//00 00 FE FF	    UTF - 32BE, big - endian
		//FF FE 00 00	    UTF - 32LE, little - endian
		//FE FF	            UTF - 16BE, big - endian
		//FF FE	            UTF - 16LE, little - endian
		//EF BB BF	        UTF - 8
		// Order of ByteOrderMark is associative with order of BOMEnum
		const std::vector<std::vector<char>> ByteOrderMark{ // literals are chars, so can't use unsigned char
			std::vector{ '\x00', '\x00', '\xfe', '\xff' },              // UTF_32BE         0
			std::vector{ '\xff', '\xfe', '\x00', '\x00' },              // UTF_32LE         1
			std::vector{ '\xfe', '\xff' },                              // UTF_16BE         2
			std::vector{ '\xff', '\xfe' },                              // UTF_16LE         3
			std::vector{ '\xef', '\xbb', '\xbf' }                       // UTF_8            4
		};

		// Variants of platform Endianness
		enum class SystemEndianness {
			Big_endian = 0,
			Little_endian,
			Mixed, // std::endian::native equals neither std::endian::big nor std::endian::little
			Not_matter // endianness does not matter and all three values, std::endian::little, std::endian::big, and std::endian::native are the same
		};

		// GetSystemBOM()

		// ���������� enum � �������� ���� ����������� ��� ������� ���������
		inline SystemEndianness GetSystemEndianness() {
			if constexpr (std::endian::native == std::endian::big && std::endian::native == std::endian::little) {
				// endianness does not matter and all three values, std::endian::little, std::endian::big, and std::endian::native are the same
				return SystemEndianness::Not_matter;
			}
			else if constexpr (std::endian::native == std::endian::big) {
				return SystemEndianness::Big_endian;
			}
			else if constexpr (std::endian::native == std::endian::little) {
				return SystemEndianness::Little_endian;
			}
			else {
				// std::endian::native equals neither std::endian::big nor std::endian::little
				return SystemEndianness::Mixed;
			}
		}

		// ������ � ���� ����� ������� ���� � ����� Byte Order Mark
		template<typename FileStreamT = std::ifstream>
		inline void WriteBOM(FileStreamT& file_stream, const BOMEnum bom) {
			if (file_stream.is_open()) {
				if (bom != BOMEnum::No_BOM) {
					size_t index{ static_cast<size_t>(bom) };
					file_stream.write(&ByteOrderMark[index][0], ByteOrderMark[index].size());
					FlushFile(file_stream);
				}
			} else { throw ErrorFileStreamIsClosed(); }
		}

		// ������ �� ����� ����� ������� ���� � ����� Byte Order Mark
		template<typename FileStreamT = std::ifstream>
		inline BOMEnum ReadBOM(FileStreamT& file_stream) {
			if (file_stream.is_open()) {
				std::vector<char> readed_data(2);
				file_stream.read(&readed_data[0], 2); // Readed 2 first bytes
				if (readed_data == ByteOrderMark[2]) { return BOMEnum::UTF_16BE; }
				else if (readed_data == ByteOrderMark[3]) { return BOMEnum::UTF_16LE; }

				readed_data.resize(3);
				file_stream.read(&readed_data[2], 1); // Readed 3 byte
				if (readed_data == ByteOrderMark[4]) { return BOMEnum::UTF_8; }

				readed_data.resize(4);
				file_stream.read(&readed_data[3], 1); // Readed 4 byte
				if (readed_data == ByteOrderMark[0]) { return BOMEnum::UTF_32BE; }
				else if (readed_data == ByteOrderMark[1]) { return BOMEnum::UTF_32LE; }
				else {
					return BOMEnum::No_BOM;
					file_stream.seekg(0, std::ios_base::beg); // ����������� ��������� �� ������ �����
				}
			} else { throw ErrorFileStreamIsClosed(); }
		}

		// ����������� Big endian <|> Little endian. ������ ������� 1 � 2 ���� � ������ swap_width ������.
		// swap_scope - ������ ������ �������. ��������, � ������� ���������� ������� ������ 2 �����.
		template<typename ByteT>
		inline void SwapEndianness(std::vector<ByteT>& data, const size_t swap_scope) {
			/* static_assert(std::is_same<ByteT, unsigned char>::value || std::is_same<ByteT, std::byte>::value,
						   "CharT can be only: unsigned char or std::byte");*/
			if (swap_scope > 1) { // ����� ����� ������ ������� 2 � ����� �����
				{
					long long sz = data.size();
					int remainder = std::div(sz, static_cast<long long>(swap_scope)).rem;
					data.resize(sz + remainder);
				}
				ByteT temp{};
				const int last_index = swap_scope - 1;
				int first_index{};
				int sub{};
				for (int i = 0, max = data.size(); i < max;) {
					for (int j = 0; j < swap_scope / 2; ++j) {
						first_index = i + j;
						int sub = i - j;
						temp = data[first_index];
						data[first_index] = data[sub + last_index];
						data[sub + last_index] = temp;
					}
					i += swap_scope;
				}
			}
			else if (swap_scope == 1) { // � 1 ����� �������� ������� ��� ����. 8 ��� ���������� 1-��.
				for (ByteT& elem : data) {
					elem = ~elem;
					int a = 0;
				}
			}
		}
	}
}

#endif // !BOM_HPP_
