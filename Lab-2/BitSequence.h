#pragma once

class BitSequence {
public:
    BitSequence();
    BitSequence(int size);
    BitSequence(const char* rawData, int size);
    BitSequence(const BitSequence& other);
    BitSequence& operator=(const BitSequence& other);
    ~BitSequence();

    int GetLength() const;

    int  Get(int index) const;        // возвращает 0 или 1
    void Set(int index, int bit);     // устанавливает 0 или 1
    void Flip(int index);             // инвертирует бит

    BitSequence AND(const BitSequence& other) const;
    BitSequence OR(const BitSequence& other)  const;
    BitSequence XOR(const BitSequence& other) const;
    BitSequence NOT() const;

    void Print() const;

    int  operator[](int index) const;
    bool operator==(const BitSequence& other) const;
    BitSequence operator&(const BitSequence& other) const;
    BitSequence operator|(const BitSequence& other) const;
    BitSequence operator^(const BitSequence& other) const;
    BitSequence operator~() const;

private:
    char* data_;
    int length_;    // количество бит
    int byteCount_; // количество байт = (length + 7) / 8

    void CheckIndex(int index) const;
};

#include "BitSequence_impl.h"
