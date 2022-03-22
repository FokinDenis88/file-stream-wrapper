#ifndef READSTREAMSTATE_H
#define READSTREAMSTATE_H

#include <fstream>
#include <map>

namespace file {

    // Returns all states bit in bool array
    // Service function for debugging
    template<typename FileStreamT = std::ifstream>
    std::map<std::string, bool> ReadStreamState(FileStreamT& stream) {
        std::map<std::string, bool> state{};
        if (stream.eof()) { state.emplace("eof", true); }
        else { state.emplace("eof", false); }

        if (stream.bad()) { state.emplace("bad", true); }
        else { state.emplace("bad", false); }

        if (stream.fail()) { state.emplace("fail", true); }
        else { state.emplace("fail", false); }

        if (stream.good()) { state.emplace("good", true); }
        else { state.emplace("good", false); }
        return state;
    }

}

#endif // !READSTREAMSTATE_H