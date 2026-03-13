#include "ArraySequence.h"
#include <stdexcept>


template <class T>
ArraySequence<T>::ArraySequence()
    : items_(0) {
}

template <class T>
ArraySequence<T>::ArraySequence(const T* items, int count)
    : items_(items, count) {
}

template <class T>
ArraySequence<T>::ArraySequence(const ArraySequence<T>& other)
    : items_(other.items_) {
}

template <class T>
T ArraySequence<T>::GetFirst() const {
    if (items_.GetSize() == 0)
        throw std::out_of_range("Sequence is empty");
    return items_.Get(0);
}

template <class T>
T ArraySequence<T>::GetLast() const {
    if (items_.GetSize() == 0)
        throw std::out_of_range("Sequence is empty");
    return items_.Get(items_.GetSize() - 1);
}

template <class T>
T ArraySequence<T>::Get(int index) const {
    return items_.Get(index);
}

template <class T>
int ArraySequence<T>::GetLength() const {
    return items_.GetSize();
}

template <class T>
void ArraySequence<T>::AppendInternal(const T& item) {
    int size = items_.GetSize();
    items_.Resize(size + 1);
    items_.Set(size, item);
}

template <class T>
void ArraySequence<T>::PrependInternal(const T& item) {
    int size = items_.GetSize();
    items_.Resize(size + 1);
    for (int i = size; i > 0; --i)
        items_.Set(i, items_.Get(i - 1));
    items_.Set(0, item);
}

template <class T>
void ArraySequence<T>::InsertAtInternal(const T& item, int index) {
    if (index < 0 || index > items_.GetSize())
        throw std::out_of_range("Index is out of range");
    int size = items_.GetSize();
    items_.Resize(size + 1);
    for (int i = size; i > index; --i)
        items_.Set(i, items_.Get(i - 1));
    items_.Set(index, item);
}

template <class T>
Sequence<T>* ArraySequence<T>::Append(const T& item) {
    ArraySequence<T>* instance = GetInstance();
    instance->AppendInternal(item);
    return instance;
}

template <class T>
Sequence<T>* ArraySequence<T>::Prepend(const T& item) {
    ArraySequence<T>* instance = GetInstance();
    instance->PrependInternal(item);
    return instance;
}

template <class T>
Sequence<T>* ArraySequence<T>::InsertAt(const T& item, int index) {
    ArraySequence<T>* instance = GetInstance();
    instance->InsertAtInternal(item, index);
    return instance;
}

template <class T>
Sequence<T>* ArraySequence<T>::GetSubsequence(int startIndex, int endIndex) const {
    if (startIndex < 0 || endIndex >= items_.GetSize() || startIndex > endIndex)
        throw std::out_of_range("Invalid range");
    MutableArraySequence<T>* result = new MutableArraySequence<T>();
    for (int i = startIndex; i <= endIndex; ++i)
        result->AppendInternal(items_.Get(i));
    return result;
}

template <class T>
Sequence<T>* ArraySequence<T>::Concat(const Sequence<T>& other) const {
    MutableArraySequence<T>* result = new MutableArraySequence<T>();
    for (int i = 0; i < this->GetLength(); ++i)
        result->AppendInternal(this->Get(i));
    for (int i = 0; i < other.GetLength(); ++i)
        result->AppendInternal(other.Get(i));
    return result;
}

template <class T>
T ArraySequence<T>::operator[](int index) const {
    return items_.Get(index);
}

template <class T>
Sequence<T>* ArraySequence<T>::operator+(const Sequence<T>& other) const {
    return Concat(other);
}

// MutableArraySequence

template <class T>
MutableArraySequence<T>::MutableArraySequence()
    : ArraySequence<T>() {
}

template <class T>
MutableArraySequence<T>::MutableArraySequence(const T* items, int count)
    : ArraySequence<T>(items, count) {
}

template <class T>
MutableArraySequence<T>::MutableArraySequence(const MutableArraySequence<T>& other)
    : ArraySequence<T>(other) {
}

template <class T>
ArraySequence<T>* MutableArraySequence<T>::GetInstance() {
    return this;
}

template <class T>
ArraySequence<T>* MutableArraySequence<T>::Clone() const {
    return new MutableArraySequence<T>(*this);
}

// ImmutableArraySequence

template <class T>
ImmutableArraySequence<T>::ImmutableArraySequence()
    : ArraySequence<T>() {
}

template <class T>
ImmutableArraySequence<T>::ImmutableArraySequence(const T* items, int count)
    : ArraySequence<T>(items, count) {
}

template <class T>
ImmutableArraySequence<T>::ImmutableArraySequence(const ImmutableArraySequence<T>& other)
    : ArraySequence<T>(other) {
}

template <class T>
ArraySequence<T>* ImmutableArraySequence<T>::GetInstance() {
    return Clone();
}

template <class T>
ArraySequence<T>* ImmutableArraySequence<T>::Clone() const {
    return new ImmutableArraySequence<T>(*this);
}

// Builder
template <class T>
typename MutableArraySequence<T>::Builder&
MutableArraySequence<T>::Builder::Append(const T& item) {
    seq_->Append(item);
    return *this;
}

template <class T>
typename MutableArraySequence<T>::Builder&
MutableArraySequence<T>::Builder::AppendAll(const T* items, int count) {
    for (int i = 0; i < count; i++)
        seq_->Append(items[i]);
    return *this;
}

template <class T>
typename MutableArraySequence<T>::Builder&
MutableArraySequence<T>::Builder::AppendSequence(const Sequence<T>& other) {
    for (int i = 0; i < other.GetLength(); i++)
        seq_->Append(other.Get(i));
    return *this;
}

template <class T>
MutableArraySequence<T>*
MutableArraySequence<T>::Builder::Build() {
    MutableArraySequence<T>* result = seq_;
    seq_ = new MutableArraySequence<T>();
    return result;
}
