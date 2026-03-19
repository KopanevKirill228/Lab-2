#pragma once

#include "Sequence.h"
#include "ArraySequence.h"
#include "Pair.h"
#include <cmath>
#include <stack>
#include <stdexcept>

// MAP
template <class T, class U>
Sequence<U>* Map(const Sequence<T>* seq, U(*func)(const T&)) {
    MutableArraySequence<U>* result = new MutableArraySequence<U>();
    auto* en = seq->get_enumerator();
    while (en->move_next())
        result->Append(func(en->get_current()));
    delete en;
    return result;
}

// WHERE
template <class T>
Sequence<T>* Where(const Sequence<T>* seq, bool(*func)(const T&)) {
    MutableArraySequence<T>* result = new MutableArraySequence<T>();
    auto* en = seq->get_enumerator();
    while (en->move_next()) {
        if (func(en->get_current()))
            result->Append(en->get_current());
    }
    delete en;
    return result;
}

// REDUCE
template <class T, class U>
U Reduce(const Sequence<T>* seq, U(*func)(const U&, const T&), const U& initial) {
    U result = initial;
    auto* en = seq->get_enumerator();
    while (en->move_next())
        result = func(result, en->get_current());
    delete en;
    return result;
}

// ZIP
template <class T, class U>
Sequence<Pair<T, U>>* Zip(const Sequence<T>* first, const Sequence<U>* second) {
    MutableArraySequence<Pair<T, U>>* result = new MutableArraySequence<Pair<T, U>>();
    auto* en1 = first->get_enumerator();
    auto* en2 = second->get_enumerator();

    while (en1->move_next() && en2->move_next())
        result->Append(Pair<T, U>(en1->get_current(), en2->get_current()));

    delete en1;
    delete en2;
    return result;
}

// UNZIP
template <class T, class U>
void Unzip(const Sequence<Pair<T, U>>* seq,
    Sequence<T>*& outFirst,
    Sequence<U>*& outSecond) {

    MutableArraySequence<T>* first = new MutableArraySequence<T>();
    MutableArraySequence<U>* second = new MutableArraySequence<U>();

    auto* en = seq->get_enumerator();
    while (en->move_next()) {
        first->Append(en->get_current().first);
        second->Append(en->get_current().second);
    }
    delete en;

    outFirst = first;
    outSecond = second;
}

// ZIPN
template <class T>
Sequence<Sequence<T>*>* ZipN(const Sequence<Sequence<T>*>* seqs) {
    if (seqs->GetLength() == 0)
        return new MutableArraySequence<Sequence<T>*>();

    // Получаем минимальную длину
    int minLen = seqs->Get(0)->GetLength();
    for (int i = 1; i < seqs->GetLength(); ++i)
        if (seqs->Get(i)->GetLength() < minLen)
            minLen = seqs->Get(i)->GetLength();

    MutableArraySequence<Sequence<T>*>* result = new MutableArraySequence<Sequence<T>*>();

    // Для каждой колонки
    for (int col = 0; col < minLen; ++col) {
        MutableArraySequence<T>* row = new MutableArraySequence<T>();


        auto* seqs_en = seqs->get_enumerator();
        int seqIdx = 0;
        while (seqs_en->move_next()) {
            auto* current_seq = seqs_en->get_current();
            auto* col_en = current_seq->get_enumerator();
            for (int k = 0; k <= col && col_en->move_next(); ++k);
            if (col_en->move_next())
                row->Append(col_en->get_current());
            delete col_en;
            ++seqIdx;
        }
        delete seqs_en;
        result->Append(row);
    }

    return result;
}

// UNZIPN — обратная к ZipN
template <class T>
Sequence<Sequence<T>*>* UnZipN(const Sequence<Sequence<T>*>* seqs) {
    return ZipN(seqs);
}


// П-1: min, max, avg за один проход

template <class T>
struct MinMaxAvg { T min; T max; double avg; };

template <class T>
MinMaxAvg<T> GetMinMaxAvg(const Sequence<T>& seq) {
    if (seq.GetLength() == 0)
        throw std::out_of_range("Empty sequence");

    auto* en = seq.get_enumerator();
    en->move_next();
    T mn = en->get_current();
    T mx = mn;
    double sum = (double)mn;
    while (en->move_next()) {
        const T& val = en->get_current();
        if (val < mn) mn = val;
        if (val > mx) mx = val;
        sum += val;
    }
    delete en;

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
    auto* en = seq.get_enumerator();
    for (int i = 0; i < n && en->move_next(); i++)
        arr[i] = en->get_current();
    delete en;

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
    auto* result = new MutableArraySequence<double>();

    // Первый проход — считаем sum_sq
    auto* en = seq.get_enumerator();
    while (en->move_next())
        sum_sq += (double)en->get_current() * en->get_current();
    delete en;

    double sigma2 = sum_sq / n;

    auto* en2 = seq.get_enumerator();
    while (en2->move_next()) {
        double val = sigma2 - (double)en2->get_current() * en2->get_current();
        result->Append(val >= 0 ? std::sqrt(val) : 0.0);
    }
    delete en2;

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
