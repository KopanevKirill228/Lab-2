#pragma once

#include "AdaptiveSequence.h"

template <class T>
void AdaptiveSequence<T>::maybe_switch() {
    if (is_array_ && insert_ops_ > index_ops_ + THRESHOLD) {
        Sequence<T>* new_inner = new MutableListSequence<T>();
        try {
            for (int i = 0; i < inner_->GetLength(); i++)
                new_inner->Append(inner_->Get(i));
        }
        catch (...) {
            delete new_inner;  // чистим если что-то пошло не так
            throw;             // пробрасываем исключение дальше
        }
        delete inner_;
        inner_ = new_inner;
        is_array_ = false;
        index_ops_ = insert_ops_ = 0;
    }
    else if (!is_array_ && index_ops_ > insert_ops_ + THRESHOLD) {
        Sequence<T>* new_inner = new MutableArraySequence<T>();
        try {
            for (int i = 0; i < inner_->GetLength(); i++)
                new_inner->Append(inner_->Get(i));
        }
        catch (...) {
            delete new_inner;
            throw;
        }
        delete inner_;
        inner_ = new_inner;
        is_array_ = true;
        index_ops_ = insert_ops_ = 0;
    }
}


template <class T>
AdaptiveSequence<T>::AdaptiveSequence()
    : inner_(new MutableArraySequence<T>()),
    index_ops_(0), insert_ops_(0), is_array_(true) {
}

template <class T>
AdaptiveSequence<T>::~AdaptiveSequence() {
    delete inner_;
}

template <class T>
const T& AdaptiveSequence<T>::GetFirst() const {
    return inner_->GetFirst();
}

template <class T>
const T& AdaptiveSequence<T>::GetLast() const {
    return inner_->GetLast();
}

template <class T>
const T& AdaptiveSequence<T>::Get(int index) const {
    index_ops_++;
    const_cast<AdaptiveSequence*>(this)->maybe_switch();
    return inner_->Get(index);
}

template <class T>
int AdaptiveSequence<T>::GetLength() const {
    return inner_->GetLength();
}

template <class T>
Sequence<T>* AdaptiveSequence<T>::GetSubsequence(int start, int end) const {
    return inner_->GetSubsequence(start, end);
}

template <class T>
Sequence<T>* AdaptiveSequence<T>::Append(const T& item) {
    inner_->Append(item);
    return this;
}

template <class T>
Sequence<T>* AdaptiveSequence<T>::Prepend(const T& item) {
    insert_ops_++;
    inner_->Prepend(item);
    maybe_switch();
    return this;
}

template <class T>
Sequence<T>* AdaptiveSequence<T>::InsertAt(const T& item, int index) {
    insert_ops_++;
    inner_->InsertAt(item, index);
    maybe_switch();
    return this;
}

template <class T>
Sequence<T>* AdaptiveSequence<T>::Concat(const Sequence<T>& other) const {
    return inner_->Concat(other);
}

template <class T>
T AdaptiveSequence<T>::operator[](int index) const {
    return inner_->Get(index);
}

template <class T>
Sequence<T>* AdaptiveSequence<T>::operator+(const Sequence<T>& other) const {
    return inner_->Concat(other);
}

template <class T>
IEnumerator<T>* AdaptiveSequence<T>::get_enumerator() const {
    return inner_->get_enumerator();
}

template <class T>
bool AdaptiveSequence<T>::IsArray() const {
    return is_array_;
}
