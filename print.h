#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <exception>
#include <cstdio>

#include "str.h"

enum Color { black, white, red, green, yellow, blue, magenta, cyan, normal };
constexpr const char* AsString(Color color) {
    switch (color) {
    case black: return "\033[30m";
    case red: return "\033[31m";
    case green: return "\033[32m";
    case yellow: return "\033[33m";
    case blue: return "\033[34m";
    case magenta: return "\033[35m";
    case cyan: return "\033[36m";
    case white: return "\033[37m";
    case normal: return "\033[0m";
    }
    return "\033[0m";
}
using ::Color;

namespace print {
    void wr(const str& msg) {
        printf("%s", msg.val);
    }

    void wr(const str& msg, Color color) {
        printf("%s%s%s", AsString(color), msg.val, AsString(normal));
    }

    void wrl(const str& msg) {
        wr(msg + '\n');
    }

    void wrl(const str& msg, Color color) {
        wr(msg + '\n', color);
    }

    void err(const str& msg) {
        str s("[ERROR]: ");
        s += (msg + '\n');
        wrl(s, red);
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