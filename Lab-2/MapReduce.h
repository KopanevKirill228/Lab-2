#pragma once

#include "Sequence.h"
#include "ArraySequence.h"
#include "Pair.h"
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
    U accum = initial;
    auto* en = seq->get_enumerator();
    while (en->move_next())
        accum = func(accum, en->get_current());
    delete en;
    return accum;
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