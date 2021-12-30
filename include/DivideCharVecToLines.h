#ifndef DIVIDECHARVECTOLINES_H
#define DIVIDECHARVECTOLINES_H

#include <vector>

namespace file {

    // Divede vector of char to lines & delete \r\n
    std::vector<std::vector<char>> DivideCharVecToLines(std::vector<char>& vec);

}

#endif // !DIVIDECHARVECTOLINES_H