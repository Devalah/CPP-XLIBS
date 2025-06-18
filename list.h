#pragma once

#include <stdexcept>
#include <cstdint>
#include <algorithm>

template<typename T>
class list {
private:
    T* data = nullptr;
    int16_t capacity = 0;

    void Resize(int16_t newCapacity) {
        if (newCapacity <= capacity)
            return;

        T* newData = new T[newCapacity];
        for (int16_t i = 0; i < count; ++i)
            newData[i] = std::move(data[i]);

        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    int16_t count = 0;

    list() {
        Resize(4);
    }

    ~list() {
        delete[] data;
    }

    list(const list& other) {
        Resize(other.capacity);
        count = other.count;
        for (int16_t i = 0; i < count; ++i)
            data[i] = other.data[i];
    }

    list(std::initializer_list<T> items) {
        Resize(static_cast<int16_t>(items.size()));
        for (const T& item : items)
            Add(item);
    }

    list& operator=(const list& other) {
        if (this != &other) {
            delete[] data;
            data = nullptr;
            capacity = count = 0;
            Resize(other.capacity);
            count = other.count;
            for (int16_t i = 0; i < count; ++i)
                data[i] = other.data[i];
        }
        return *this;
    }

    list(list&& other) noexcept {
        data = other.data;
        count = other.count;
        capacity = other.capacity;
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }

    list& operator=(list&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            count = other.count;
            capacity = other.capacity;
            other.data = nullptr;
            other.count = 0;
            other.capacity = 0;
        }
        return *this;
    }

    void Add(const T& item) {
        if (count >= INT16_MAX)
            throw std::length_error("List capacity exceeded int16_t max.");
        if (count >= capacity)
            Resize(capacity * 2);
        data[count++] = item;
    }

    void AddRange(const T* items, int16_t itemCount) {
        if (count + itemCount > INT16_MAX)
            throw std::length_error("AddRange would exceed int16_t max capacity.");
        if (count + itemCount > capacity)
            Resize(std::max<int16_t>(capacity * 2, count + itemCount));
        for (int16_t i = 0; i < itemCount; ++i)
            data[count++] = items[i];
    }

    bool Remove(const T& item) {
        for (int16_t i = 0; i < count; ++i) {
            if (data[i] == item) {
                for (int16_t j = i; j < count - 1; ++j)
                    data[j] = std::move(data[j + 1]);
                --count;
                return true;
            }
        }
        return false;
    }

    void RemoveAt(int16_t index) {
        if (index < 0 || index >= count)
            throw std::out_of_range("Index out of range");
        for (int16_t i = index; i < count - 1; ++i)
            data[i] = std::move(data[i + 1]);
        --count;
    }

    void Clear() {
        count = 0;
    }

    bool Contains(const T& item) const {
        for (int16_t i = 0; i < count; ++i)
            if (data[i] == item)
                return true;
        return false;
    }

    int16_t IndexOf(const T& item) const {
        for (int16_t i = 0; i < count; ++i)
            if (data[i] == item)
                return i;
        return -1;
    }

    T& operator[](int16_t index) {
        if (index < 0 || index >= count)
            throw std::out_of_range("Index out of range");
        return data[index];
    }

    const T& operator[](int16_t index) const {
        if (index < 0 || index >= count)
            throw std::out_of_range("Index out of range");
        return data[index];
    }

    const T& At(int16_t index) {
        return (*this)[index];
    }

    const T& At(int16_t index) const {
        return (*this)[index];
    }

    T* begin() { return data; }
    T* end() { return data + count; }
    const T* begin() const { return data; }
    const T* end() const { return data + count; }
};
