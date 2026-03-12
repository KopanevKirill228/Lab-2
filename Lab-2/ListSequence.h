#pragma once

#include "Sequence.h"
#include "Linked_List.h"

template <class T>
class ListSequence : public Sequence<T> {
public:
    ListSequence();
    ListSequence(const T* items, int count);
    ListSequence(const LinkedList<T>& list);
    ListSequence(const ListSequence<T>& other);
    virtual ~ListSequence() = default;

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
    virtual ListSequence<T>* GetInstance() = 0;
    virtual ListSequence<T>* Clone() const = 0;

    void AppendInternal(const T& item);
    void PrependInternal(const T& item);
    void InsertAtInternal(const T& item, int index);

private:
    LinkedList<T> items_;
};


template <class T>
class MutableListSequence : public ListSequence<T> {
public:
    MutableListSequence();
    MutableListSequence(const T* items, int count);
    MutableListSequence(const LinkedList<T>& list);
    MutableListSequence(const MutableListSequence<T>& other);

protected:
    ListSequence<T>* GetInstance() override;
    ListSequence<T>* Clone() const override;
};

template <class T>
class ImmutableListSequence : public ListSequence<T> {
public:
    ImmutableListSequence();
    ImmutableListSequence(const T* items, int count);
    ImmutableListSequence(const LinkedList<T>& list);
    ImmutableListSequence(const ImmutableListSequence<T>& other);

protected:
    ListSequence<T>* GetInstance() override;
    ListSequence<T>* Clone() const override;
};


#include "ListSequence_impl.h"