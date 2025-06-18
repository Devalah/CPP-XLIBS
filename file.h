#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <istream>
#include <ostream>
#include <tuple>
#include <algorithm>
#include <bitset>
#include <iomanip>
#include <cmath>
#include <sstream>
#include <cstdint>
#include <fstream>

#include "list.h"
#include "str.h"

namespace File {
    inline bool WriteFileBytes(const str& path, const list<uint8_t>& bytes) {
        std::ofstream file(path.val, std::ios::binary);
        if (!file.is_open()) return false;

        for (int16_t i = 0; i < bytes.count; ++i)
            file.put(static_cast<char>(bytes.At(i)));

        return file.good();
    }

    inline list<uint8_t> ReadFileBytes(const str& path) {
        list<uint8_t> out;
        std::ifstream file(path.val, std::ios::binary);
        if (!file.is_open()) return out;

        file.seekg(0, std::ios::end);
        std::streamsize size = file.tellg();
        if (size < 0) return out;
        file.seekg(0, std::ios::beg);

        for (std::streamsize i = 0; i < size; ++i) {
            char byte;
            file.get(byte);
            if (file.eof()) break;
            out.Add(static_cast<uint8_t>(byte));
        }

        if (file.good() || file.eof())
            return out;

        throw std::exception("Reading bytes of file failed");
    }

    inline str ReadFileText(const str& path) {
        std::ifstream file(path.val);
        if (!file.is_open()) throw std::exception("Could not open file for reading text");

        std::stringstream buffer;
        buffer << file.rdbuf();

        if (file.good() || file.eof())
            return buffer.str().c_str();

        throw std::exception("Reading text of file failed");
    }
};

using namespace File;