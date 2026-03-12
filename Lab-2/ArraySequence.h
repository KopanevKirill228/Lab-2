#pragma once

#include "Sequence.h"
#include "Dynamic_Array.h"

template <class T>
class ArraySequence : public Sequence<T> {
public:
    ArraySequence();
    ArraySequence(const T* items, int count);
    ArraySequence(const ArraySequence<T>& other);
    virtual ~ArraySequence() = default;

    T GetFirst() const override;
    T GetLast() const override;
    T Get(int index) const override;
    int GetLength() const override;

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override;
    Sequence<T>* Append(const T& item) override;
    Sequence<T>* Prepend(const T& item) override;
    Sequence<T>* InsertAt(const T& item, int index) override;
    Sequence<T>* Concat(const Sequence<T>& other) const override;

    T operator[](int index) const override;
    Sequence<T>* operator+(const Sequence<T>& other) const override;

protected:
    virtual ArraySequence<T>* GetInstance() = 0;
    virtual ArraySequence<T>* Clone() const = 0;

    void AppendInternal(const T& item);
    void PrependInternal(const T& item);
    void InsertAtInternal(const T& item, int index);

private:
    DynamicArray<T> items_;
};


template <class T>
class MutableArraySequence : public ArraySequence<T> {
public:
    MutableArraySequence();
    MutableArraySequence(const T* items, int count);
    MutableArraySequence(const MutableArraySequence<T>& other);

protected:
    ArraySequence<T>* GetInstance() override;
    ArraySequence<T>* Clone() const override;
};


template <class T>
class ImmutableArraySequence : public ArraySequence<T> {
public:
    ImmutableArraySequence();
    ImmutableArraySequence(const T* items, int count);
    ImmutableArraySequence(const ImmutableArraySequence<T>& other);

protected:
    ArraySequence<T>* GetInstance() override;
    ArraySequence<T>* Clone() const override;
};

#include "ArraySequence_impl.h"
