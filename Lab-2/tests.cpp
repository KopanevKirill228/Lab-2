#include <iostream>
#include <cassert>
#include <stdexcept>
#include <windows.h>

#include "Dynamic_Array.h"
#include "Linked_List.h"
#include "ArraySequence.h"
#include "ListSequence.h"
#include "BitSequence.h"
#include "AdaptiveSequence.h"
#include "MapReduce.h"

static int total = 0;
static void ok() { std::cout << "Тест номер " << ++total << " пройден\n"; }

template<typename F>
static void expectThrow(F f) {
    bool threw = false;
    try { f(); }
    catch (...) { threw = true; }
    assert(threw && "Ожидалось исключение");
}

template<typename F>
static void expectNoThrow(F f) {
    f();
}


//  1. DynamicArray


void test_DynamicArray() {
    { DynamicArray<int> arr(0); assert(arr.GetSize() == 0); ok(); }

    {
        int d[] = { 1,2,3 }; DynamicArray<int> arr(d, 3);
        assert(arr.GetSize() == 3); assert(arr.Get(0) == 1); assert(arr.Get(2) == 3); ok();
    }

    {
        int d[] = { 10,20 }; DynamicArray<int> a(d, 2); DynamicArray<int> b(a);
        b.Set(0, 99); assert(a.Get(0) == 10); ok();
    }

    { DynamicArray<int> arr(3); arr.Set(1, 42); assert(arr.Get(1) == 42); ok(); }

    {
        int d[] = { 1,2,3 }; DynamicArray<int> arr(d, 3); arr.Resize(5);
        assert(arr.GetSize() == 5); assert(arr.Get(0) == 1); ok();
    }

    {
        int d[] = { 1,2,3,4,5 }; DynamicArray<int> arr(d, 5); arr.Resize(2);
        assert(arr.GetSize() == 2); assert(arr.Get(1) == 2); ok();
    }

    {
        int d[] = { 1,2 }; DynamicArray<int> arr(d, 2); arr.Resize(0);
        assert(arr.GetSize() == 0); ok();
    }

    { int d[] = { 7 }; DynamicArray<int> arr(d, 1); assert(arr.Get(0) == 7); ok(); }

    expectThrow([] { DynamicArray<int>(-1); }); ok();
    expectThrow([] { DynamicArray<int>(nullptr, 3); }); ok();
    {
        DynamicArray<int> arr(3);
        expectThrow([&] { arr.Get(3); }); ok();
        expectThrow([&] { arr.Get(-1); }); ok();
    }
    { DynamicArray<int> arr(2); expectThrow([&] { arr.Set(5, 1); }); ok(); }
    { DynamicArray<int> arr(2); expectThrow([&] { arr.Resize(-1); }); ok(); }
}


//  2. LinkedList

void test_LinkedList() {
    { LinkedList<int> l; assert(l.GetLength() == 0); ok(); }

    {
        int d[] = { 1,2,3 }; LinkedList<int> l(d, 3);
        assert(l.GetLength() == 3); assert(l.GetFirst() == 1); assert(l.GetLast() == 3); ok();
    }

    {
        LinkedList<int> l; l.Append(10); l.Append(20);
        assert(l.Get(0) == 10); assert(l.Get(1) == 20); ok();
    }

    { LinkedList<int> l; l.Append(2); l.Prepend(1); assert(l.GetFirst() == 1); ok(); }

    {
        int d[] = { 1,3 }; LinkedList<int> l(d, 2); l.InsertAt(2, 1);
        assert(l.Get(1) == 2); assert(l.GetLength() == 3); ok();
    }

    {
        int d[] = { 10,20,30,40 }; LinkedList<int> l(d, 4);
        auto sub = l.GetSubList(1, 2);
        assert(sub.GetLength() == 2); assert(sub.Get(0) == 20); ok();
    }

    {
        int a[] = { 1,2 }, b[] = { 3,4 }; LinkedList<int> la(a, 2), lb(b, 2);
        auto result = la.Concat(lb);
        assert(result.GetLength() == 4); assert(result.Get(2) == 3); ok();
    }

    {
        int d[] = { 1,2,3 }; LinkedList<int> a(d, 3); LinkedList<int> b(a);
        b.Append(99); assert(a.GetLength() == 3); ok();
    }

    {
        LinkedList<int> l; l.Append(42);
        assert(l.GetFirst() == 42); assert(l.GetLast() == 42); ok();
    }

    { int d[] = { 2,3 }; LinkedList<int> l(d, 2); l.InsertAt(1, 0); assert(l.GetFirst() == 1); ok(); }
    { int d[] = { 1,2 }; LinkedList<int> l(d, 2); l.InsertAt(3, 2); assert(l.GetLast() == 3); ok(); }

    { LinkedList<int> l; expectThrow([&] { l.GetFirst(); }); ok(); }
    { LinkedList<int> l; expectThrow([&] { l.GetLast(); }); ok(); }
    {
        int d[] = { 1,2 }; LinkedList<int> l(d, 2);
        expectThrow([&] { l.Get(5); }); ok();
        expectThrow([&] { l.Get(-1); }); ok();
    }
    { LinkedList<int> l; expectThrow([&] { l.InsertAt(1, 5); }); ok(); }
    expectThrow([] { LinkedList<int>(nullptr, -1); }); ok();
}

