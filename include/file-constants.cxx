#ifndef FILE_CONSTANTS_HPP_
#define FILE_CONSTANTS_HPP_

module;

#include <fstream>

export module file.constants;

// write, read files, another service operations based on fstream
export {
	namespace file {
		constexpr std::ios_base::openmode OpenModeWriteAppend = std::ios_base::out | std::ios_base::app;

		constexpr std::ios_base::openmode OpenModeWriteRewrite = std::ios_base::out | std::ios_base::trunc;

		constexpr std::ios_base::openmode OpenModeReadFromBegin = std::ios_base::in;

		constexpr std::ios_base::openmode OpenModeWriteBinaryAppend = std::ios_base::out | std::ios_base::binary | std::ios_base::app;

		constexpr std::ios_base::openmode OpenModeWriteBinaryRewrite = std::ios_base::out | std::ios_base::binary | std::ios_base::trunc;

		constexpr std::ios_base::openmode OpenModeReadBinary = std::ios_base::in | std::ios_base::binary;

		constexpr std::ios_base::openmode OpenModeReadWriteBinary = std::ios_base::in | std::ios_base::out | std::ios_base::binary;
	}
}

#endif // !FILECONSTANTS_HPP_
