#pragma once

#include <stdexcept>
#include "Sequence.h"
#include "Linked_List.h"
#include "ienumerator.h"

template <class T>
class ListSequence : public Sequence<T> {
public:
    ListSequence();
    ListSequence(const T* items, int count);
    ListSequence(const LinkedList<T>& list);
    ListSequence(const ListSequence<T>& other);
    virtual ~ListSequence() = default;

    const T& GetFirst() const override;
    const T& GetLast() const override;
    const T& Get(int index) const override;
    int GetLength() const override;

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override;
    Sequence<T>* Append(const T& item) override;
    Sequence<T>* Prepend(const T& item) override;
    Sequence<T>* InsertAt(const T& item, int index) override;
    Sequence<T>* Concat(const Sequence<T>& other) const override;

    T operator[](int index) const override;
    Sequence<T>* operator+(const Sequence<T>& other) const override;


    class Enumerator : public IEnumerator<T> {
    private:
        const ListSequence<T>* seq_;
        int index_;
    public:
        Enumerator(const ListSequence<T>* seq) : seq_(seq), index_(-1) {}

        bool move_next() override {
            index_++;
            return index_ < seq_->GetLength();
        }

        const T& get_current() const override {
            if (index_ < 0 || index_ >= seq_->GetLength()) {
                throw std::out_of_range("Enumerator is out of range");
            }
            return seq_->Get(index_);
        }

        void reset() override { index_ = -1; }
    };

    IEnumerator<T>* get_enumerator() const override {
        return new Enumerator(this);
    }


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

    // вложенный Builder
    class Builder {
    private:
        MutableListSequence<T>* seq_;
    public:
        Builder() : seq_(new MutableListSequence<T>()) {}

        ~Builder() { delete seq_; }

        Builder& Append(const T& item);
        Builder& AppendAll(const T* items, int count);
        Builder& AppendSequence(const Sequence<T>& other);
        MutableListSequence<T>* Build();
    };

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


#include "ListSequence.tpp"