//  3. MutableArraySequence

void test_MutableArraySequence() {
    { MutableArraySequence<int> s; assert(s.GetLength() == 0); ok(); }

    {
        int d[] = { 1,2,3 }; MutableArraySequence<int> s(d, 3);
        assert(s.GetLength() == 3); assert(s.GetFirst() == 1); assert(s.GetLast() == 3); ok();
    }

    {
        MutableArraySequence<int> s; s.Append(1); s.Append(2);
        assert(s.GetLength() == 2); assert(s.Get(1) == 2); ok();
    }

    {
        MutableArraySequence<int> s; s.Append(2); s.Prepend(1);
        assert(s.GetFirst() == 1); assert(s.GetLength() == 2); ok();
    }

    {
        int d[] = { 1,3 }; MutableArraySequence<int> s(d, 2); s.InsertAt(2, 1);
        assert(s.Get(1) == 2); assert(s.GetLength() == 3); ok();
    }

    {
        int d[] = { 10,20,30,40 }; MutableArraySequence<int> s(d, 4);
        auto* sub = s.GetSubsequence(1, 2);
        assert(sub->GetLength() == 2); assert(sub->Get(0) == 20); delete sub; ok();
    }

    {
        int a[] = { 1,2 }, b[] = { 3,4 }; MutableArraySequence<int> sa(a, 2), sb(b, 2);
        auto* r = sa.Concat(sb);
        assert(r->GetLength() == 4); assert(r->Get(2) == 3); delete r; ok();
    }

    {
        int d[] = { 5,10,15 }; MutableArraySequence<int> s(d, 3);
        assert(s[0] == 5); assert(s[2] == 15); ok();
    }

    {
        int d[] = { 1,2,3 }; MutableArraySequence<int> s(d, 3);
        Sequence<int>* r = s.Append(4);
        assert(r == &s); assert(s.GetLength() == 4); ok();
    }

    {
        MutableArraySequence<int> s;
        for (int i = 0; i < 100; ++i) s.Append(i);
        assert(s.GetLength() == 100); assert(s.Get(99) == 99); ok();
    }

    {
        int d[] = { 1,2,3 }; MutableArraySequence<int> s(d, 3);
        auto* sub = s.GetSubsequence(0, 2); assert(sub->GetLength() == 3); delete sub; ok();
    }

    {
        int d[] = { 1,2,3 }; MutableArraySequence<int> s(d, 3);
        auto* sub = s.GetSubsequence(1, 1);
        assert(sub->GetLength() == 1); assert(sub->Get(0) == 2); delete sub; ok();
    }

    { MutableArraySequence<int> s; expectThrow([&] { s.GetFirst(); }); ok(); }
    { MutableArraySequence<int> s; expectThrow([&] { s.GetLast(); }); ok(); }
    {
        int d[] = { 1,2 }; MutableArraySequence<int> s(d, 2);
        expectThrow([&] { s.Get(5); }); ok();
        expectThrow([&] { s.Get(-1); }); ok();
    }
    { MutableArraySequence<int> s; expectThrow([&] { s.InsertAt(1, 5); }); ok(); }
    {
        int d[] = { 1,2,3 }; MutableArraySequence<int> s(d, 3);
        expectThrow([&] { s.GetSubsequence(2, 1); }); ok();
        expectThrow([&] { s.GetSubsequence(-1, 1); }); ok();
        expectThrow([&] { s.GetSubsequence(0, 5); }); ok();
    }
    expectThrow([] { MutableArraySequence<int>(nullptr, 3); }); ok();
}


