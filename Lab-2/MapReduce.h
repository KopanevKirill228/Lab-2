#pragma once

#include "Sequence.h"
#include "ArraySequence.h"
#include "Pair.h"
#include <cmath>
#include <stdexcept>
#include <functional>

// MAP — применяет func к каждому элементу
template <class T, class U>
Sequence<U>* Map(const Sequence<T>* seq, std::function<U(const T&)> func) {
    auto* result = new MutableArraySequence<U>();
    auto* en = seq->get_enumerator();
    while (en->move_next())
        result->Append(func(en->get_current()));
    delete en;
    return result;
}

// WHERE — оставляет только элементы где func = true
template <class T>
Sequence<T>* Where(const Sequence<T>* seq, std::function<bool(const T&)> func) {
    auto* result = new MutableArraySequence<T>();
    auto* en = seq->get_enumerator();
    while (en->move_next()) {
        const T& val = en->get_current();
        if (func(val))
            result->Append(val);
    }
    delete en;
    return result;
}

// REDUCE — сворачивает последовательность в одно значение
template <class T, class U>
U Reduce(const Sequence<T>* seq, std::function<U(const U&, const T&)> func, const U& initial) {
    U acc = initial;
    auto* en = seq->get_enumerator();
    while (en->move_next())
        acc = func(acc, en->get_current());
    delete en;
    return acc;
}

// ZIP — объединяет две последовательности в одну попарно
template <class T, class U>
Sequence<Pair<T, U>>* Zip(const Sequence<T>* first, const Sequence<U>* second) {
    auto* result = new MutableArraySequence<Pair<T, U>>();
    auto* en1 = first->get_enumerator();
    auto* en2 = second->get_enumerator();
    while (en1->move_next() && en2->move_next())
        result->Append(Pair<T, U>(en1->get_current(), en2->get_current()));
    delete en1;
    delete en2;
    return result;
}

// UNZIP — разбивает последовательность пар на две
template <class T, class U>
void Unzip(const Sequence<Pair<T, U>>* seq,
    Sequence<T>*& outFirst,
    Sequence<U>*& outSecond)
{
    auto* first = new MutableArraySequence<T>();
    auto* second = new MutableArraySequence<U>();
    auto* en = seq->get_enumerator();
    while (en->move_next()) {
        const Pair<T, U>& p = en->get_current();
        first->Append(p.first);
        second->Append(p.second);
    }
    delete en;
    outFirst = first;
    outSecond = second;
}

// П-1: минимум, максимум, среднее за один проход
template <class T>
struct MinMaxAvg { T min; T max; double avg; };

template <class T>
MinMaxAvg<T> GetMinMaxAvg(const Sequence<T>& seq) {
    if (seq.GetLength() == 0)
        throw std::out_of_range("Empty sequence");

    // Берём первый элемент как начальные min/max
    auto* en = seq.get_enumerator();
    en->move_next();
    T firstVal = en->get_current();
    delete en;

    T mn = firstVal;
    T mx = firstVal;
    double sum = 0.0;
    int cnt = 0;

    auto* en2 = seq.get_enumerator();
    while (en2->move_next()) {
        const T& val = en2->get_current();
        if (val < mn) mn = val;
        if (val > mx) mx = val;
        sum += (double)val;
        cnt++;
    }
    delete en2;

    return { mn, mx, sum / cnt };
}

// П-2: медиана — сортировка + середина
template <class T>
void QuickSort(T* arr, int left, int right) {
    if (left >= right) return;
    T pivot = arr[(left + right) / 2];
    int i = left, j = right;
    while (i <= j) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;
        if (i <= j) {
            T tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
            i++; j--;
        }
    }
    QuickSort(arr, left, j);
    QuickSort(arr, i, right);
}

template <class T>
double GetMedian(const Sequence<T>& seq) {
    int n = seq.GetLength();
    if (n == 0) throw std::out_of_range("Empty sequence");

    // Копируем в массив для сортировки
    T* arr = new T[n];
    auto* en = seq.get_enumerator();
    for (int i = 0; i < n && en->move_next(); i++)
        arr[i] = en->get_current();
    delete en;

    QuickSort(arr, 0, n - 1);

    double result = (n % 2 == 0) ? (arr[n / 2 - 1] + arr[n / 2]) / 2.0 : (double)arr[n / 2];

    delete[] arr;
    return result;
}

