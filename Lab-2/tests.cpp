#include <iostream>
#include <cassert>
#include <stdexcept>
#include <sstream>
#include <windows.h>

#include "Dynamic_Array.h"
#include "Linked_List.h"
#include "ArraySequence.h"
#include "ListSequence.h"
#include "BitSequence.h"
#include "AdaptiveSequence.h"
#include "MapReduce.h"

static int total = 0;
static int failed = 0;
static const char* current_suite = "";

static void ok(const char* desc) {
    ++total;
    std::cout << "  [PASS] " << desc << "\n";
}

static void fail(const char* desc, const char* file, int line, const char* expr) {
    ++total; ++failed;
    std::cout << "  [FAIL] " << desc << "\n"
        << "         " << file << ":" << line << " — " << expr << "\n";
}

#define CHECK(desc, expr) \
    do { if (expr) { ok(desc); } else { fail(desc, __FILE__, __LINE__, #expr); } } while(0)

#define CHECK_THROWS(desc, expr) \
    do { \
        bool _threw = false; \
        try { expr; } catch (...) { _threw = true; } \
        if (_threw) { ok(desc); } \
        else { fail(desc, __FILE__, __LINE__, "expected exception: " #expr); } \
    } while(0)

#define SUITE(name) \
    do { current_suite = name; \
         std::cout << "\n=== " << name << " ===\n"; } while(0)

// 1. DynamicArray
void test_DynamicArray() {
    SUITE("DynamicArray");

    {
        DynamicArray<int> arr(0);
        CHECK("empty array has size 0", arr.GetSize() == 0);
    }

    {
        int d[] = { 1,2,3 }; DynamicArray<int> arr(d, 3);
        CHECK("size from array", arr.GetSize() == 3);
        CHECK("first element", arr.Get(0) == 1);
        CHECK("last element", arr.Get(2) == 3);
    }

    {
        int d[] = { 10,20 }; DynamicArray<int> a(d, 2); DynamicArray<int> b(a);
        b.Set(0, 99);
        CHECK("copy is independent", a.Get(0) == 10);
    }

    {
        DynamicArray<int> arr(3); arr.Set(1, 42);
        CHECK("Set and Get", arr.Get(1) == 42);
    }

    {
        int d[] = { 1,2,3 }; DynamicArray<int> arr(d, 3); arr.Resize(5);
        CHECK("resize up keeps size", arr.GetSize() == 5);
        CHECK("resize up keeps elements", arr.Get(0) == 1);
    }

    {
        int d[] = { 1,2,3,4,5 }; DynamicArray<int> arr(d, 5); arr.Resize(2);
        CHECK("resize down keeps size", arr.GetSize() == 2);
        CHECK("resize down keeps elements", arr.Get(1) == 2);
    }

    {
        int d[] = { 1,2 }; DynamicArray<int> arr(d, 2); arr.Resize(0);
        CHECK("resize to 0", arr.GetSize() == 0);
    }

    {
        int d[] = { 7 }; DynamicArray<int> arr(d, 1);
        CHECK("single element", arr.Get(0) == 7);
    }

    CHECK_THROWS("negative size throws", DynamicArray<int>(-1));
    CHECK_THROWS("null ptr throws", DynamicArray<int>(nullptr, 3));
    {
        DynamicArray<int> arr(3);
        CHECK_THROWS("Get out of range (high)", arr.Get(3));
        CHECK_THROWS("Get out of range (neg)", arr.Get(-1));
    }
    {
        DynamicArray<int> arr(2);
        CHECK_THROWS("Set out of range", arr.Set(5, 1));
    }
    {
        DynamicArray<int> arr(2);
        CHECK_THROWS("Resize negative", arr.Resize(-1));
    }
}

// 2. LinkedList
void test_LinkedList() {
    SUITE("LinkedList");

    {
        LinkedList<int> l;
        CHECK("empty list length 0", l.GetLength() == 0);
    }

    {
        int d[] = { 1,2,3 }; LinkedList<int> l(d, 3);
        CHECK("length from array", l.GetLength() == 3);
        CHECK("GetFirst", l.GetFirst() == 1);
        CHECK("GetLast", l.GetLast() == 3);
    }

    {
        LinkedList<int> l; l.Append(10); l.Append(20);
        CHECK("Append Get(0)", l.Get(0) == 10);
        CHECK("Append Get(1)", l.Get(1) == 20);
    }

    {
        LinkedList<int> l; l.Append(2); l.Prepend(1);
        CHECK("Prepend", l.GetFirst() == 1);
    }

    {
        int d[] = { 1,3 }; LinkedList<int> l(d, 2); l.InsertAt(2, 1);
        CHECK("InsertAt value", l.Get(1) == 2);
        CHECK("InsertAt length", l.GetLength() == 3);
    }

    {
        int d[] = { 10,20,30,40 }; LinkedList<int> l(d, 4);
        auto sub = l.GetSubList(1, 2);
        CHECK("GetSubList length", sub.GetLength() == 2);
        CHECK("GetSubList element", sub.Get(0) == 20);
    }

    {
        int a[] = { 1,2 }, b[] = { 3,4 }; LinkedList<int> la(a, 2), lb(b, 2);
        auto r = la.Concat(lb);
        CHECK("Concat length", r.GetLength() == 4);
        CHECK("Concat element", r.Get(2) == 3);
    }

    {
        int d[] = { 1,2,3 }; LinkedList<int> a(d, 3); LinkedList<int> b(a);
        b.Append(99);
        CHECK("copy constructor independent", a.GetLength() == 3);
    }

    {
        LinkedList<int> l; l.Append(42);
        CHECK("single Append GetFirst", l.GetFirst() == 42);
        CHECK("single Append GetLast", l.GetLast() == 42);
    }

    {
        int d[] = { 2,3 }; LinkedList<int> l(d, 2); l.InsertAt(1, 0);
        CHECK("InsertAt front", l.GetFirst() == 1);
    }
    {
        int d[] = { 1,2 }; LinkedList<int> l(d, 2); l.InsertAt(3, 2);
        CHECK("InsertAt back", l.GetLast() == 3);
    }

    {
        LinkedList<int> l;
        CHECK_THROWS("GetFirst on empty", l.GetFirst());
    }
    {
        LinkedList<int> l;
        CHECK_THROWS("GetLast on empty", l.GetLast());
    }
    {
        int d[] = { 1,2 }; LinkedList<int> l(d, 2);
        CHECK_THROWS("Get out of range (high)", l.Get(5));
        CHECK_THROWS("Get out of range (neg)", l.Get(-1));
    }
    {
        LinkedList<int> l;
        CHECK_THROWS("InsertAt on empty", l.InsertAt(1, 5));
    }
    CHECK_THROWS("null+neg throws", LinkedList<int>(nullptr, -1));
}

// 3. MutableArraySequence
void test_MutableArraySequence() {
    SUITE("MutableArraySequence");

    {
        MutableArraySequence<int> s;
        CHECK("empty length 0", s.GetLength() == 0);
    }

    {
        int d[] = { 1,2,3 }; MutableArraySequence<int> s(d, 3);
        CHECK("length", s.GetLength() == 3);
        CHECK("GetFirst", s.GetFirst() == 1);
        CHECK("GetLast", s.GetLast() == 3);
    }

    {
        MutableArraySequence<int> s; s.Append(1); s.Append(2);
        CHECK("Append length", s.GetLength() == 2);
        CHECK("Append Get", s.Get(1) == 2);
    }

    {
        MutableArraySequence<int> s; s.Append(2); s.Prepend(1);
        CHECK("Prepend first", s.GetFirst() == 1);
        CHECK("Prepend length", s.GetLength() == 2);
    }

    {
        int d[] = { 1,3 }; MutableArraySequence<int> s(d, 2); s.InsertAt(2, 1);
        CHECK("InsertAt value", s.Get(1) == 2);
        CHECK("InsertAt length", s.GetLength() == 3);
    }

    {
        int d[] = { 10,20,30,40 }; MutableArraySequence<int> s(d, 4);
        auto* sub = s.GetSubsequence(1, 2);
        CHECK("GetSubsequence length", sub->GetLength() == 2);
        CHECK("GetSubsequence element", sub->Get(0) == 20);
        delete sub;
    }

    {
        int a[] = { 1,2 }, b[] = { 3,4 }; MutableArraySequence<int> sa(a, 2), sb(b, 2);
        auto* r = sa.Concat(sb);
        CHECK("Concat length", r->GetLength() == 4);
        CHECK("Concat element", r->Get(2) == 3);
        delete r;
    }

    {
        int d[] = { 5,10,15 }; MutableArraySequence<int> s(d, 3);
        CHECK("operator[] first", s[0] == 5);
        CHECK("operator[] last", s[2] == 15);
    }

    {
        int d[] = { 1,2,3 }; MutableArraySequence<int> s(d, 3);
        Sequence<int>* r = s.Append(4);
        CHECK("mutable Append returns this", r == &s);
        CHECK("mutable Append length", s.GetLength() == 4);
    }

    {
        MutableArraySequence<int> s;
        for (int i = 0; i < 100; ++i) s.Append(i);
        CHECK("100 elements length", s.GetLength() == 100);
        CHECK("100 elements last", s.Get(99) == 99);
    }

    {
        int d[] = { 1,2,3 }; MutableArraySequence<int> s(d, 3);
        auto* sub = s.GetSubsequence(0, 2);
        CHECK("GetSubsequence full", sub->GetLength() == 3);
        delete sub;
    }

    {
        int d[] = { 1,2,3 }; MutableArraySequence<int> s(d, 3);
        auto* sub = s.GetSubsequence(1, 1);
        CHECK("GetSubsequence single length", sub->GetLength() == 1);
        CHECK("GetSubsequence single element", sub->Get(0) == 2);
        delete sub;
    }

    {
        MutableArraySequence<int> s;
        CHECK_THROWS("GetFirst on empty", s.GetFirst());
    }
    {
        MutableArraySequence<int> s;
        CHECK_THROWS("GetLast on empty", s.GetLast());
    }
    {
        int d[] = { 1,2 }; MutableArraySequence<int> s(d, 2);
        CHECK_THROWS("Get out of range (high)", s.Get(5));
        CHECK_THROWS("Get out of range (neg)", s.Get(-1));
    }
    {
        MutableArraySequence<int> s;
        CHECK_THROWS("InsertAt on empty", s.InsertAt(1, 5));
    }
    {
        int d[] = { 1,2,3 }; MutableArraySequence<int> s(d, 3);
        CHECK_THROWS("GetSubsequence end<start", s.GetSubsequence(2, 1));
        CHECK_THROWS("GetSubsequence neg start", s.GetSubsequence(-1, 1));
        CHECK_THROWS("GetSubsequence OOB end", s.GetSubsequence(0, 5));
    }
    CHECK_THROWS("null ptr throws", MutableArraySequence<int>(nullptr, 3));
}

// 4. ImmutableArraySequence
void test_ImmutableArraySequence() {
    SUITE("ImmutableArraySequence");

    {
        int d[] = { 1,2,3 }; ImmutableArraySequence<int> s(d, 3);
        Sequence<int>* r = s.Append(4);
        CHECK("Append returns new", r != &s);
        CHECK("original unchanged", s.GetLength() == 3);
        CHECK("new has appended element", r->GetLength() == 4);
        delete r;
    }

    {
        int d[] = { 2,3 }; ImmutableArraySequence<int> s(d, 2);
        Sequence<int>* r = s.Prepend(1);
        CHECK("Prepend returns new", r != &s);
        CHECK("original first intact", s.GetFirst() == 2);
        CHECK("new first is prepended", r->GetFirst() == 1);
        delete r;
    }

    {
        int d[] = { 1,3 }; ImmutableArraySequence<int> s(d, 2);
        Sequence<int>* r = s.InsertAt(2, 1);
        CHECK("InsertAt original unchanged", s.GetLength() == 2);
        CHECK("InsertAt new length", r->GetLength() == 3);
        delete r;
    }
}

// 5. Builder (ArraySequence)
void test_Builder() {
    SUITE("Builder (ArraySequence)");

    {
        auto* s = MutableArraySequence<int>::Builder()
            .Append(1).Append(2).Append(3).Build();
        CHECK("builder chain length", s->GetLength() == 3);
        CHECK("builder chain element", s->Get(0) == 1);
        delete s;
    }

    {
        int d[] = { 10,20,30 };
        auto* s = MutableArraySequence<int>::Builder().AppendAll(d, 3).Build();
        CHECK("AppendAll length", s->GetLength() == 3);
        CHECK("AppendAll element", s->Get(2) == 30);
        delete s;
    }

    {
        int d[] = { 1,2 }; MutableArraySequence<int> src(d, 2);
        auto* s = MutableArraySequence<int>::Builder().AppendSequence(src).Build();
        CHECK("AppendSequence length", s->GetLength() == 2);
        delete s;
    }

    {
        MutableArraySequence<int>::Builder b;
        CHECK_THROWS("AppendAll null throws", b.AppendAll(nullptr, 3));
    }
}

// 6. Enumerator (ArraySequence)
void test_Enumerator() {
    SUITE("Enumerator (ArraySequence)");

    {
        int d[] = { 1,2,3 }; MutableArraySequence<int> s(d, 3);
        auto* en = s.get_enumerator(); int sum = 0;
        while (en->move_next()) sum += en->get_current();
        CHECK("iterate sum", sum == 6);
        delete en;
    }

    {
        int d[] = { 5,10 }; MutableArraySequence<int> s(d, 2);
        auto* en = s.get_enumerator();
        en->move_next(); en->reset(); en->move_next();
        CHECK("reset restarts iteration", en->get_current() == 5);
        delete en;
    }

    {
        int d[] = { 1 }; MutableArraySequence<int> s(d, 1);
        auto* en = s.get_enumerator();
        CHECK_THROWS("get_current before move_next throws", en->get_current());
        delete en;
    }

    {
        MutableArraySequence<int> s;
        auto* en = s.get_enumerator();
        CHECK("move_next on empty returns false", !en->move_next());
        delete en;
    }
}

// 7. Map / Where / Reduce
void test_MapReduce() {
    SUITE("Map / Where / Reduce");

    {
        int d[] = { 1,2,3 }; MutableArraySequence<int> s(d, 3);
        auto* r = Map<int, int>(&s, [](const int& x) { return x * 2; });
        CHECK("Map first element", r->Get(0) == 2);
        CHECK("Map last element", r->Get(2) == 6);
        delete r;
    }

    {
        int d[] = { 1,2,3,4,5 }; MutableArraySequence<int> s(d, 5);
        auto* r = Where<int>(&s, [](const int& x) { return x % 2 == 0; });
        CHECK("Where length", r->GetLength() == 2);
        CHECK("Where first element", r->Get(0) == 2);
        delete r;
    }

    {
        int d[] = { 1,2,3,4 }; MutableArraySequence<int> s(d, 4);
        int sum = Reduce<int, int>(&s, [](const int& a, const int& x) { return a + x; }, 0);
        CHECK("Reduce sum", sum == 10);
    }

    {
        MutableArraySequence<int> s;
        auto* r = Map<int, int>(&s, [](const int& x) { return x * 2; });
        CHECK("Map on empty", r->GetLength() == 0);
        delete r;
    }

    {
        int d[] = { 1,2,3 }; MutableArraySequence<int> s(d, 3);
        int p = Reduce<int, int>(&s, [](const int& a, const int& x) { return a * x; }, 1);
        CHECK("Reduce product", p == 6);
    }
}

// 8. Zip / Unzip
void test_Zip() {
    SUITE("Zip / Unzip");

    {
        int a[] = { 1,2,3 }, b[] = { 4,5,6 };
        MutableArraySequence<int> sa(a, 3), sb(b, 3);
        auto* z = Zip<int, int>(&sa, &sb);
        CHECK("Zip length", z->GetLength() == 3);
        CHECK("Zip first.first", z->Get(0).first == 1);
        CHECK("Zip first.second", z->Get(0).second == 4);
        delete z;
    }

    {
        int a[] = { 1,2,3 }, b[] = { 4,5 };
        MutableArraySequence<int> sa(a, 3), sb(b, 2);
        auto* z = Zip<int, int>(&sa, &sb);
        CHECK("Zip truncates to shorter", z->GetLength() == 2);
        delete z;
    }

    {
        int a[] = { 1,2 }, b[] = { 3,4 };
        MutableArraySequence<int> sa(a, 2), sb(b, 2);
        auto* z = Zip<int, int>(&sa, &sb);
        Sequence<int>* outA; Sequence<int>* outB;
        Unzip<int, int>(z, outA, outB);
        CHECK("Unzip first sequence", outA->Get(0) == 1);
        CHECK("Unzip second sequence", outB->Get(1) == 4);
        delete z; delete outA; delete outB;
    }
}

// 9. Algorithms
void test_Algorithms() {
    SUITE("Algorithms (П1-П8)");

    {
        int d[] = { 3,1,4,1,5 }; MutableArraySequence<int> s(d, 5);
        auto r = GetMinMaxAvg(s);
        CHECK("П1: min", r.min == 1);
        CHECK("П1: max", r.max == 5);
        CHECK("П1: avg", r.avg == 2.8);
    }

    {
        MutableArraySequence<int> s;
        CHECK_THROWS("П1: empty throws", GetMinMaxAvg(s));
    }

    {
        int d[] = { 3,1,2 }; MutableArraySequence<int> s(d, 3);
        CHECK("П2: median odd", GetMedian(s) == 2.0);
    }

    {
        int d[] = { 1,2,3,4 }; MutableArraySequence<int> s(d, 4);
        CHECK("П2: median even", GetMedian(s) == 2.5);
    }

    {
        MutableArraySequence<int> s;
        CHECK_THROWS("П2: empty throws", GetMedian(s));
    }

    {
        int d[] = { 1,2,3 }; MutableArraySequence<int> s(d, 3);
        CHECK("П3: sorted = 0 inversions", CountInversions(s) == 0);
    }

    {
        int d[] = { 3,2,1 }; MutableArraySequence<int> s(d, 3);
        CHECK("П3: reversed = 3 inversions", CountInversions(s) == 3);
    }

    {
        int d[] = { 1,2,3 }; MutableArraySequence<int> s(d, 3);
        auto* p = GetPrefixes(s);
        CHECK("П5: prefixes count", p->GetLength() == 3);
        CHECK("П5: first prefix length", p->Get(0)->GetLength() == 1);
        CHECK("П5: last prefix length", p->Get(2)->GetLength() == 3);
        for (int i = 0; i < p->GetLength(); ++i) delete p->Get(i);
        delete p;
    }

    {
        int d[] = { 1,2,3 }; MutableArraySequence<int> s(d, 3);
        auto* sf = GetSuffixes(s);
        CHECK("П5: suffixes count", sf->GetLength() == 3);
        CHECK("П5: first suffix length", sf->Get(0)->GetLength() == 3);
        CHECK("П5: last suffix length", sf->Get(2)->GetLength() == 1);
        for (int i = 0; i < sf->GetLength(); ++i) delete sf->Get(i);
        delete sf;
    }

    {
        int d[] = { 1,2,3 }; MutableArraySequence<int> s(d, 3);
        auto* avg = GetMovingAverage(s);
        CHECK("П6: moving avg edge", avg->Get(0) == 1.5);
        CHECK("П6: moving avg center", avg->Get(1) == 2.0);
        delete avg;
    }

    {
        int d[] = { 1,2,3 }; MutableArraySequence<int> s(d, 3);
        auto* m = GetMirrorSum(s);
        CHECK("П8: mirror sum first", m->Get(0) == 1 + 3);
        CHECK("П8: mirror sum middle", m->Get(1) == 2 + 2);
        delete m;
    }
}

// 10. BitSequence
void test_BitSequence() {
    SUITE("BitSequence");

    {
        BitSequence bs;
        CHECK("default length 0", bs.GetLength() == 0);
    }

    {
        BitSequence bs(8);
        CHECK("length 8", bs.GetLength() == 8);
        bool allZero = true;
        for (int i = 0; i < 8; ++i) if (bs.Get(i) != 0) allZero = false;
        CHECK("all bits zero by default", allZero);
    }

    {
        BitSequence bs(8); bs.Set(0, 1); bs.Set(3, 1);
        CHECK("Set bit 0", bs.Get(0) == 1);
        CHECK("unset bit 1", bs.Get(1) == 0);
        CHECK("Set bit 3", bs.Get(3) == 1);
    }

    {
        BitSequence bs(4); bs.Set(1, 1); bs.Flip(1);
        CHECK("Flip 1->0", bs.Get(1) == 0);
        bs.Flip(1);
        CHECK("Flip 0->1", bs.Get(1) == 1);
    }

    {
        BitSequence a(4); a.Set(0, 1); BitSequence b(a); b.Set(0, 0);
        CHECK("copy constructor independent", a.Get(0) == 1);
    }

    {
        BitSequence a(4); a.Set(2, 1); BitSequence b(4); b = a; b.Set(2, 0);
        CHECK("assignment independent", a.Get(2) == 1);
    }

    {
        BitSequence bs(4); bs.Set(2, 1);
        CHECK("operator[]", bs[2] == 1);
    }

    {
        BitSequence a(4), b(4); a.Set(0, 1); a.Set(1, 1); b.Set(1, 1); b.Set(2, 1);
        BitSequence r(4);
        a.AND(b, r);
        CHECK("AND bit0", r.Get(0) == 0);
        CHECK("AND bit1", r.Get(1) == 1);
        CHECK("AND bit2", r.Get(2) == 0);
    }

    {
        BitSequence a(4), b(4); a.Set(0, 1); b.Set(1, 1);
        BitSequence r(4);
        a.OR(b, r);
        CHECK("OR bit0", r.Get(0) == 1);
        CHECK("OR bit1", r.Get(1) == 1);
        CHECK("OR bit2", r.Get(2) == 0);
    }

    {
        BitSequence a(4), b(4); a.Set(0, 1); a.Set(1, 1); b.Set(1, 1); b.Set(2, 1);
        BitSequence r(4);
        a.XOR(b, r);
        CHECK("XOR bit0", r.Get(0) == 1);
        CHECK("XOR bit1", r.Get(1) == 0);
        CHECK("XOR bit2", r.Get(2) == 1);
    }

    {
        BitSequence a(4); a.Set(0, 1); a.Set(2, 1);
        BitSequence r(4);
        a.NOT(r);
        CHECK("NOT bit0", r.Get(0) == 0);
        CHECK("NOT bit1", r.Get(1) == 1);
        CHECK("NOT bit2", r.Get(2) == 0);
        CHECK("NOT bit3", r.Get(3) == 1);
    }

    {
        BitSequence a(4), b(4); a.Set(0, 1); b.Set(0, 1);
        auto r = a & b;
        CHECK("operator& bit0", r.Get(0) == 1);
    }

    {
        BitSequence a(4), b(4); a.Set(0, 1); b.Set(1, 1);
        auto r = a | b;
        CHECK("operator| bit0", r.Get(0) == 1);
        CHECK("operator| bit1", r.Get(1) == 1);
    }

    {
        BitSequence a(4), b(4); a.Set(0, 1); b.Set(0, 1);
        auto r = a ^ b;
        CHECK("operator^ cancels", r.Get(0) == 0);
    }

    {
        BitSequence a(4); a.Set(0, 1); auto r = ~a;
        CHECK("operator~ bit0", r.Get(0) == 0);
        CHECK("operator~ bit1", r.Get(1) == 1);
    }

    {
        BitSequence data(4), mask(4);
        data.Set(0, 1); data.Set(1, 1); data.Set(2, 1); data.Set(3, 1);
        mask.Set(0, 1); mask.Set(2, 1);
        auto r = data & mask;
        CHECK("bitmask bit0", r.Get(0) == 1);
        CHECK("bitmask bit1", r.Get(1) == 0);
        CHECK("bitmask bit2", r.Get(2) == 1);
        CHECK("bitmask bit3", r.Get(3) == 0);
    }

    {
        BitSequence a(9);
        BitSequence r(9);
        a.NOT(r);
        CHECK("NOT length 9", r.GetLength() == 9);
        bool allOne = true;
        for (int i = 0; i < 9; ++i) if (r.Get(i) != 1) allOne = false;
        CHECK("NOT all ones", allOne);
    }

    {
        BitSequence bs(1); bs.Set(0, 1);
        CHECK("single bit set", bs.Get(0) == 1);
        bs.Flip(0);
        CHECK("single bit flip", bs.Get(0) == 0);
    }

    {
        BitSequence bs(13);
        CHECK("length 13", bs.GetLength() == 13);
        bs.Set(12, 1);
        CHECK("last bit set", bs.Get(12) == 1);
    }

    CHECK_THROWS("negative size throws", BitSequence(-1));
    {
        BitSequence bs(4);
        CHECK_THROWS("Get neg index", bs.Get(-1));
        CHECK_THROWS("Get OOB index", bs.Get(4));
    }
    {
        BitSequence bs(4);
        CHECK_THROWS("Set neg index", bs.Set(-1, 1));
        CHECK_THROWS("Set OOB index", bs.Set(4, 1));
    }
    {
        BitSequence bs(4);
        CHECK_THROWS("Set value > 1", bs.Set(0, 2));
        CHECK_THROWS("Set value < 0", bs.Set(0, -1));
    }
    {
        BitSequence bs(4);
        CHECK_THROWS("Flip OOB", bs.Flip(10));
    }
    {
        BitSequence a(4), b(8), r(4);
        CHECK_THROWS("AND size mismatch", a.AND(b, r));
        CHECK_THROWS("OR size mismatch", a.OR(b, r));
        CHECK_THROWS("XOR size mismatch", a.XOR(b, r));
    }
}

// 11. AdaptiveSequence
void test_AdaptiveSequence() {
    SUITE("AdaptiveSequence");

    {
        AdaptiveSequence<int> s;
        CHECK("starts as array", s.IsArray());
        CHECK("empty length 0", s.GetLength() == 0);
    }

    {
        AdaptiveSequence<int> s; s.Append(1); s.Append(2); s.Append(3);
        CHECK("Append length", s.GetLength() == 3);
        CHECK("Append first", s.Get(0) == 1);
        CHECK("Append last", s.Get(2) == 3);
    }

    {
        AdaptiveSequence<int> s; s.Append(2); s.Prepend(1);
        CHECK("Prepend", s.GetFirst() == 1);
    }

    {
        AdaptiveSequence<int> s; s.Append(1); s.Append(3); s.InsertAt(2, 1);
        CHECK("InsertAt value", s.Get(1) == 2);
        CHECK("InsertAt length", s.GetLength() == 3);
    }

    {
        AdaptiveSequence<int> s; s.Append(10); s.Append(20);
        CHECK("GetFirst", s.GetFirst() == 10);
        CHECK("GetLast", s.GetLast() == 20);
    }

    {
        AdaptiveSequence<int> s; s.Append(5); s.Append(15);
        CHECK("operator[] first", s[0] == 5);
        CHECK("operator[] second", s[1] == 15);
    }

    {
        AdaptiveSequence<int> s;
        for (int i = 0; i < 5; ++i) s.Append(i);
        for (int i = 0; i < 15; ++i) s.InsertAt(99, 0);
        CHECK("many InsertAt switches to list", !s.IsArray());
    }

    {
        AdaptiveSequence<int> s;
        for (int i = 0; i < 5; ++i) s.Append(i);
        for (int i = 0; i < 15; ++i) s.InsertAt(0, 0);
        CHECK("switches to list", !s.IsArray());
        for (int i = 0; i < 25; ++i) s.Get(0);
        CHECK("many Get switches back to array", s.IsArray());
    }

    {
        AdaptiveSequence<int> s; s.Append(1); s.Append(2); s.Append(3);
        for (int i = 0; i < 15; ++i) s.InsertAt(0, 0);
        CHECK("original data preserved after switch [n-3]", s.Get(s.GetLength() - 3) == 1);
        CHECK("original data preserved after switch [n-2]", s.Get(s.GetLength() - 2) == 2);
        CHECK("original data preserved after switch [n-1]", s.Get(s.GetLength() - 1) == 3);
    }

    {
        AdaptiveSequence<int> s; s.Append(42);
        CHECK("single element GetFirst", s.GetFirst() == 42);
        CHECK("single element GetLast", s.GetLast() == 42);
        CHECK("single element length", s.GetLength() == 1);
    }

    {
        AdaptiveSequence<int> s;
        for (int i = 0; i < 5; ++i) s.Append(i);
        auto* sub = s.GetSubsequence(1, 3);
        CHECK("GetSubsequence length", sub->GetLength() == 3);
        CHECK("GetSubsequence element", sub->Get(0) == 1);
        delete sub;
    }

    {
        AdaptiveSequence<int> s;
        CHECK_THROWS("GetFirst on empty", s.GetFirst());
    }
    {
        AdaptiveSequence<int> s;
        CHECK_THROWS("GetLast on empty", s.GetLast());
    }
    {
        AdaptiveSequence<int> s; s.Append(1);
        CHECK_THROWS("Get OOB", s.Get(5));
        CHECK_THROWS("Get neg", s.Get(-1));
    }
}

// 12. MutableListSequence
void test_MutableListSequence() {
    SUITE("MutableListSequence");

    {
        MutableListSequence<int> s;
        CHECK("empty length 0", s.GetLength() == 0);
    }

    {
        int d[] = { 1,2,3 }; MutableListSequence<int> s(d, 3);
        CHECK("length", s.GetLength() == 3);
        CHECK("GetFirst", s.GetFirst() == 1);
        CHECK("GetLast", s.GetLast() == 3);
    }

    {
        int d[] = { 10,20,30 }; LinkedList<int> l(d, 3); MutableListSequence<int> s(l);
        CHECK("from LinkedList length", s.GetLength() == 3);
        CHECK("from LinkedList element", s.Get(1) == 20);
    }

    {
        int d[] = { 1,2,3 }; MutableListSequence<int> a(d, 3); MutableListSequence<int> b(a);
        b.Append(99);
        CHECK("copy independent", a.GetLength() == 3);
    }

    {
        MutableListSequence<int> s; s.Append(1); s.Append(2);
        CHECK("Append length", s.GetLength() == 2);
        CHECK("Append last", s.GetLast() == 2);
    }

    {
        MutableListSequence<int> s; s.Append(2); s.Prepend(1);
        CHECK("Prepend first", s.GetFirst() == 1);
        CHECK("Prepend length", s.GetLength() == 2);
    }

    {
        int d[] = { 1,3 }; MutableListSequence<int> s(d, 2); s.InsertAt(2, 1);
        CHECK("InsertAt value", s.Get(1) == 2);
        CHECK("InsertAt length", s.GetLength() == 3);
    }

    {
        int d[] = { 2,3 }; MutableListSequence<int> s(d, 2); s.InsertAt(1, 0);
        CHECK("InsertAt front", s.GetFirst() == 1);
    }

    {
        int d[] = { 1,2 }; MutableListSequence<int> s(d, 2); s.InsertAt(3, 2);
        CHECK("InsertAt back", s.GetLast() == 3);
    }

    {
        int d[] = { 10,20,30,40 }; MutableListSequence<int> s(d, 4);
        auto* sub = s.GetSubsequence(1, 2);
        CHECK("GetSubsequence length", sub->GetLength() == 2);
        CHECK("GetSubsequence element0", sub->Get(0) == 20);
        CHECK("GetSubsequence element1", sub->Get(1) == 30);
        delete sub;
    }

    {
        int d[] = { 1,2,3 }; MutableListSequence<int> s(d, 3);
        auto* sub = s.GetSubsequence(0, 2);
        CHECK("GetSubsequence full", sub->GetLength() == 3);
        delete sub;
    }

    {
        int d[] = { 1,2,3 }; MutableListSequence<int> s(d, 3);
        auto* sub = s.GetSubsequence(2, 2);
        CHECK("GetSubsequence single length", sub->GetLength() == 1);
        CHECK("GetSubsequence single element", sub->Get(0) == 3);
        delete sub;
    }

    {
        int a[] = { 1,2 }, b[] = { 3,4 }; MutableListSequence<int> sa(a, 2), sb(b, 2);
        auto* r = sa.Concat(sb);
        CHECK("Concat length", r->GetLength() == 4);
        CHECK("Concat element0", r->Get(0) == 1);
        CHECK("Concat element2", r->Get(2) == 3);
        delete r;
    }

    {
        int a[] = { 1,2 }, b[] = { 3,4 }; MutableListSequence<int> sa(a, 2), sb(b, 2);
        auto* r = sa + sb;
        CHECK("operator+ length", r->GetLength() == 4);
        delete r;
    }

    {
        int d[] = { 5,10,15 }; MutableListSequence<int> s(d, 3);
        CHECK("operator[] first", s[0] == 5);
        CHECK("operator[] last", s[2] == 15);
    }

    {
        int d[] = { 1,2,3 }; MutableListSequence<int> s(d, 3);
        Sequence<int>* r = s.Append(4);
        CHECK("mutable Append returns this", r == &s);
        CHECK("mutable Append length", s.GetLength() == 4);
    }

    {
        MutableListSequence<int> s;
        CHECK_THROWS("GetFirst on empty", s.GetFirst());
    }
    {
        MutableListSequence<int> s;
        CHECK_THROWS("GetLast on empty", s.GetLast());
    }
    {
        int d[] = { 1,2 }; MutableListSequence<int> s(d, 2);
        CHECK_THROWS("Get neg", s.Get(-1));
        CHECK_THROWS("Get OOB", s.Get(5));
    }
    {
        MutableListSequence<int> s;
        CHECK_THROWS("InsertAt on empty", s.InsertAt(1, 5));
    }
    {
        int d[] = { 1,2,3 }; MutableListSequence<int> s(d, 3);
        CHECK_THROWS("GetSubsequence end<start", s.GetSubsequence(2, 1));
        CHECK_THROWS("GetSubsequence neg start", s.GetSubsequence(-1, 1));
        CHECK_THROWS("GetSubsequence OOB end", s.GetSubsequence(0, 5));
    }
    CHECK_THROWS("null throws", MutableListSequence<int>(nullptr, 3));
    CHECK_THROWS("null neg throws", MutableListSequence<int>(nullptr, -1));
}

// 13. ImmutableListSequence
void test_ImmutableListSequence() {
    SUITE("ImmutableListSequence");

    {
        int d[] = { 1,2,3 }; ImmutableListSequence<int> s(d, 3);
        Sequence<int>* r = s.Append(4);
        CHECK("Append returns new", r != &s);
        CHECK("original unchanged", s.GetLength() == 3);
        CHECK("new has appended element", r->GetLength() == 4);
        delete r;
    }

    {
        int d[] = { 2,3 }; ImmutableListSequence<int> s(d, 2);
        Sequence<int>* r = s.Prepend(1);
        CHECK("Prepend returns new", r != &s);
        CHECK("original first intact", s.GetFirst() == 2);
        CHECK("new first is prepended", r->GetFirst() == 1);
        delete r;
    }

    {
        int d[] = { 1,3 }; ImmutableListSequence<int> s(d, 2);
        Sequence<int>* r = s.InsertAt(2, 1);
        CHECK("InsertAt returns new", r != &s);
        CHECK("InsertAt original unchanged", s.GetLength() == 2);
        CHECK("InsertAt new length", r->GetLength() == 3);
        delete r;
    }

    {
        int d[] = { 1,2,3 }; ImmutableListSequence<int> s(d, 3);
        auto* r1 = s.Append(4); auto* r2 = s.Prepend(0);
        CHECK("multiple ops original unchanged", s.GetLength() == 3);
        CHECK("Append new length", r1->GetLength() == 4);
        CHECK("Prepend new length", r2->GetLength() == 4);
        delete r1; delete r2;
    }
}

// 14. ListSequence Builder
void test_ListBuilder() {
    SUITE("Builder (ListSequence)");

    {
        auto* s = MutableListSequence<int>::Builder()
            .Append(1).Append(2).Append(3).Build();
        CHECK("chain length", s->GetLength() == 3);
        CHECK("chain first", s->Get(0) == 1);
        CHECK("chain last", s->Get(2) == 3);
        delete s;
    }

    {
        int d[] = { 10,20,30 };
        auto* s = MutableListSequence<int>::Builder().AppendAll(d, 3).Build();
        CHECK("AppendAll length", s->GetLength() == 3);
        CHECK("AppendAll element", s->Get(1) == 20);
        delete s;
    }

    {
        int d[] = { 1,2,3 }; MutableListSequence<int> src(d, 3);
        auto* s = MutableListSequence<int>::Builder().AppendSequence(src).Build();
        CHECK("AppendSequence length", s->GetLength() == 3);
        CHECK("AppendSequence element", s->Get(2) == 3);
        delete s;
    }

    {
        MutableListSequence<int>::Builder b;
        CHECK_THROWS("AppendAll null throws", b.AppendAll(nullptr, 3));
    }

    {
        auto builder = MutableListSequence<int>::Builder();
        builder.Append(1).Append(2);
        auto* first = builder.Build();
        builder.Append(99);
        auto* second = builder.Build();
        CHECK("Build resets builder (first)", first->GetLength() == 2);
        CHECK("Build resets builder (second)", second->GetLength() == 1);
        delete first; delete second;
    }
}

// 15. ListSequence Enumerator
void test_ListEnumerator() {
    SUITE("Enumerator (ListSequence)");

    {
        int d[] = { 1,2,3 }; MutableListSequence<int> s(d, 3);
        auto* en = s.get_enumerator(); int sum = 0;
        while (en->move_next()) sum += en->get_current();
        CHECK("iterate sum", sum == 6);
        delete en;
    }

    {
        int d[] = { 5,10 }; MutableListSequence<int> s(d, 2);
        auto* en = s.get_enumerator();
        en->move_next(); en->reset(); en->move_next();
        CHECK("reset restarts iteration", en->get_current() == 5);
        delete en;
    }

    {
        int d[] = { 1 }; MutableListSequence<int> s(d, 1);
        auto* en = s.get_enumerator();
        CHECK_THROWS("get_current before move_next throws", en->get_current());
        delete en;
    }

    {
        MutableListSequence<int> s;
        auto* en = s.get_enumerator();
        CHECK("move_next on empty returns false", !en->move_next());
        delete en;
    }
}

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
    std::cout << "\n────────────────────────────────────────\n";
    if (failed == 0) {
        std::cout << "OK  Все тесты пройдены: " << total << "/" << total << "\n";
    }
    else {
        std::cout << "FAIL  Провалено: " << failed << " / " << total << "\n";
    }
    return failed > 0 ? 1 : 0;
}