//  4. ImmutableArraySequence


void test_ImmutableArraySequence() {
    {
        int d[] = { 1,2,3 }; ImmutableArraySequence<int> s(d, 3);
        Sequence<int>* r = s.Append(4);
        assert(r != &s); assert(s.GetLength() == 3); assert(r->GetLength() == 4); delete r; ok();
    }

    {
        int d[] = { 2,3 }; ImmutableArraySequence<int> s(d, 2);
        Sequence<int>* r = s.Prepend(1);
        assert(r != &s); assert(s.GetFirst() == 2); assert(r->GetFirst() == 1); delete r; ok();
    }

    {
        int d[] = { 1,3 }; ImmutableArraySequence<int> s(d, 2);
        Sequence<int>* r = s.InsertAt(2, 1);
        assert(s.GetLength() == 2); assert(r->GetLength() == 3); delete r; ok();
    }
}

//  5. Builder (ArraySequence)


void test_Builder() {
    {
        auto* s = MutableArraySequence<int>::Builder()
            .Append(1).Append(2).Append(3).Build();
        assert(s->GetLength() == 3); assert(s->Get(0) == 1); delete s; ok();
    }

    {
        int d[] = { 10,20,30 };
        auto* s = MutableArraySequence<int>::Builder().AppendAll(d, 3).Build();
        assert(s->GetLength() == 3); assert(s->Get(2) == 30); delete s; ok();
    }

    {
        int d[] = { 1,2 }; MutableArraySequence<int> src(d, 2);
        auto* s = MutableArraySequence<int>::Builder().AppendSequence(src).Build();
        assert(s->GetLength() == 2); delete s; ok();
    }

    {
        MutableArraySequence<int>::Builder b;
        expectThrow([&] { b.AppendAll(nullptr, 3); }); ok();
    }
}


//  6. Enumerator (ArraySequence)


void test_Enumerator() {
    {
        int d[] = { 1,2,3 }; MutableArraySequence<int> s(d, 3);
        auto* en = s.get_enumerator(); int sum = 0;
        while (en->move_next()) sum += en->get_current();
        assert(sum == 6); delete en; ok();
    }

    {
        int d[] = { 5,10 }; MutableArraySequence<int> s(d, 2);
        auto* en = s.get_enumerator();
        en->move_next(); en->reset(); en->move_next();
        assert(en->get_current() == 5); delete en; ok();
    }

    {
        int d[] = { 1 }; MutableArraySequence<int> s(d, 1);
        auto* en = s.get_enumerator();
        expectThrow([&] { en->get_current(); }); delete en; ok();
    }

    {
        MutableArraySequence<int> s;
        auto* en = s.get_enumerator();
        assert(!en->move_next()); delete en; ok();
    }
}


//  7. Map / Where / Reduce


void test_MapReduce() {
    {
        int d[] = { 1,2,3 }; MutableArraySequence<int> s(d, 3);
        auto* r = Map<int, int>(&s, [](const int& x) { return x * 2; });
        assert(r->Get(0) == 2); assert(r->Get(2) == 6); delete r; ok();
    }

    {
        int d[] = { 1,2,3,4,5 }; MutableArraySequence<int> s(d, 5);
        auto* r = Where<int>(&s, [](const int& x) { return x % 2 == 0; });
        assert(r->GetLength() == 2); assert(r->Get(0) == 2); delete r; ok();
    }

    {
        int d[] = { 1,2,3,4 }; MutableArraySequence<int> s(d, 4);
        int sum = Reduce<int, int>(&s, [](const int& a, const int& x) { return a + x; }, 0);
        assert(sum == 10); ok();
    }

    {
        MutableArraySequence<int> s;
        auto* r = Map<int, int>(&s, [](const int& x) { return x * 2; });
        assert(r->GetLength() == 0); delete r; ok();
    }

    {
        int d[] = { 1,2,3 }; MutableArraySequence<int> s(d, 3);
        int p = Reduce<int, int>(&s, [](const int& a, const int& x) { return a * x; }, 1);
        assert(p == 6); ok();
    }
}


