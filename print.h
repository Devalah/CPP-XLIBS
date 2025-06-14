#pragma once

#include <iostream>
#include <string>
#include <cstring>

#include "str.h"

namespace print {
    void wr(const str& msg) {
        std::cout << msg;
    }

    void wrl(const str& msg) {
        wr(msg + '\n');
    }

    void err(const str& msg) {
        str s("[ERROR]: ");
        s += (msg + '\n');
        wr(s.val);
    }

    const str rdl() {
        std::string input;
        std::cin >> input;
        return input.c_str();
    }

    bool rdl(str& out) {
        std::string input;
        std::cin >> input;
        out = str(input.c_str());
        return input.length() > 0;
    }
}

using namespace print;