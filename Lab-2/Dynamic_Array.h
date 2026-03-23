#pragma once

template <class T>
class DynamicArray {
public:
    DynamicArray();
    DynamicArray(int size);
    DynamicArray(const T* items, int count);
    DynamicArray(const DynamicArray<T>& other);

    DynamicArray<T>& operator=(const DynamicArray<T>& other);

    ~DynamicArray();

    const T& Get(int index) const;
    int GetSize() const;
    void Set(int index, const T& value);
    void Resize(int newSize);

    T& operator[](int index);
    const T& operator[](int index) const;

private:
    T* data_;
    int size_;

    void CheckIndex(int index) const;
};

#include "Dynamic_Array.tpp"