//  8. Zip / Unzip


void test_Zip() {
    {
        int a[] = { 1,2,3 }, b[] = { 4,5,6 };
        MutableArraySequence<int> sa(a, 3), sb(b, 3);
        auto* z = Zip<int, int>(&sa, &sb);
        assert(z->GetLength() == 3); assert(z->Get(0).first == 1); assert(z->Get(0).second == 4);
        delete z; ok();
    }

    {
        int a[] = { 1,2,3 }, b[] = { 4,5 };
        MutableArraySequence<int> sa(a, 3), sb(b, 2);
        auto* z = Zip<int, int>(&sa, &sb);
        assert(z->GetLength() == 2); delete z; ok();
    }

    {
        int a[] = { 1,2 }, b[] = { 3,4 };
        MutableArraySequence<int> sa(a, 2), sb(b, 2);
        auto* z = Zip<int, int>(&sa, &sb);
        Sequence<int>* outA; Sequence<int>* outB;
        Unzip<int, int>(z, outA, outB);
        assert(outA->Get(0) == 1); assert(outB->Get(1) == 4);
        delete z; delete outA; delete outB; ok();
    }
}


//  9. Algorithms


void test_Algorithms() {
    {
        int d[] = { 3,1,4,1,5 }; MutableArraySequence<int> s(d, 5);
        auto r = GetMinMaxAvg(s);
        assert(r.min == 1); assert(r.max == 5); assert(r.avg == 2.8); ok();
    }

    { MutableArraySequence<int> s; expectThrow([&] { GetMinMaxAvg(s); }); ok(); }

    {
        int d[] = { 3,1,2 }; MutableArraySequence<int> s(d, 3);
        assert(GetMedian(s) == 2.0); ok();
    }

    {
        int d[] = { 1,2,3,4 }; MutableArraySequence<int> s(d, 4);
        assert(GetMedian(s) == 2.5); ok();
    }

    { MutableArraySequence<int> s; expectThrow([&] { GetMedian(s); }); ok(); }

    {
        int d[] = { 1,2,3 }; MutableArraySequence<int> s(d, 3);
        assert(CountInversions(s) == 0); ok();
    }

    {
        int d[] = { 3,2,1 }; MutableArraySequence<int> s(d, 3);
        assert(CountInversions(s) == 3); ok();
    }

    {
        int d[] = { 1,2,3 }; MutableArraySequence<int> s(d, 3);
        auto* p = GetPrefixes(s);
        assert(p->GetLength() == 3); assert(p->Get(0)->GetLength() == 1); assert(p->Get(2)->GetLength() == 3);
        for (int i = 0; i < p->GetLength(); ++i) { delete p->Get(i); }
        delete p; ok();

    }

    {
        int d[] = { 1,2,3 }; MutableArraySequence<int> s(d, 3);
        auto* sf = GetSuffixes(s);
        assert(sf->GetLength() == 3); assert(sf->Get(0)->GetLength() == 3); assert(sf->Get(2)->GetLength() == 1);
        for (int i = 0; i < sf->GetLength(); ++i) { delete sf->Get(i); }
        delete sf; ok();

    }

    {
        int d[] = { 1,2,3 }; MutableArraySequence<int> s(d, 3);
        auto* avg = GetMovingAverage(s);
        assert(avg->Get(0) == 1.5); assert(avg->Get(1) == 2.0); delete avg; ok();
    }

    {
        int d[] = { 1,2,3 }; MutableArraySequence<int> s(d, 3);
        auto* m = GetMirrorSum(s);
        assert(m->Get(0) == 1 + 3); assert(m->Get(1) == 2 + 2); delete m; ok();
    }
}


