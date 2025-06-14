#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <float.h>
#include <istream>
#include <ostream>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <cstdint>

struct UDecimal {
private:
    void Normalize() {
        uint32_t scale = FromDecimals(deciCount);
        val = (val > scale * 1000000) ? val % (scale * 1000000) : val;
    }

public:

    uint32_t val;
    uint8_t deciCount;

    static uint16_t FromDecimals(uint8_t decimals) {
        switch (decimals) {
        case 1: return 10;
        case 2: return 100;
        case 3: return 1000;
        case 4: return 10000;
        default: return 1000;
        }
    }

    UDecimal(float value, uint8_t decimals = 2)
        : deciCount(decimals) {
        val = static_cast<uint32_t>(roundf(value * FromDecimals(deciCount)));
        Normalize();
    }

    UDecimal(uint32_t rawVal, uint8_t decimals)
        : val(rawVal), deciCount(decimals) {
        Normalize();
    }

    std::string ToString() const {
        uint32_t scale = FromDecimals(deciCount);
        uint32_t intPart = val / scale;
        uint32_t fracPart = val % scale;

        std::ostringstream oss;
        oss << intPart << "." << std::setfill('0') << std::setw(deciCount) << fracPart;
        return oss.str();
    }

    float ToFloat() const {
        return static_cast<float>(val) / FromDecimals(deciCount);
    }

    UDecimal operator+(const UDecimal& o) const {
        if (deciCount != o.deciCount)
            throw std::runtime_error("Decimal mismatch in UDecimal");

        UDecimal d(0.0f, deciCount);
        d.val = val + o.val;
        d.Normalize();
        return d;
    }

    UDecimal operator-(const UDecimal& o) const {
        if (deciCount != o.deciCount)
            throw std::runtime_error("Decimal mismatch in UDecimal");

        UDecimal d(0.0f, deciCount);
        d.val = (val > o.val) ? val - o.val : 0;
        d.Normalize();
        return d;
    }

    UDecimal operator*(const UDecimal& o) const {
        if (deciCount != o.deciCount)
            throw std::runtime_error("Decimal mismatch in UDecimal");

        uint32_t res = static_cast<uint32_t>(val) * o.val;

        UDecimal d(0.0f, deciCount);
        d.val = static_cast<uint32_t>(res / FromDecimals(deciCount));
        d.Normalize();
        return d;
    }

    UDecimal operator/(const UDecimal& o) const {
        if (deciCount != o.deciCount)
            throw std::runtime_error("Decimal mismatch in UDecimal");
        if (o.val == 0)
            throw std::runtime_error("Division by zero");

        uint32_t scaled = static_cast<uint32_t>(val) * FromDecimals(deciCount);

        UDecimal d(0.0f, deciCount);
        d.val = static_cast<uint32_t>(scaled / o.val);
        d.Normalize();
        return d;
    }

    UDecimal operator^(const UDecimal& o) const {
        if (deciCount != o.deciCount)
            throw std::runtime_error("Decimal mismatch in UDecimal");

        double base = static_cast<float>(val) / FromDecimals(deciCount);
        double exponent = static_cast<float>(o.val) / FromDecimals(deciCount);
        double result = pow(base, exponent);

        UDecimal d(static_cast<float>(result), deciCount);
        d.Normalize();
        return d;
    }
};

struct Decimal {
private:
    void Normalize() {
        int32_t scale = FromDecimals(deciCount);
        val = (val > scale * 1000000) ? val % (scale * 1000000) : val;
    }

public:
    int32_t val;
    uint8_t deciCount;

    static constexpr uint16_t FromDecimals(uint8_t decimals) {
        switch (decimals) {
        case 1: return 10;
        case 2: return 100;
        case 3: return 1000;
        case 4: return 10000;
        default: return 1000;
        }
    }

    Decimal(float value, uint8_t decimals = 2)
        : val(static_cast<int32_t>(std::round(value* FromDecimals(decimals)))),
        deciCount(decimals) {}

    Decimal(int32_t rawVal, uint8_t decimals)
        : val(rawVal), deciCount(decimals) {
        Normalize();
    }

    float ToFloat() const {
        return static_cast<float>(val) / FromDecimals(deciCount);
    }

    std::string ToString() const {
        int16_t sc = FromDecimals(deciCount);
        int32_t intP = val / sc;
        int32_t fracP = std::abs(val % sc);

        std::ostringstream oss;
        if (val < 0 && intP == 0) oss << "-";
        oss << intP << "." << std::setfill('0') << std::setw(deciCount) << fracP;
        return oss.str();
    }

    Decimal operator+(const Decimal& o) const {
        if (deciCount != o.deciCount) throw std::runtime_error("Decimal mismatch");
        Decimal d(static_cast<float>(val + o.val) / FromDecimals(deciCount), deciCount);
        d.Normalize();
        return d;
    }

    Decimal operator-(const Decimal& o) const {
        if (deciCount != o.deciCount) throw std::runtime_error("Decimal mismatch");
        Decimal d(static_cast<float>(val - o.val) / FromDecimals(deciCount), deciCount);
        d.Normalize();
        return d;
    }

    Decimal operator*(const Decimal& o) const {
        if (deciCount != o.deciCount) throw std::runtime_error("Decimal mismatch");

        int32_t prod = static_cast<int32_t>(val) * o.val;
        int32_t scVal = static_cast<int32_t>(prod / FromDecimals(deciCount));

        Decimal d(static_cast<float>(scVal) / FromDecimals(deciCount), deciCount);
        d.Normalize();
        return d;
    }

    Decimal operator/(const Decimal& o) const {
        if (deciCount != o.deciCount) throw std::runtime_error("Decimal mismatch");
        if (o.val == 0) throw std::runtime_error("Division by zero");

        int32_t sc = static_cast<int32_t>(val) * FromDecimals(deciCount);
        int32_t res = static_cast<int32_t>(sc / o.val);

        Decimal d(static_cast<float>(res) / FromDecimals(deciCount), deciCount);
        d.Normalize();
        return d;
    }

    Decimal operator^(const Decimal& o) const {
        if (deciCount != o.deciCount) throw std::runtime_error("Decimal mismatch");

        float a = ToFloat();
        float b = o.ToFloat();
        float res = std::pow(a, b);

        return Decimal(res, deciCount);
    }

};