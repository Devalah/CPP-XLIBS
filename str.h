#pragma once

#include <cstring>
#include <float.h>

#include "list.h"

struct str {
public:
    char* val;

    str() {
        val = new char[1];
        val[0] = '\0';
    }

    uint16_t length() const {
        return static_cast<uint16_t>(std::strlen(val));
    }

    str substr(uint16_t start, uint16_t step) const {
        uint16_t str_len = std::strlen(val);
        if (start >= str_len)
            return str("");

        uint16_t count = 0;
        for (uint16_t i = 0; i < str_len; ++i)
            if (i >= start && ((i - start) % step == 0))
                ++count;

        char* buffer = new char[count + 1];
        uint16_t index = 0;

        for (uint16_t i = 0; i < str_len; ++i) {
            if (i >= start && ((i - start) % step == 0)) {
                buffer[index++] = val[i];
            }
        }

        buffer[index] = '\0';
        str result(buffer);
        delete[] buffer;
        return result;
    }

    bool find(const char& ch) const {
        for (int i = static_cast<int>(std::strlen(val)) - 1; i >= 0; --i) {
            if (val[i] == ch) {
                return true;
            }
        }
        return false;
    }


    bool find(const str& ch) const {
        uint16_t val_len = std::strlen(val);
        uint16_t ch_len = std::strlen(ch.val);

        if (ch_len == 0 || ch_len > val_len)
            return false;

        for (uint16_t i = 0; i <= val_len - ch_len; ++i) {
            bool match = true;
            for (uint16_t j = 0; j < ch_len; ++j) {
                if (val[i + j] != ch[j]) {
                    match = false;
                    break;
                }
            }
            if (match)
                return true;
        }

        return false;
    }

    str(const char* value) {
        val = new char[std::strlen(value) + 1];
        strcpy_s(val, std::strlen(value) + 1, value);
    }

    str(const str& other) {
        val = new char[std::strlen(other.val) + 1];
        strcpy_s(val, std::strlen(other.val) + 1, other.val);
    }

    str(str&& other) noexcept : val(other.val) {
        other.val = nullptr;
    }

    ~str() {
        delete[] val;
    }

    char& operator[](size_t index) {
        return val[index];
    }

    const char& operator[](size_t index) const {
        return val[index];
    }

    str& operator=(const str& other) {
        if (this != &other) {
            char* new_val = new char[std::strlen(other.val) + 1];
            strcpy_s(new_val, std::strlen(other.val) + 1, other.val);
            delete[] val;
            val = new_val;
        }
        return *this;
    }

    str& operator=(str&& other) noexcept {
        if (this != &other) {
            delete[] val;
            val = other.val;
            other.val = nullptr;
        }
        return *this;
    }

    str& operator+=(const str& other) {
        uint16_t len1 = std::strlen(val);
        uint16_t len2 = std::strlen(other.val);
        char* nb = new char[len1 + len2 + 1];

        strcpy_s(nb, len1 + len2 + 1, val);
        strcat_s(nb, len1 + len2 + 1, other.val);

        delete[] val;
        val = nb;
        return *this;
    }

    str& operator+=(const char* cstr) {
        uint16_t len1 = std::strlen(val);
        uint16_t len2 = std::strlen(cstr);
        char* nb = new char[len1 + len2 + 1];

        strcpy_s(nb, len1 + len2 + 1, val);
        strcat_s(nb, len1 + len2 + 1, cstr);

        delete[] val;
        val = nb;
        return *this;
    }

    str& operator+=(char ch) {
        char cstr[2] = { ch, '\0' };
        return *this += cstr;
    }

    str operator+(const str& other) const {
        str result(*this);
        result += other;
        return result;
    }

    str operator+(const char* cstr) const {
        str result(*this);
        result += cstr;
        return result;
    }

    str operator+(char ch) const {
        str result(*this);
        result += ch;
        return result;
    }

    friend str operator+(const char* lhs, const str& rhs) {
        str result(lhs);
        result += rhs;
        return result;
    }

    bool operator==(const str& other) const {
        return std::strcmp(val, other.val) == 0;
    }

    bool operator==(const char* cstr) const {
        return std::strcmp(val, cstr) == 0;
    }

    bool operator==(list<const char*> cstr) const {
        bool ret = false;

        for (auto& s : cstr)
            if (strcmp(val, s) == 0)
                ret = true;

        return ret;
    }

    friend bool operator==(list<const char*> lhs, const str& rhs) {
        bool ret = false;

        for (auto& s : lhs)
            if (strcmp(rhs.val, s) == 0)
                ret = true;

        return ret;
    }

    friend bool operator==(const char* lhs, const str& rhs) {
        return std::strcmp(lhs, rhs.val) == 0;
    }

    friend std::ostream& operator<<(std::ostream& os, const str& s) {
        return os << s.val;
    }

    char* begin() {
        return val;
    }

    char* end() {
        return val + std::strlen(val);
    }

    const char* begin() const {
        return val;
    }

    const char* end() const {
        return val + std::strlen(val);
    }
};