// П-3: количество инверсий
template <class T>
int CountInversions(const Sequence<T>& seq) {
    int n = seq.GetLength();
    int count = 0;

    T* arr = new T[n];
    auto* en = seq.get_enumerator();
    for (int i = 0; i < n && en->move_next(); i++)
        arr[i] = en->get_current();
    delete en;

    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (arr[i] > arr[j])
                count++;

    delete[] arr;
    return count;
}

// П-5: все префиксы [a0], [a0,a1], [a0,a1,a2]...
template <class T>
MutableArraySequence<MutableArraySequence<T>*>* GetPrefixes(const Sequence<T>& seq) {
    auto* result = new MutableArraySequence<MutableArraySequence<T>*>();
    int n = seq.GetLength();

    for (int len = 1; len <= n; len++) {
        auto* prefix = new MutableArraySequence<T>();
        auto* en = seq.get_enumerator();
        int i = 0;
        while (en->move_next() && i < len) {
            prefix->Append(en->get_current());
            i++;
        }
        delete en;
        result->Append(prefix);
    }
    return result;
}

// П-5: все суффиксы [a0..an], [a1..an], [an]...
template <class T>
MutableArraySequence<MutableArraySequence<T>*>* GetSuffixes(const Sequence<T>& seq) {
    auto* result = new MutableArraySequence<MutableArraySequence<T>*>();
    int n = seq.GetLength();

    for (int start = 0; start < n; start++) {
        auto* suffix = new MutableArraySequence<T>();
        auto* en = seq.get_enumerator();
        int i = 0;
        while (en->move_next()) {
            if (i >= start)
                suffix->Append(en->get_current());
            i++;
        }
        delete en;
        result->Append(suffix);
    }
    return result;
}

// П-6: скользящее среднее
template <class T>
MutableArraySequence<double>* GetMovingAverage(const Sequence<T>& seq) {
    int n = seq.GetLength();
    auto* result = new MutableArraySequence<double>();

    T* arr = new T[n];
    auto* en = seq.get_enumerator();
    for (int i = 0; i < n && en->move_next(); i++)
        arr[i] = en->get_current();
    delete en;

    for (int i = 0; i < n; i++) {
        double sum = (double)arr[i];
        int cnt = 1;
        if (i > 0) { sum += (double)arr[i - 1]; cnt++; }
        if (i < n - 1) { sum += (double)arr[i + 1]; cnt++; }
        result->Append(sum / cnt);
    }

    delete[] arr;
    return result;
}

// П-7: для каждого a_i: sqrt(sigma^2 - a_i^2)
template <class T>
MutableArraySequence<double>* GetSqrtVariance(const Sequence<T>& seq) {
    int n = seq.GetLength();
    if (n == 0) throw std::out_of_range("Empty sequence");

    double sum_sq = 0.0;
    auto* en1 = seq.get_enumerator();
    while (en1->move_next()) {
        double v = (double)en1->get_current();
        sum_sq += v * v;
    }
    delete en1;

    double sigma2 = sum_sq / n;

    auto* result = new MutableArraySequence<double>();
    auto* en2 = seq.get_enumerator();
    while (en2->move_next()) {
        double v = (double)en2->get_current();
        double diff = sigma2 - v * v;
        result->Append(diff >= 0.0 ? std::sqrt(diff) : 0.0);
    }
    delete en2;

    return result;
}

// П-8: a_i + a_{n-1-i} (сумма с зеркальным элементом) через zip + map
template <class T>
MutableArraySequence<T>* GetMirrorSum(const Sequence<T>& seq) {
    int n = seq.GetLength();

    // Строим обратную последовательность
    T* arr = new T[n];
    auto* en = seq.get_enumerator();
    for (int i = 0; i < n && en->move_next(); i++)
        arr[i] = en->get_current();
    delete en;

    auto* reversed = new MutableArraySequence<T>();
    for (int i = n - 1; i >= 0; i--)
        reversed->Append(arr[i]);
    delete[] arr;

    // Zip(seq, reversed) даёт пары (a_i, a_{n-1-i})
    auto* zipped = Zip<T, T>(&seq, reversed);

    // Map: пара → сумма
    auto* mapped = Map<Pair<T, T>, T>(zipped, [](const Pair<T, T>& p) -> T {
            return p.first + p.second;
        }
    );

    auto* result = new MutableArraySequence<T>();
    auto* en2 = mapped->get_enumerator();
    while (en2->move_next())
        result->Append(en2->get_current());
    delete en2;

    delete reversed;
    delete zipped;
    delete mapped;
    return result;
}