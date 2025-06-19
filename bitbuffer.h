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

#include "list.h"
#include "decimal.h"
#include "file.h"
#include "str.h"

class BitBuffer {
private:
    list<uint8_t> bytes;
    uint16_t bitPos = 0;
    uint16_t readBitPos = 0;

    static constexpr char CHARSET_4[] = "0123456789ABCDEF";
    static constexpr char CHARSET_5[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ .!?";
    static constexpr char CHARSET_6[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 .!?";

    const char* GetCharset(uint8_t bits, uint8_t& outSize) const {
        switch (bits) {
        case 4: outSize = sizeof(CHARSET_4) - 1; return CHARSET_4;
        case 5: outSize = sizeof(CHARSET_5) - 1; return CHARSET_5;
        case 6: outSize = sizeof(CHARSET_6) - 1; return CHARSET_6;
        case 7: outSize = 128; return nullptr;
        default: throw std::invalid_argument("Unsupported bit width for character encoding");
        }
    }

    const int GetCharIndex(char c, const char* charset, uint16_t size) {
        for (uint16_t i = 0; i < size; ++i)
            if (charset[i] == c) return static_cast<int>(i);
        return -1;
    }

public:
    BitBuffer(const list<uint8_t>& bytes) : bytes(bytes) {}

    BitBuffer(const str& path) {
        ReadFromFile(path);
    }
    BitBuffer() {
        bytes.Add(0);
    }

    void Write(const bool& value) {
        if (bitPos == 8) {
            bytes.Add(0);
            bitPos = 0;
        }

        if (value)
            bytes[bytes.count - 1] |= (1 << bitPos);

        bitPos++;
    }

    void WriteUInt(const uint32_t& num, const uint8_t& bitCount) {
        if (bitCount > 32)
            throw std::invalid_argument("bitCount cannot go over 32");

        for (uint8_t i = 0; i < bitCount; ++i) {
            bool bit = (num >> i) & 1;
            Write(bit);
        }
    }

    void WriteUDecimal(const UDecimal& dec, const uint8_t& bitCount) {
        if (bitCount > 32)
            throw std::invalid_argument("bitCount cannot go over 32");

        WriteUInt(dec.deciCount, 2);
        WriteUInt(dec.val, bitCount);
    }

    void WriteDecimal(const Decimal& dec, const uint8_t& bitCount) {
        if (bitCount > 32)
            throw std::invalid_argument("bitCount cannot go over 32");

        WriteUInt(dec.deciCount, 2);
        WriteInt(dec.val, bitCount);
    }

    void WriteInt(const int32_t& number, const uint8_t& bitCount) {
        if (bitCount > 32)
            throw std::invalid_argument("bitCount cannot go over 32");

        WriteUInt(number, bitCount);
    }



    void WriteChar(const char& c, const uint8_t& bits) {
        if (bits == 7) {
            if ((unsigned char)c > 127) throw std::invalid_argument("Char is over 7 bits");
            WriteUInt(static_cast<const uint8_t>(c), 7);
            return;
        }

        uint8_t tableSize;
        const char* charset = GetCharset(bits, tableSize);
        int index = GetCharIndex(c, charset, tableSize);
        if (index == -1) throw std::invalid_argument("Char isn't in charset, use more bits");

        WriteUInt(index, bits);
    }

    // bpc = amount of bits used for char (Bits Per Char)
    // bfcl = the amount of bits used for defying the amount of chars within the string (Bits For Char Length)
    void WriteString(const str& value, const uint8_t& bpc, const uint8_t& bfcl = 12) {
        WriteUInt(value.length(), bfcl);
        for (char c : value)
            WriteChar(c, bpc);
    }







    const bool Read() {
        int byteIndex = readBitPos / 8;
        int bitIndex = readBitPos % 8;

        if (byteIndex >= (uint8_t)bytes.count)
            throw std::out_of_range("Cannot read past end of buffer, align your write bitCounts with your read bitCounts");

        const uint8_t byte = bytes[byteIndex];
        bool bit = (byte & (1 << bitIndex)) != 0;
        readBitPos++;
        return bit;
    }

    const uint32_t ReadUInt(const uint8_t& bitCount) {
        if (bitCount > 32)
            throw std::invalid_argument("bitCount cannot go over 32");

        uint32_t number = 0;
        for (uint8_t i = 0; i < bitCount; ++i)
            if (Read())
                number |= (uint32_t(1) << i);

        return number;
    }

    const UDecimal ReadUDecimal(const uint8_t& bitCount) {
        if (bitCount > 32)
            throw std::invalid_argument("bitCount cannot go over 32");

        uint8_t decimals = ReadUInt(2);
        uint32_t num = ReadUInt(bitCount);
        return UDecimal(num, decimals);
    }

    const Decimal ReadDecimal(const uint8_t& bitCount) {
        if (bitCount > 32)
            throw std::invalid_argument("bitCount cannot go over 32");

        uint8_t decimals = ReadUInt(2);
        int32_t num = ReadInt(bitCount);
        return Decimal(num, decimals);
    }

    const int32_t ReadInt(const uint8_t& bitCount) {
        uint32_t raw = ReadUInt(bitCount);

        if (bitCount < 32 && (raw & (1U << (bitCount - 1)))) {
            raw |= ~((1U << bitCount) - 1);
        }

        return raw;
    }


    const char ReadChar(const uint8_t& bits) {
        if (bits == 7)
            return static_cast<char>(ReadUInt(7));

        uint8_t tableSize;
        const char* charset = GetCharset(bits, tableSize);
        const uint32_t index = ReadUInt(bits);
        if (index >= tableSize) throw std::out_of_range("Char isn't in range");

        return charset[index];
    }

    // bpc = amount of bits used for char (Bits Per Char)
    // bfcl = the amount of bits used for defying the amount of chars within the string (Bits For Char Length)
    str ReadString(const uint8_t& bpc, const uint8_t bfcl = 12) {
        str ret;
        uint16_t l = ReadUInt(bfcl);
        for (uint16_t i = 0; i < l; ++i)
            ret += ReadChar(bpc);
        return ret;
    }



    const list<uint8_t>& GetBuffer() const {
        return bytes;
    }

    void Reset() {
        bitPos = 0;
        readBitPos = 0;
    }




    bool WriteToFile(const str& path) {
        bool b = WriteFileBytes(path, GetBuffer());
        return b;
    }

    void ReadFromFile(const str path) {
        bytes = ReadFileBytes(path);
    }
};