//  10. BitSequence


void test_BitSequence() {
    { BitSequence bs; assert(bs.GetLength() == 0); ok(); }
    {
        BitSequence bs(8); assert(bs.GetLength() == 8);
        for (int i = 0; i < 8; ++i) { assert(bs.Get(i) == 0); }
        ok();
    }
    {
        BitSequence bs(8); bs.Set(0, 1); bs.Set(3, 1);
        assert(bs.Get(0) == 1); assert(bs.Get(1) == 0); assert(bs.Get(3) == 1); ok();
    }
    {
        BitSequence bs(4); bs.Set(1, 1); bs.Flip(1); assert(bs.Get(1) == 0);
        bs.Flip(1); assert(bs.Get(1) == 1); ok();
    }
    {
        BitSequence a(4); a.Set(0, 1); BitSequence b(a); b.Set(0, 0);
        assert(a.Get(0) == 1); ok();
    }
    {
        BitSequence a(4); a.Set(2, 1); BitSequence b(4); b = a; b.Set(2, 0);
        assert(a.Get(2) == 1); ok();
    }
    { BitSequence bs(4); bs.Set(2, 1); assert(bs[2] == 1); ok(); }
    {
        BitSequence a(4), b(4); a.Set(1, 1); b.Set(1, 1);
        assert(a == b); b.Set(0, 1); assert(!(a == b)); ok();
    }

    {
        BitSequence a(4), b(4); a.Set(0, 1); a.Set(1, 1); b.Set(1, 1); b.Set(2, 1);
        auto r = a.AND(b); assert(r.Get(0) == 0); assert(r.Get(1) == 1); assert(r.Get(2) == 0); ok();
    }
    {
        BitSequence a(4), b(4); a.Set(0, 1); b.Set(1, 1);
        auto r = a.OR(b); assert(r.Get(0) == 1); assert(r.Get(1) == 1); assert(r.Get(2) == 0); ok();
    }
    {
        BitSequence a(4), b(4); a.Set(0, 1); a.Set(1, 1); b.Set(1, 1); b.Set(2, 1);
        auto r = a.XOR(b); assert(r.Get(0) == 1); assert(r.Get(1) == 0); assert(r.Get(2) == 1); ok();
    }
    {
        BitSequence a(4); a.Set(0, 1); a.Set(2, 1); auto r = a.NOT();
        assert(r.Get(0) == 0); assert(r.Get(1) == 1); assert(r.Get(2) == 0); assert(r.Get(3) == 1); ok();
    }
    { BitSequence a(4), b(4); a.Set(0, 1); b.Set(0, 1); auto r = a & b; assert(r.Get(0) == 1); ok(); }
    {
        BitSequence a(4), b(4); a.Set(0, 1); b.Set(1, 1); auto r = a | b;
        assert(r.Get(0) == 1); assert(r.Get(1) == 1); ok();
    }
    { BitSequence a(4), b(4); a.Set(0, 1); b.Set(0, 1); auto r = a ^ b; assert(r.Get(0) == 0); ok(); }
    { BitSequence a(4); a.Set(0, 1); auto r = ~a; assert(r.Get(0) == 0); assert(r.Get(1) == 1); ok(); }

    {
        BitSequence data(4), mask(4);
        data.Set(0, 1); data.Set(1, 1); data.Set(2, 1); data.Set(3, 1);
        mask.Set(0, 1); mask.Set(2, 1);
        auto r = data & mask;
        assert(r.Get(0) == 1); assert(r.Get(1) == 0); assert(r.Get(2) == 1); assert(r.Get(3) == 0); ok();
    }

    {
        BitSequence a(9); auto r = a.NOT(); assert(r.GetLength() == 9);
        for (int i = 0; i < 9; ++i) { assert(r.Get(i) == 1); }
        ok();
    }

    { BitSequence bs(1); bs.Set(0, 1); assert(bs.Get(0) == 1); bs.Flip(0); assert(bs.Get(0) == 0); ok(); }
    { BitSequence bs(13); assert(bs.GetLength() == 13); bs.Set(12, 1); assert(bs.Get(12) == 1); ok(); }

    expectThrow([] { BitSequence(-1); }); ok();
    {
        BitSequence bs(4); expectThrow([&] { bs.Get(-1); }); ok();
        expectThrow([&] { bs.Get(4); }); ok();
    }
    {
        BitSequence bs(4); expectThrow([&] { bs.Set(-1, 1); }); ok();
        expectThrow([&] { bs.Set(4, 1); }); ok();
    }
    {
        BitSequence bs(4); expectThrow([&] { bs.Set(0, 2); }); ok();
        expectThrow([&] { bs.Set(0, -1); }); ok();
    }
    { BitSequence bs(4); expectThrow([&] { bs.Flip(10); }); ok(); }
    { BitSequence a(4), b(8); expectThrow([&] { a.AND(b); }); ok(); }
    { BitSequence a(4), b(8); expectThrow([&] { a.OR(b); }); ok(); }
    { BitSequence a(4), b(8); expectThrow([&] { a.XOR(b); }); ok(); }
}


