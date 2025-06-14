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
#include <stdexcept>

template<typename T>
class List {
private:
    std::vector<T> data;

public:
    void Add(const T& item) {
        if (data.size() >= static_cast<std::size_t>(INT16_MAX))
            throw std::length_error("List capacity exceeded int16_t max.");
        data.push_back(item);
    }

    void AddRange(const std::vector<T>& items) {
        if (data.size() + items.size() > static_cast<std::size_t>(INT16_MAX))
            throw std::length_error("AddRange would exceed int16_t max capacity.");
        data.insert(data.end(), items.begin(), items.end());
    }

    bool Remove(const T& item) {
        auto it = std::find(data.begin(), data.end(), item);
        if (it != data.end()) {
            data.erase(it);
            return true;
        }
        return false;
    }

    void RemoveAt(int16_t index) {
        if (index < 0 || static_cast<std::size_t>(index) >= data.size())
            throw std::out_of_range("Index out of range");
        data.erase(data.begin() + index);
    }

    void Clear() {
        data.clear();
    }

    bool Contains(const T& item) const {
        return std::find(data.begin(), data.end(), item) != data.end();
    }

    int16_t IndexOf(const T& item) const {
        auto it = std::find(data.begin(), data.end(), item);
        if (it != data.end()) {
            return static_cast<int16_t>(std::distance(data.begin(), it));
        }
        return static_cast<int16_t>(-1);
    }

    int16_t Count() const {
        return static_cast<int16_t>(data.size());
    }

    T& operator[](int16_t index) {
        if (index < 0 || static_cast<std::size_t>(index) >= data.size())
            throw std::out_of_range("Index out of range");
        return data[index];
    }

    const T& operator[](int16_t index) const {
        if (index < 0 || static_cast<std::size_t>(index) >= data.size())
            throw std::out_of_range("Index out of range");
        return data[index];
    }

    T& At(int16_t index) {
        if (index < 0 || static_cast<std::size_t>(index) >= data.size())
            throw std::out_of_range("Index out of range");
        return data[index];
    }

    const T& At(int16_t index) const {
        if (index < 0 || static_cast<std::size_t>(index) >= data.size())
            throw std::out_of_range("Index out of range");
        return data[index];
    }

    auto begin() { return data.begin(); }
    auto end() { return data.end(); }
    auto begin() const { return data.begin(); }
    auto end()   const { return data.end(); }
};