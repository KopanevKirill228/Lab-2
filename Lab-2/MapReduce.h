#pragma once
#include "Sequence.h"
#include "ArraySequence.h"
#include "Pair.h"

// MAP
// Принимает последовательность T, функцию T->U
// Возвращает новую последовательность U
template <class T, class U>
Sequence<U>* Map(const Sequence<T>* seq, U(*func)(const T&)) {
    MutableArraySequence<U>* result = new MutableArraySequence<U>();
    for (int i = 0; i < seq->GetLength(); ++i)
        result->Append(func(seq->Get(i)));
    return result;
}

// WHERE
// Принимает последовательность T, функцию T->bool
// Возвращает новую последовательность T только с элементами где func вернул true
template <class T>
Sequence<T>* Where(const Sequence<T>* seq, bool(*func)(const T&)) {
    MutableArraySequence<T>* result = new MutableArraySequence<T>();
    for (int i = 0; i < seq->GetLength(); ++i)
        if (func(seq->Get(i)))
            result->Append(seq->Get(i));
    return result;
}

// REDUCE
// Принимает последовательность T, функцию (U,T)->U и начальное значение
// Возвращает одно значение типа U
template <class T, class U>
U Reduce(const Sequence<T>* seq, U(*func)(const U&, const T&), const U& initial) {
    U result = initial;
    for (int i = 0; i < seq->GetLength(); ++i)
        result = func(result, seq->Get(i));
    return result;
}

// ZIP
// Берёт две последовательности T и U
// Возвращает последовательность Pair<T,U>
// Длина результата = минимум из длин двух последовательностей
template <class T, class U>
Sequence<Pair<T, U>>* Zip(const Sequence<T>* first, const Sequence<U>* second) {
    MutableArraySequence<Pair<T, U>>* result = new MutableArraySequence<Pair<T, U>>();
    int length = first->GetLength() < second->GetLength()
        ? first->GetLength()
        : second->GetLength();
    for (int i = 0; i < length; ++i)
        result->Append(Pair<T, U>(first->Get(i), second->Get(i)));
    return result;
}

// UNZIP
// Берёт последовательность Pair<T,U>
// Возвращает две отдельные последовательности через выходные параметры
template <class T, class U>
void Unzip(const Sequence<Pair<T, U>>* seq,
    Sequence<T>*& outFirst,
    Sequence<U>*& outSecond) {
    MutableArraySequence<T>* first = new MutableArraySequence<T>();
    MutableArraySequence<U>* second = new MutableArraySequence<U>();
    for (int i = 0; i < seq->GetLength(); ++i) {
        first->Append(seq->Get(i).first);
        second->Append(seq->Get(i).second);
    }
    outFirst = first;
    outSecond = second;
}

// ZIPN
// Принимает последовательность последовательностей одного типа T
// Возвращает транспонированную последовательность последовательностей
template <class T>
Sequence<Sequence<T>*>* ZipN(const Sequence<Sequence<T>*>* seqs) {
    if (seqs->GetLength() == 0)
        return new MutableArraySequence<Sequence<T>*>();

    // минимальная длина среди всех последовательностей
    int minLen = seqs->Get(0)->GetLength();
    for (int i = 1; i < seqs->GetLength(); ++i)
        if (seqs->Get(i)->GetLength() < minLen)
            minLen = seqs->Get(i)->GetLength();

    MutableArraySequence<Sequence<T>*>* result = new MutableArraySequence<Sequence<T>*>();

    for (int col = 0; col < minLen; ++col) {
        MutableArraySequence<T>* row = new MutableArraySequence<T>();
        for (int seqIdx = 0; seqIdx < seqs->GetLength(); ++seqIdx)
            row->Append(seqs->Get(seqIdx)->Get(col));
        result->Append(row);
    }

    return result;
}

// UNZIPN — обратная к ZipN
template <class T>
Sequence<Sequence<T>*>* UnZipN(const Sequence<Sequence<T>*>* seqs) {
    return ZipN(seqs);  // транспонирование — обратная операция к себе самой
}