//  11. AdaptiveSequence


void test_AdaptiveSequence() {
    { AdaptiveSequence<int> s; assert(s.IsArray()); assert(s.GetLength() == 0); ok(); }
    {
        AdaptiveSequence<int> s; s.Append(1); s.Append(2); s.Append(3);
        assert(s.GetLength() == 3); assert(s.Get(0) == 1); assert(s.Get(2) == 3); ok();
    }
    { AdaptiveSequence<int> s; s.Append(2); s.Prepend(1); assert(s.GetFirst() == 1); ok(); }
    {
        AdaptiveSequence<int> s; s.Append(1); s.Append(3); s.InsertAt(2, 1);
        assert(s.Get(1) == 2); assert(s.GetLength() == 3); ok();
    }
    {
        AdaptiveSequence<int> s; s.Append(10); s.Append(20);
        assert(s.GetFirst() == 10); assert(s.GetLast() == 20); ok();
    }
    {
        AdaptiveSequence<int> s; s.Append(5); s.Append(15);
        assert(s[0] == 5); assert(s[1] == 15); ok();
    }

    {
        AdaptiveSequence<int> s;
        for (int i = 0; i < 5; ++i) s.Append(i);
        for (int i = 0; i < 15; ++i) s.InsertAt(99, 0);
        assert(!s.IsArray()); ok();
    }

    {
        AdaptiveSequence<int> s;
        for (int i = 0; i < 5; ++i) s.Append(i);
        for (int i = 0; i < 15; ++i) s.InsertAt(0, 0);
        assert(!s.IsArray());
        for (int i = 0; i < 25; ++i) s.Get(0);
        assert(s.IsArray()); ok();
    }

    {
        AdaptiveSequence<int> s; s.Append(1); s.Append(2); s.Append(3);
        for (int i = 0; i < 15; ++i) s.InsertAt(0, 0);
        assert(s.Get(s.GetLength() - 3) == 1);
        assert(s.Get(s.GetLength() - 2) == 2);
        assert(s.Get(s.GetLength() - 1) == 3); ok();
    }

    {
        AdaptiveSequence<int> s; s.Append(42);
        assert(s.GetFirst() == 42); assert(s.GetLast() == 42); assert(s.GetLength() == 1); ok();
    }

    {
        AdaptiveSequence<int> s;
        for (int i = 0; i < 5; ++i) s.Append(i);
        auto* sub = s.GetSubsequence(1, 3);
        assert(sub->GetLength() == 3); assert(sub->Get(0) == 1); delete sub; ok();
    }

    { AdaptiveSequence<int> s; expectThrow([&] { s.GetFirst(); }); ok(); }
    { AdaptiveSequence<int> s; expectThrow([&] { s.GetLast(); }); ok(); }
    {
        AdaptiveSequence<int> s; s.Append(1);
        expectThrow([&] { s.Get(5); }); ok();
        expectThrow([&] { s.Get(-1); }); ok();
    }
}


//  12. MutableListSequence


