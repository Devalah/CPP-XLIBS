#pragma once

#include <iostream>
#include <string>

namespace print {
    inline void wr(const std::string& msg) {
        std::cout << msg;
    }

    inline void wrl(const std::string& msg) {
        wr(msg + '\n');
    }

    inline std::string rdl() {
        std::string input;
        std::cin >> input;
        return input;
    }
}

using print::wr;
using print::wrl;
using print::rdl;