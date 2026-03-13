#ifndef ADAPTIVE_SEQUENCE_H
#define ADAPTIVE_SEQUENCE_H

#include "sequence.h"
#include "ArraySequence.h"
#include "ListSequence.h"

template <class T>
class AdaptiveSequence : public Sequence<T> {
private:
    mutable int index_ops_;
    mutable int insert_ops_;
    mutable bool is_array_;
    mutable Sequence<T>* inner_;


    static const int THRESHOLD = 10;

    void maybe_switch() {
        if (is_array_ && insert_ops_ > index_ops_ + THRESHOLD) {
            Sequence<T>* new_inner = new MutableListSequence<T>();
            for (int i = 0; i < inner_->GetLength(); i++)
                new_inner->Append(inner_->Get(i));
            delete inner_;
            inner_ = new_inner;
            is_array_ = false;
            index_ops_ = insert_ops_ = 0;

        }
        else if (!is_array_ && index_ops_ > insert_ops_ + THRESHOLD) {
            Sequence<T>* new_inner = new MutableArraySequence<T>();
            for (int i = 0; i < inner_->GetLength(); i++)
                new_inner->Append(inner_->Get(i));
            delete inner_;
            inner_ = new_inner;
            is_array_ = true;
            index_ops_ = insert_ops_ = 0;
        }
    }

public:
    AdaptiveSequence()
        : inner_(new MutableArraySequence<T>()),
        index_ops_(0), insert_ops_(0), is_array_(true) {
    }

    ~AdaptiveSequence() {
        delete inner_;
    }

    T GetFirst() const override {
        return inner_->GetFirst();
    }

    T GetLast() const override {
        return inner_->GetLast();
    }

    T Get(int index) const override {
        index_ops_++;
        const_cast<AdaptiveSequence*>(this)->maybe_switch();
        return inner_->Get(index);
    }

    int GetLength() const override {
        return inner_->GetLength();
    }

    Sequence<T>* GetSubsequence(int start, int end) const override {
        return inner_->GetSubsequence(start, end);
    }

    Sequence<T>* Append(const T& item) override {
        inner_->Append(item);
        return this;
    }

    Sequence<T>* Prepend(const T& item) override {
        insert_ops_++;
        inner_->Prepend(item);
        maybe_switch();
        return this;
    }

    Sequence<T>* InsertAt(const T& item, int index) override {
        insert_ops_++;
        inner_->InsertAt(item, index);
        maybe_switch();
        return this;
    }

    // ← исправлено: const Sequence<T>& вместо Sequence<T>*
    Sequence<T>* Concat(const Sequence<T>& other) const override {
        return inner_->Concat(other);
    }

    // ← добавлено: operator[]
    T operator[](int index) const override {
        return inner_->Get(index);
    }

    // ← добавлено: operator+
    Sequence<T>* operator+(const Sequence<T>& other) const override {
        return inner_->Concat(other);
    }

    IEnumerator<T>* get_enumerator() const override {
        return inner_->get_enumerator();
    }

    bool IsArray() const { return is_array_; }
};

#endif