void test_MutableListSequence() {
    { MutableListSequence<int> s; assert(s.GetLength() == 0); ok(); }
    {
        int d[] = { 1,2,3 }; MutableListSequence<int> s(d, 3);
        assert(s.GetLength() == 3); assert(s.GetFirst() == 1); assert(s.GetLast() == 3); ok();
    }
    {
        int d[] = { 10,20,30 }; LinkedList<int> l(d, 3); MutableListSequence<int> s(l);
        assert(s.GetLength() == 3); assert(s.Get(1) == 20); ok();
    }
    {
        int d[] = { 1,2,3 }; MutableListSequence<int> a(d, 3); MutableListSequence<int> b(a);
        b.Append(99); assert(a.GetLength() == 3); ok();
    }
    {
        MutableListSequence<int> s; s.Append(1); s.Append(2);
        assert(s.GetLength() == 2); assert(s.GetLast() == 2); ok();
    }
    {
        MutableListSequence<int> s; s.Append(2); s.Prepend(1);
        assert(s.GetFirst() == 1); assert(s.GetLength() == 2); ok();
    }
    {
        int d[] = { 1,3 }; MutableListSequence<int> s(d, 2); s.InsertAt(2, 1);
        assert(s.Get(1) == 2); assert(s.GetLength() == 3); ok();
    }
    {
        int d[] = { 2,3 }; MutableListSequence<int> s(d, 2); s.InsertAt(1, 0);
        assert(s.GetFirst() == 1); ok();
    }
    {
        int d[] = { 1,2 }; MutableListSequence<int> s(d, 2); s.InsertAt(3, 2);
        assert(s.GetLast() == 3); ok();
    }
    {
        int d[] = { 10,20,30,40 }; MutableListSequence<int> s(d, 4);
        auto* sub = s.GetSubsequence(1, 2);
        assert(sub->GetLength() == 2); assert(sub->Get(0) == 20); assert(sub->Get(1) == 30);
        delete sub; ok();
    }
    {
        int d[] = { 1,2,3 }; MutableListSequence<int> s(d, 3);
        auto* sub = s.GetSubsequence(0, 2); assert(sub->GetLength() == 3); delete sub; ok();
    }
    {
        int d[] = { 1,2,3 }; MutableListSequence<int> s(d, 3);
        auto* sub = s.GetSubsequence(2, 2);
        assert(sub->GetLength() == 1); assert(sub->Get(0) == 3); delete sub; ok();
    }
    {
        int a[] = { 1,2 }, b[] = { 3,4 }; MutableListSequence<int> sa(a, 2), sb(b, 2);
        auto* r = sa.Concat(sb);
        assert(r->GetLength() == 4); assert(r->Get(0) == 1); assert(r->Get(2) == 3); delete r; ok();
    }
    {
        int a[] = { 1,2 }, b[] = { 3,4 }; MutableListSequence<int> sa(a, 2), sb(b, 2);
        auto* r = sa + sb; assert(r->GetLength() == 4); delete r; ok();
    }
    {
        int d[] = { 5,10,15 }; MutableListSequence<int> s(d, 3);
        assert(s[0] == 5); assert(s[2] == 15); ok();
    }
    {
        int d[] = { 1,2,3 }; MutableListSequence<int> s(d, 3);
        Sequence<int>* r = s.Append(4); assert(r == &s); assert(s.GetLength() == 4); ok();
    }

    { MutableListSequence<int> s; expectThrow([&] { s.GetFirst(); }); ok(); }
    { MutableListSequence<int> s; expectThrow([&] { s.GetLast(); }); ok(); }
    {
        int d[] = { 1,2 }; MutableListSequence<int> s(d, 2);
        expectThrow([&] { s.Get(-1); }); ok();
        expectThrow([&] { s.Get(5); }); ok();
    }
    { MutableListSequence<int> s; expectThrow([&] { s.InsertAt(1, 5); }); ok(); }
    {
        int d[] = { 1,2,3 }; MutableListSequence<int> s(d, 3);
        expectThrow([&] { s.GetSubsequence(2, 1); }); ok();
        expectThrow([&] { s.GetSubsequence(-1, 1); }); ok();
        expectThrow([&] { s.GetSubsequence(0, 5); }); ok();
    }
    expectThrow([] { MutableListSequence<int>(nullptr, 3); }); ok();
    expectThrow([] { MutableListSequence<int>(nullptr, -1); }); ok();
}


