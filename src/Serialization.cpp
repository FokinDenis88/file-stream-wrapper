#include "Serialization.h"

// https://isocpp.org/wiki/faq/serialization
//1) The first step is to make an eyes - open decision between text - and binary - formats.
//2) If your objects aren’t part of an inheritance hierarchy and don’t contain pointers, use solution #1.
//3) Else if your objects don’t contain pointers to other objects, use solution #2.
//4) Else if the graph of pointers within your objects contain neither cycles nor joins, use solution #3.
//5) Else if the graph of pointers within your objects don’t contain cyclesand if the only joins are to terminal(leaf) nodes, use solution #4.
//6) Else use solution #5.

namespace file {
}