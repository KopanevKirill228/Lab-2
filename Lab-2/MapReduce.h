#pragma once

#include "Sequence.h"
#include "ArraySequence.h"
#include "Pair.h"
#include <cmath>
#include <stack>
#include <stdexcept>

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


// П-1: min, max, avg за один проход

template <class T>
struct MinMaxAvg { T min; T max; double avg; };

template <class T>
MinMaxAvg<T> GetMinMaxAvg(const Sequence<T>& seq) {
    if (seq.GetLength() == 0)
        throw std::out_of_range("Empty sequence");

    T mn = seq.Get(0), mx = seq.Get(0);
    double sum = 0;

    for (int i = 0; i < seq.GetLength(); i++) {
        const T& val = seq.Get(i);
        if (val < mn) mn = val;
        if (val > mx) mx = val;
        sum += val;
    }
    return { mn, mx, sum / seq.GetLength() };
}

// П-2: медиана (QuickSort + середина)

template <class T>
void QuickSort(T* arr, int left, int right) {
    if (left >= right) return;
    T pivot = arr[(left + right) / 2];
    int i = left, j = right;
    while (i <= j) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;
        if (i <= j) { T tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp; i++; j--; }
    }
    QuickSort(arr, left, j);
    QuickSort(arr, i, right);
}

template <class T>
double GetMedian(const Sequence<T>& seq) {
    int n = seq.GetLength();
    if (n == 0) throw std::out_of_range("Empty sequence");
    T* arr = new T[n];
    for (int i = 0; i < n; i++) arr[i] = seq.Get(i);
    QuickSort(arr, 0, n - 1);
    double result = (n % 2 == 0)
        ? (arr[n / 2 - 1] + arr[n / 2]) / 2.0
        : arr[n / 2];
    delete[] arr;
    return result;
}

// П-3: количество перестановок (инверсий)

template <class T>
int CountInversions(const Sequence<T>& seq) {
    int count = 0;
    int n = seq.GetLength();
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (seq.Get(i) > seq.Get(j))
                count++;
    return count;
}



// П-5: все префиксы и постфиксы

template <class T>
MutableArraySequence<MutableArraySequence<T>*>* GetPrefixes(const Sequence<T>& seq) {
    auto* result = new MutableArraySequence<MutableArraySequence<T>*>();
    for (int len = 1; len <= seq.GetLength(); len++) {
        auto* prefix = new MutableArraySequence<T>();
        for (int i = 0; i < len; i++)
            prefix->Append(seq.Get(i));
        result->Append(prefix);
    }
    return result;
}

template <class T>
MutableArraySequence<MutableArraySequence<T>*>* GetSuffixes(const Sequence<T>& seq) {
    auto* result = new MutableArraySequence<MutableArraySequence<T>*>();
    for (int start = 0; start < seq.GetLength(); start++) {
        auto* suffix = new MutableArraySequence<T>();
        for (int i = start; i < seq.GetLength(); i++)
            suffix->Append(seq.Get(i));
        result->Append(suffix);
    }
    return result;
}

// П-6: для каждого a_i: (a_{i-1} + a_i + a_{i+1}) / 3

template <class T>
MutableArraySequence<double>* GetMovingAverage(const Sequence<T>& seq) {
    auto* result = new MutableArraySequence<double>();
    int n = seq.GetLength();
    for (int i = 0; i < n; i++) {
        double sum = seq.Get(i);
        int cnt = 1;
        if (i > 0) { sum += seq.Get(i - 1); cnt++; }
        if (i < n - 1) { sum += seq.Get(i + 1); cnt++; }
        result->Append(sum / cnt);
    }
    return result;
}

// П-7: для каждого a_i: sqrt(sigma^2 - a_i^2)

template <class T>
MutableArraySequence<double>* GetSqrtVariance(const Sequence<T>& seq) {
    int n = seq.GetLength();
    double sum_sq = 0;
    for (int i = 0; i < n; i++)
        sum_sq += (double)seq.Get(i) * seq.Get(i);
    double sigma2 = sum_sq / n;

    auto* result = new MutableArraySequence<double>();
    for (int i = 0; i < n; i++) {
        double val = sigma2 - (double)seq.Get(i) * seq.Get(i);
        result->Append(val >= 0 ? std::sqrt(val) : 0.0);
    }
    return result;
}

// П-8: a_i + a_{n-i} (сумма с отражением)

template <class T>
MutableArraySequence<T>* GetMirrorSum(const Sequence<T>& seq) {
    int n = seq.GetLength();
    auto* result = new MutableArraySequence<T>();
    for (int i = 0; i < n; i++)
        result->Append(seq.Get(i) + seq.Get(n - 1 - i));
    return result;
}
