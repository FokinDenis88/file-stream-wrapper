#include "DivideCharVecToLines.h"

#include <iterator>

namespace file {

    std::vector<std::vector<char>> DivideCharVecToLines(std::vector<char>& vec) {
        std::vector<std::vector<char>> new_vec{};
        if (vec.size() > 0) {
            const std::vector<char>::const_iterator cbegin{ vec.cbegin() };
            const std::vector<char>::const_iterator cend{ vec.cend() };
            std::vector<char>::const_iterator first_char{ vec.cbegin() };
            std::vector<char>::const_iterator last_char{ vec.cbegin() };
            bool return_flag{ true };
            do {
                while (last_char != cend && *last_char != '\n') {
                    ++last_char;
                }
                if (last_char == cbegin || *std::prev(last_char) != '\r') { return_flag = false; } // no \r character before \n character
                else { return_flag = true; } // \r\n sequence

                if (first_char == last_char && first_char == cbegin) { // first character is \n without \r
                    new_vec.emplace_back(std::vector<char>{});
                } else {
                    if (return_flag) { // there is \r character, that must be deleted
                        new_vec.emplace_back(std::vector<char>(first_char, std::prev(last_char)));
                    } else { new_vec.emplace_back(std::vector<char>(first_char, last_char)); }
                }

                first_char = ++last_char;
            } while (last_char != cend);
        }
        return new_vec;
    }

}