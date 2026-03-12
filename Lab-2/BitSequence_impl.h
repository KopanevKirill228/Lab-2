#include "BitSequence.h"
#include <stdexcept>
#include <iostream>

BitSequence::BitSequence()
    : data_(nullptr), length_(0), byteCount_(0) {
}

BitSequence::BitSequence(int size) {
    if (size < 0)
        throw std::invalid_argument("Size cannot be negative");
    length_ = size;
    byteCount_ = (size + 7) / 8;  // округление вверх
    data_ = new char[byteCount_];
    for (int i = 0; i < byteCount_; ++i)
        data_[i] = 0;
}

BitSequence::BitSequence(const char* rawData, int size) {
    length_ = size;
    byteCount_ = (size + 7) / 8;
    data_ = new char[byteCount_];
    for (int i = 0; i < byteCount_; ++i)
        data_[i] = rawData[i];
}

BitSequence::BitSequence(const BitSequence& other) {
    length_ = other.length_;
    byteCount_ = other.byteCount_;
    data_ = new char[byteCount_];
    for (int i = 0; i < byteCount_; ++i)
        data_[i] = other.data_[i];
}

BitSequence& BitSequence::operator=(const BitSequence& other) {
    if (this != &other) {
        delete[] data_;
        length_ = other.length_;
        byteCount_ = other.byteCount_;
        data_ = new char[byteCount_];
        for (int i = 0; i < byteCount_; ++i)
            data_[i] = other.data_[i];
    }
    return *this;
}

BitSequence::~BitSequence() {
    delete[] data_;
}

void BitSequence::CheckIndex(int index) const {
    if (index < 0 || index >= length_)
        throw std::out_of_range("Index is out of range");
}

int BitSequence::GetLength() const {
    return length_;
}

int BitSequence::Get(int index) const {
    CheckIndex(index);
    int byte = index / 8;
    int pos = index % 8;
    return (data_[byte] >> pos) & 1;
}

void BitSequence::Set(int index, int bit) {
    CheckIndex(index);
    if (bit != 0 && bit != 1)
        throw std::invalid_argument("Bit must be 0 or 1");
    int byte = index / 8;
    int pos = index % 8;
    if (bit == 1)
        data_[byte] |= (1 << pos);  // установить в 1
    else
        data_[byte] &= ~(1 << pos);  // установить в 0
}

void BitSequence::Flip(int index) {
    CheckIndex(index);
    int byte = index / 8;
    int pos = index % 8;
    data_[byte] ^= (1 << pos);  // XOR с 1 инвертирует бит
}

BitSequence BitSequence::AND(const BitSequence& other) const {
    if (length_ != other.length_)
        throw std::invalid_argument("Sequences must have equal length");
    BitSequence result(length_);
    for (int i = 0; i < byteCount_; ++i)
        result.data_[i] = data_[i] & other.data_[i];
    return result;
}

BitSequence BitSequence::OR(const BitSequence& other) const {
    if (length_ != other.length_)
        throw std::invalid_argument("Sequences must have equal length");
    BitSequence result(length_);
    for (int i = 0; i < byteCount_; ++i)
        result.data_[i] = data_[i] | other.data_[i];
    return result;
}

BitSequence BitSequence::XOR(const BitSequence& other) const {
    if (length_ != other.length_)
        throw std::invalid_argument("Sequences must have equal length");
    BitSequence result(length_);
    for (int i = 0; i < byteCount_; ++i)
        result.data_[i] = data_[i] ^ other.data_[i];
    return result;
}

BitSequence BitSequence::NOT() const {
    BitSequence result(length_);
    for (int i = 0; i < byteCount_; ++i)
        result.data_[i] = ~data_[i];
    // обнуляем лишние биты в последнем байте
    int extraBits = byteCount_ * 8 - length_;
    if (extraBits > 0)
        result.data_[byteCount_ - 1] &= (1 << (8 - extraBits)) - 1;
    return result;
}

void BitSequence::Print() const {
    std::cout << "[";
    for (int i = 0; i < length_; ++i) {
        std::cout << Get(i);
        if (i < length_ - 1)
            std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}

int BitSequence::operator[](int index) const {
    return Get(index);
}

bool BitSequence::operator==(const BitSequence& other) const {
    if (length_ != other.length_)
        return false;
    for (int i = 0; i < byteCount_; ++i)
        if (data_[i] != other.data_[i])
            return false;
    return true;
}

BitSequence BitSequence::operator&(const BitSequence& other) const {
    return AND(other);
}

BitSequence BitSequence::operator|(const BitSequence& other) const {
    return OR(other);
}

BitSequence BitSequence::operator^(const BitSequence& other) const {
    return XOR(other);
}

BitSequence BitSequence::operator~() const {
    return NOT();
}
