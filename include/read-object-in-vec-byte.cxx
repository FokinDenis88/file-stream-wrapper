#ifndef READ_OBJECT_IN_VEC_BYTE_HPP_
#define READ_OBJECT_IN_VEC_BYTE_HPP_

module;

#include <vector>
#include <cstring> // memcpy

export {
	namespace file {

		// Read object from vector of byte
		// obj_first_byte_index - index in vector of bytes
		template<typename DestT>
		inline void ReadObjectInVecByte(DestT* dest, std::vector<unsigned char>& readed_data,
										long long& obj_first_byte_index, size_t obj_size = sizeof(DestT)) {
			std::memcpy(dest, &readed_data[obj_first_byte_index], obj_size);
			obj_first_byte_index += obj_size;
		}

		// Read object from vector of byte
		// obj_first_byte_index - index in vector of bytes
		// Version with return value
		template<typename DestT>
		inline DestT ReadObjectInVecByteR(std::vector<unsigned char>& readed_data, long long& obj_first_byte_index, size_t obj_size) {
			DestT new_obj;
			std::memcpy(&new_obj, &readed_data[obj_first_byte_index], obj_size);
			obj_first_byte_index += obj_size;
			return new_obj;
		}
	}
}

#endif // !READ_OBJECT_IN_VEC_BYTE_HPP_
