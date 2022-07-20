#ifndef FILE_TYPES_HPP_
#define FILE_TYPES_HPP_

#include <fstream>

// write, read files, another service operations based on fstream
namespace file {
    template<typename CharT>
    concept CharsType = std::same_as<char, CharT> || std::same_as<wchar_t, CharT>;

    template<typename CharsType>
    using StringCharT = std::basic_string<CharsType>;


}

#endif // !FILE_TYPES_HPP_