//  13. ImmutableListSequence


void test_ImmutableListSequence() {
    {
        int d[] = { 1,2,3 }; ImmutableListSequence<int> s(d, 3);
        Sequence<int>* r = s.Append(4);
        assert(r != &s); assert(s.GetLength() == 3); assert(r->GetLength() == 4); delete r; ok();
    }
    {
        int d[] = { 2,3 }; ImmutableListSequence<int> s(d, 2);
        Sequence<int>* r = s.Prepend(1);
        assert(r != &s); assert(s.GetFirst() == 2); assert(r->GetFirst() == 1); delete r; ok();
    }
    {
        int d[] = { 1,3 }; ImmutableListSequence<int> s(d, 2);
        Sequence<int>* r = s.InsertAt(2, 1);
        assert(r != &s); assert(s.GetLength() == 2); assert(r->GetLength() == 3); delete r; ok();
    }
    {
        int d[] = { 1,2,3 }; ImmutableListSequence<int> s(d, 3);
        auto* r1 = s.Append(4); auto* r2 = s.Prepend(0);
        assert(s.GetLength() == 3); assert(r1->GetLength() == 4); assert(r2->GetLength() == 4);
        delete r1; delete r2; ok();
    }
}


//  14. ListSequence Builder


void test_ListBuilder() {
    {
        auto* s = MutableListSequence<int>::Builder()
            .Append(1).Append(2).Append(3).Build();
        assert(s->GetLength() == 3); assert(s->Get(0) == 1); assert(s->Get(2) == 3); delete s; ok();
    }
    {
        int d[] = { 10,20,30 };
        auto* s = MutableListSequence<int>::Builder().AppendAll(d, 3).Build();
        assert(s->GetLength() == 3); assert(s->Get(1) == 20); delete s; ok();
    }
    {
        int d[] = { 1,2,3 }; MutableListSequence<int> src(d, 3);
        auto* s = MutableListSequence<int>::Builder().AppendSequence(src).Build();
        assert(s->GetLength() == 3); assert(s->Get(2) == 3); delete s; ok();
    }
    {
        MutableListSequence<int>::Builder b;
        expectThrow([&] { b.AppendAll(nullptr, 3); }); ok();
    }
    {
        auto builder = MutableListSequence<int>::Builder();
        builder.Append(1).Append(2);
        auto* first = builder.Build();
        builder.Append(99);
        auto* second = builder.Build();
        assert(first->GetLength() == 2); assert(second->GetLength() == 1);
        delete first; delete second; ok();
    }
}


//  15. ListSequence Enumerator


void test_ListEnumerator() {
    {
        int d[] = { 1,2,3 }; MutableListSequence<int> s(d, 3);
        auto* en = s.get_enumerator(); int sum = 0;
        while (en->move_next()) sum += en->get_current();
        assert(sum == 6); delete en; ok();
    }
    {
        int d[] = { 5,10 }; MutableListSequence<int> s(d, 2);
        auto* en = s.get_enumerator();
        en->move_next(); en->reset(); en->move_next();
        assert(en->get_current() == 5); delete en; ok();
    }
    {
        int d[] = { 1 }; MutableListSequence<int> s(d, 1);
        auto* en = s.get_enumerator();
        expectThrow([&] { en->get_current(); }); delete en; ok();
    }
    {
        MutableListSequence<int> s;
        auto* en = s.get_enumerator();
        assert(!en->move_next()); delete en; ok();
    }
}


//  main


int main() {
    SetConsoleOutputCP(65001);

    test_DynamicArray();
    test_LinkedList();
    test_MutableArraySequence();
    test_ImmutableArraySequence();
    test_Builder();
    test_Enumerator();
    test_MapReduce();
    test_Zip();
    test_Algorithms();
    test_BitSequence();
    test_AdaptiveSequence();
    test_MutableListSequence();
    test_ImmutableListSequence();
    test_ListBuilder();
    test_ListEnumerator();

    std::cout << "\nУспешно! Всего тестов: " << total << "\n";
    return 0;
}
