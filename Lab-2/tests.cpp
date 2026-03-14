#include <gtest/gtest.h>
#include <stdexcept>
#include <windows.h>

#include "Dynamic_Array.h"
#include "Linked_List.h"
#include "ArraySequence.h"
#include "ListSequence.h"
#include "BitSequence.h"
#include "AdaptiveSequence.h"
#include "MapReduce.h"


// ═══════════════════════════════════════════════════════════════
//  Слушатель: выводит "Тест номер X пройден" и в конце "Успешно!"
// ═══════════════════════════════════════════════════════════════

class CustomListener : public ::testing::EmptyTestEventListener {
    int counter_ = 0;
public:
    void OnTestEnd(const ::testing::TestInfo& info) override {
        if (info.result()->Passed())
            std::cout << "Тест номер " << ++counter_ << " пройден\n";
    }
    void OnTestProgramEnd(const ::testing::UnitTest& unit) override {
        if (unit.successful_test_count() == unit.total_test_count())
            std::cout << "\nУспешно!\n";
    }
};

int main(int argc, char** argv) {
    SetConsoleOutputCP(65001); // UTF-8
    ::testing::InitGoogleTest(&argc, argv);
    auto& listeners = ::testing::UnitTest::GetInstance()->listeners();
    delete listeners.Release(listeners.default_result_printer());
    listeners.Append(new CustomListener);
    return RUN_ALL_TESTS();
}


// ═══════════════════════════════════════════════════════════════
//  1. DynamicArray — базовые операции
// ═══════════════════════════════════════════════════════════════

TEST(DynamicArray, DefaultConstructorEmptySize) {
    DynamicArray<int> arr(0);
    EXPECT_EQ(arr.GetSize(), 0);
}

TEST(DynamicArray, ConstructorFromItems) {
    int data[] = { 1, 2, 3 };
    DynamicArray<int> arr(data, 3);
    EXPECT_EQ(arr.GetSize(), 3);
    EXPECT_EQ(arr.Get(0), 1);
    EXPECT_EQ(arr.Get(2), 3);
}

TEST(DynamicArray, CopyConstructor) {
    int data[] = { 10, 20 };
    DynamicArray<int> a(data, 2);
    DynamicArray<int> b(a);
    b.Set(0, 99);
    EXPECT_EQ(a.Get(0), 10); // глубокая копия
}

TEST(DynamicArray, SetAndGet) {
    DynamicArray<int> arr(3);
    arr.Set(1, 42);
    EXPECT_EQ(arr.Get(1), 42);
}

TEST(DynamicArray, ResizeGrow) {
    int data[] = { 1, 2, 3 };
    DynamicArray<int> arr(data, 3);
    arr.Resize(5);
    EXPECT_EQ(arr.GetSize(), 5);
    EXPECT_EQ(arr.Get(0), 1);
}

TEST(DynamicArray, ResizeShrink) {
    int data[] = { 1, 2, 3, 4, 5 };
    DynamicArray<int> arr(data, 5);
    arr.Resize(2);
    EXPECT_EQ(arr.GetSize(), 2);
    EXPECT_EQ(arr.Get(1), 2);
}

// ═══════════════════════════════════════════════════════════════
//  2. DynamicArray — пограничные ситуации
// ═══════════════════════════════════════════════════════════════

TEST(DynamicArray, ResizeToZero) {
    int data[] = { 1, 2 };
    DynamicArray<int> arr(data, 2);
    arr.Resize(0);
    EXPECT_EQ(arr.GetSize(), 0);
}

TEST(DynamicArray, SingleElement) {
    int data[] = { 7 };
    DynamicArray<int> arr(data, 1);
    EXPECT_EQ(arr.Get(0), 7);
}

// ═══════════════════════════════════════════════════════════════
//  3. DynamicArray — некорректные ситуации
// ═══════════════════════════════════════════════════════════════

TEST(DynamicArray, NegativeSizeThrows) {
    EXPECT_THROW(DynamicArray<int>(-1), std::invalid_argument);
}

TEST(DynamicArray, NullptrWithCountThrows) {
    EXPECT_THROW(DynamicArray<int>(nullptr, 3), std::invalid_argument);
}

TEST(DynamicArray, GetOutOfRangeThrows) {
    DynamicArray<int> arr(3);
    EXPECT_THROW(arr.Get(3), std::out_of_range);
    EXPECT_THROW(arr.Get(-1), std::out_of_range);
}

TEST(DynamicArray, SetOutOfRangeThrows) {
    DynamicArray<int> arr(2);
    EXPECT_THROW(arr.Set(5, 1), std::out_of_range);
}

TEST(DynamicArray, NegativeResizeThrows) {
    DynamicArray<int> arr(2);
    EXPECT_THROW(arr.Resize(-1), std::invalid_argument);
}

// ═══════════════════════════════════════════════════════════════
//  4. LinkedList — базовые операции
// ═══════════════════════════════════════════════════════════════

TEST(LinkedList, DefaultConstructorEmpty) {
    LinkedList<int> list;
    EXPECT_EQ(list.GetLength(), 0);
}

TEST(LinkedList, ConstructorFromItems) {
    int data[] = { 1, 2, 3 };
    LinkedList<int> list(data, 3);
    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.GetFirst(), 1);
    EXPECT_EQ(list.GetLast(), 3);
}

TEST(LinkedList, AppendAndGet) {
    LinkedList<int> list;
    list.Append(10);
    list.Append(20);
    EXPECT_EQ(list.Get(0), 10);
    EXPECT_EQ(list.Get(1), 20);
}

TEST(LinkedList, Prepend) {
    LinkedList<int> list;
    list.Append(2);
    list.Prepend(1);
    EXPECT_EQ(list.GetFirst(), 1);
}

TEST(LinkedList, InsertAt) {
    int data[] = { 1, 3 };
    LinkedList<int> list(data, 2);
    list.InsertAt(2, 1);
    EXPECT_EQ(list.Get(1), 2);
    EXPECT_EQ(list.GetLength(), 3);
}

TEST(LinkedList, GetSubList) {
    int data[] = { 10, 20, 30, 40 };
    LinkedList<int> list(data, 4);
    auto* sub = list.GetSubList(1, 2);
    EXPECT_EQ(sub->GetLength(), 2);
    EXPECT_EQ(sub->Get(0), 20);
    delete sub;
}

TEST(LinkedList, Concat) {
    int a[] = { 1, 2 }, b[] = { 3, 4 };
    LinkedList<int> la(a, 2), lb(b, 2);
    auto result = la.Concat(lb);
    EXPECT_EQ(result.GetLength(), 4);
    EXPECT_EQ(result.Get(2), 3);
}

TEST(LinkedList, CopyConstructorDeep) {
    int data[] = { 1, 2, 3 };
    LinkedList<int> a(data, 3);
    LinkedList<int> b(a);
    b.Append(99);
    EXPECT_EQ(a.GetLength(), 3);
}

// ═══════════════════════════════════════════════════════════════
//  5. LinkedList — пограничные ситуации
// ═══════════════════════════════════════════════════════════════

TEST(LinkedList, SingleElementList) {
    LinkedList<int> list;
    list.Append(42);
    EXPECT_EQ(list.GetFirst(), 42);
    EXPECT_EQ(list.GetLast(), 42);
}

TEST(LinkedList, InsertAtBegin) {
    int data[] = { 2, 3 };
    LinkedList<int> list(data, 2);
    list.InsertAt(1, 0);
    EXPECT_EQ(list.GetFirst(), 1);
}

TEST(LinkedList, InsertAtEnd) {
    int data[] = { 1, 2 };
    LinkedList<int> list(data, 2);
    list.InsertAt(3, 2);
    EXPECT_EQ(list.GetLast(), 3);
}

// ═══════════════════════════════════════════════════════════════
//  6. LinkedList — некорректные ситуации
// ═══════════════════════════════════════════════════════════════

TEST(LinkedList, GetFirstEmptyThrows) {
    LinkedList<int> list;
    EXPECT_THROW(list.GetFirst(), std::out_of_range);
}

TEST(LinkedList, GetLastEmptyThrows) {
    LinkedList<int> list;
    EXPECT_THROW(list.GetLast(), std::out_of_range);
}

TEST(LinkedList, GetOutOfRangeThrows) {
    int data[] = { 1, 2 };
    LinkedList<int> list(data, 2);
    EXPECT_THROW(list.Get(5), std::out_of_range);
    EXPECT_THROW(list.Get(-1), std::out_of_range);
}

TEST(LinkedList, InsertAtOutOfRangeThrows) {
    LinkedList<int> list;
    EXPECT_THROW(list.InsertAt(1, 5), std::out_of_range);
}

TEST(LinkedList, NegativeCountThrows) {
    EXPECT_THROW(LinkedList<int>(nullptr, -1), std::invalid_argument);
}

// ═══════════════════════════════════════════════════════════════
//  7. MutableArraySequence — базовые операции
// ═══════════════════════════════════════════════════════════════

TEST(MutableArraySequence, DefaultConstructorEmpty) {
    MutableArraySequence<int> seq;
    EXPECT_EQ(seq.GetLength(), 0);
}

TEST(MutableArraySequence, ConstructorFromItems) {
    int data[] = { 1, 2, 3 };
    MutableArraySequence<int> seq(data, 3);
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.GetFirst(), 1);
    EXPECT_EQ(seq.GetLast(), 3);
}

TEST(MutableArraySequence, Append) {
    MutableArraySequence<int> seq;
    seq.Append(1);
    seq.Append(2);
    EXPECT_EQ(seq.GetLength(), 2);
    EXPECT_EQ(seq.Get(1), 2);
}

TEST(MutableArraySequence, Prepend) {
    MutableArraySequence<int> seq;
    seq.Append(2);
    seq.Prepend(1);
    EXPECT_EQ(seq.GetFirst(), 1);
    EXPECT_EQ(seq.GetLength(), 2);
}

TEST(MutableArraySequence, InsertAt) {
    int data[] = { 1, 3 };
    MutableArraySequence<int> seq(data, 2);
    seq.InsertAt(2, 1);
    EXPECT_EQ(seq.Get(1), 2);
    EXPECT_EQ(seq.GetLength(), 3);
}

TEST(MutableArraySequence, GetSubsequence) {
    int data[] = { 10, 20, 30, 40 };
    MutableArraySequence<int> seq(data, 4);
    auto* sub = seq.GetSubsequence(1, 2);
    EXPECT_EQ(sub->GetLength(), 2);
    EXPECT_EQ(sub->Get(0), 20);
    delete sub;
}

TEST(MutableArraySequence, Concat) {
    int a[] = { 1, 2 }, b[] = { 3, 4 };
    MutableArraySequence<int> sa(a, 2), sb(b, 2);
    auto* result = sa.Concat(sb);
    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(2), 3);
    delete result;
}

TEST(MutableArraySequence, OperatorBracket) {
    int data[] = { 5, 10, 15 };
    MutableArraySequence<int> seq(data, 3);
    EXPECT_EQ(seq[0], 5);
    EXPECT_EQ(seq[2], 15);
}

TEST(MutableArraySequence, IsMutableInPlace) {
    int data[] = { 1, 2, 3 };
    MutableArraySequence<int> seq(data, 3);
    Sequence<int>* result = seq.Append(4);
    EXPECT_EQ(result, &seq); // mutable — тот же объект
    EXPECT_EQ(seq.GetLength(), 4);
}

//  8. MutableArraySequence — пограничные ситуации

TEST(MutableArraySequence, AppendManyCapacityGrow) {
    MutableArraySequence<int> seq;
    for (int i = 0; i < 100; ++i)
        seq.Append(i);
    EXPECT_EQ(seq.GetLength(), 100);
    EXPECT_EQ(seq.Get(99), 99);
}

TEST(MutableArraySequence, SubsequenceFullRange) {
    int data[] = { 1, 2, 3 };
    MutableArraySequence<int> seq(data, 3);
    auto* sub = seq.GetSubsequence(0, 2);
    EXPECT_EQ(sub->GetLength(), 3);
    delete sub;
}

TEST(MutableArraySequence, SubsequenceSingleElement) {
    int data[] = { 1, 2, 3 };
    MutableArraySequence<int> seq(data, 3);
    auto* sub = seq.GetSubsequence(1, 1);
    EXPECT_EQ(sub->GetLength(), 1);
    EXPECT_EQ(sub->Get(0), 2);
    delete sub;
}

// ═══════════════════════════════════════════════════════════════
//  9. MutableArraySequence — некорректные ситуации
// ═══════════════════════════════════════════════════════════════

TEST(MutableArraySequence, GetFirstEmptyThrows) {
    MutableArraySequence<int> seq;
    EXPECT_THROW(seq.GetFirst(), std::out_of_range);
}

TEST(MutableArraySequence, GetLastEmptyThrows) {
    MutableArraySequence<int> seq;
    EXPECT_THROW(seq.GetLast(), std::out_of_range);
}

TEST(MutableArraySequence, GetOutOfRangeThrows) {
    int data[] = { 1, 2 };
    MutableArraySequence<int> seq(data, 2);
    EXPECT_THROW(seq.Get(5), std::out_of_range);
    EXPECT_THROW(seq.Get(-1), std::out_of_range);
}

TEST(MutableArraySequence, InsertAtOutOfRangeThrows) {
    MutableArraySequence<int> seq;
    EXPECT_THROW(seq.InsertAt(1, 5), std::out_of_range);
}

TEST(MutableArraySequence, SubsequenceInvalidRangeThrows) {
    int data[] = { 1, 2, 3 };
    MutableArraySequence<int> seq(data, 3);
    EXPECT_THROW(seq.GetSubsequence(2, 1), std::out_of_range);
    EXPECT_THROW(seq.GetSubsequence(-1, 1), std::out_of_range);
    EXPECT_THROW(seq.GetSubsequence(0, 5), std::out_of_range);
}

TEST(MutableArraySequence, NullptrConstructorThrows) {
    EXPECT_THROW(MutableArraySequence<int>(nullptr, 3), std::invalid_argument);
}

// ═══════════════════════════════════════════════════════════════
//  10. ImmutableArraySequence — неизменяемость
// ═══════════════════════════════════════════════════════════════

TEST(ImmutableArraySequence, AppendReturnsNewObject) {
    int data[] = { 1, 2, 3 };
    ImmutableArraySequence<int> seq(data, 3);
    Sequence<int>* result = seq.Append(4);
    EXPECT_NE(result, &seq);        // новый объект
    EXPECT_EQ(seq.GetLength(), 3);  // оригинал не изменился
    EXPECT_EQ(result->GetLength(), 4);
    delete result;
}

TEST(ImmutableArraySequence, PrependReturnsNewObject) {
    int data[] = { 2, 3 };
    ImmutableArraySequence<int> seq(data, 2);
    Sequence<int>* result = seq.Prepend(1);
    EXPECT_NE(result, &seq);
    EXPECT_EQ(seq.GetFirst(), 2);
    EXPECT_EQ(result->GetFirst(), 1);
    delete result;
}

TEST(ImmutableArraySequence, OriginalUnchangedAfterInsert) {
    int data[] = { 1, 3 };
    ImmutableArraySequence<int> seq(data, 2);
    Sequence<int>* result = seq.InsertAt(2, 1);
    EXPECT_EQ(seq.GetLength(), 2);
    EXPECT_EQ(result->GetLength(), 3);
    delete result;
}

// ═══════════════════════════════════════════════════════════════
//  11. Builder
// ═══════════════════════════════════════════════════════════════

TEST(Builder, AppendAndBuild) {
    auto* seq = MutableArraySequence<int>::Builder()
        .Append(1).Append(2).Append(3)
        .Build();
    EXPECT_EQ(seq->GetLength(), 3);
    EXPECT_EQ(seq->Get(0), 1);
    delete seq;
}

TEST(Builder, AppendAll) {
    int data[] = { 10, 20, 30 };
    auto* seq = MutableArraySequence<int>::Builder()
        .AppendAll(data, 3)
        .Build();
    EXPECT_EQ(seq->GetLength(), 3);
    EXPECT_EQ(seq->Get(2), 30);
    delete seq;
}

TEST(Builder, AppendSequence) {
    int data[] = { 1, 2 };
    MutableArraySequence<int> source(data, 2);
    auto* seq = MutableArraySequence<int>::Builder()
        .AppendSequence(source)
        .Build();
    EXPECT_EQ(seq->GetLength(), 2);
    delete seq;
}

TEST(Builder, NullptrThrows) {
    MutableArraySequence<int>::Builder b;
    EXPECT_THROW(b.AppendAll(nullptr, 3), std::invalid_argument);
}

// ═══════════════════════════════════════════════════════════════
//  12. Enumerator
// ═══════════════════════════════════════════════════════════════

TEST(Enumerator, IteratesAllElements) {
    int data[] = { 1, 2, 3 };
    MutableArraySequence<int> seq(data, 3);
    auto* en = seq.get_enumerator();
    int sum = 0;
    while (en->move_next())
        sum += en->get_current();
    EXPECT_EQ(sum, 6);
    delete en;
}

TEST(Enumerator, ResetWorks) {
    int data[] = { 5, 10 };
    MutableArraySequence<int> seq(data, 2);
    auto* en = seq.get_enumerator();
    en->move_next();
    en->reset();
    en->move_next();
    EXPECT_EQ(en->get_current(), 5);
    delete en;
}

TEST(Enumerator, GetCurrentBeforeMoveNextThrows) {
    int data[] = { 1 };
    MutableArraySequence<int> seq(data, 1);
    auto* en = seq.get_enumerator();
    EXPECT_THROW(en->get_current(), std::out_of_range);
    delete en;
}

TEST(Enumerator, EmptySequenceNoMove) {
    MutableArraySequence<int> seq;
    auto* en = seq.get_enumerator();
    EXPECT_FALSE(en->move_next());
    delete en;
}

// ═══════════════════════════════════════════════════════════════
//  13. Map / Where / Reduce
// ═══════════════════════════════════════════════════════════════

TEST(MapReduce, Map) {
    int data[] = { 1, 2, 3 };
    MutableArraySequence<int> seq(data, 3);
    auto* result = Map<int, int>(&seq, [](const int& x) { return x * 2; });
    EXPECT_EQ(result->Get(0), 2);
    EXPECT_EQ(result->Get(2), 6);
    delete result;
}

TEST(MapReduce, Where) {
    int data[] = { 1, 2, 3, 4, 5 };
    MutableArraySequence<int> seq(data, 5);
    auto* result = Where<int>(&seq, [](const int& x) { return x % 2 == 0; });
    EXPECT_EQ(result->GetLength(), 2);
    EXPECT_EQ(result->Get(0), 2);
    delete result;
}

TEST(MapReduce, Reduce) {
    int data[] = { 1, 2, 3, 4 };
    MutableArraySequence<int> seq(data, 4);
    int sum = Reduce<int, int>(&seq, [](const int& acc, const int& x) {
        return acc + x;
        }, 0);
    EXPECT_EQ(sum, 10);
}

TEST(MapReduce, MapEmptySequence) {
    MutableArraySequence<int> seq;
    auto* result = Map<int, int>(&seq, [](const int& x) { return x * 2; });
    EXPECT_EQ(result->GetLength(), 0);
    delete result;
}

TEST(MapReduce, ReduceWithInitial) {
    int data[] = { 1, 2, 3 };
    MutableArraySequence<int> seq(data, 3);
    int product = Reduce<int, int>(&seq, [](const int& acc, const int& x) {
        return acc * x;
        }, 1);
    EXPECT_EQ(product, 6);
}

// ═══════════════════════════════════════════════════════════════
//  14. Zip / Unzip
// ═══════════════════════════════════════════════════════════════

TEST(Zip, ZipTwoSequences) {
    int a[] = { 1, 2, 3 };
    int b[] = { 4, 5, 6 };
    MutableArraySequence<int> sa(a, 3), sb(b, 3);
    auto* zipped = Zip<int, int>(&sa, &sb);
    EXPECT_EQ(zipped->GetLength(), 3);
    EXPECT_EQ(zipped->Get(0).first, 1);
    EXPECT_EQ(zipped->Get(0).second, 4);
    delete zipped;
}

TEST(Zip, ZipDifferentLengthsTakesMin) {
    int a[] = { 1, 2, 3 }, b[] = { 4, 5 };
    MutableArraySequence<int> sa(a, 3), sb(b, 2);
    auto* zipped = Zip<int, int>(&sa, &sb);
    EXPECT_EQ(zipped->GetLength(), 2);
    delete zipped;
}

TEST(Zip, Unzip) {
    int a[] = { 1, 2 }, b[] = { 3, 4 };
    MutableArraySequence<int> sa(a, 2), sb(b, 2);
    auto* zipped = Zip<int, int>(&sa, &sb);
    Sequence<int>* outA, * outB;
    Unzip<int, int>(zipped, outA, outB);
    EXPECT_EQ(outA->Get(0), 1);
    EXPECT_EQ(outB->Get(1), 4);
    delete zipped; delete outA; delete outB;
}

// ═══════════════════════════════════════════════════════════════
//  15. GetMinMaxAvg / GetMedian / CountInversions
// ═══════════════════════════════════════════════════════════════

TEST(Algorithms, MinMaxAvg) {
    int data[] = { 3, 1, 4, 1, 5 };
    MutableArraySequence<int> seq(data, 5);
    auto res = GetMinMaxAvg(seq);
    EXPECT_EQ(res.min, 1);
    EXPECT_EQ(res.max, 5);
    EXPECT_DOUBLE_EQ(res.avg, 2.8);
}

TEST(Algorithms, MinMaxAvgEmptyThrows) {
    MutableArraySequence<int> seq;
    EXPECT_THROW(GetMinMaxAvg(seq), std::out_of_range);
}

TEST(Algorithms, MedianOdd) {
    int data[] = { 3, 1, 2 };
    MutableArraySequence<int> seq(data, 3);
    EXPECT_DOUBLE_EQ(GetMedian(seq), 2.0);
}

TEST(Algorithms, MedianEven) {
    int data[] = { 1, 2, 3, 4 };
    MutableArraySequence<int> seq(data, 4);
    EXPECT_DOUBLE_EQ(GetMedian(seq), 2.5);
}

TEST(Algorithms, MedianEmptyThrows) {
    MutableArraySequence<int> seq;
    EXPECT_THROW(GetMedian(seq), std::out_of_range);
}

TEST(Algorithms, CountInversionsNone) {
    int data[] = { 1, 2, 3 };
    MutableArraySequence<int> seq(data, 3);
    EXPECT_EQ(CountInversions(seq), 0);
}

TEST(Algorithms, CountInversionsAll) {
    int data[] = { 3, 2, 1 };
    MutableArraySequence<int> seq(data, 3);
    EXPECT_EQ(CountInversions(seq), 3);
}

// ═══════════════════════════════════════════════════════════════
//  16. GetPrefixes / GetSuffixes / GetMovingAverage / GetMirrorSum
// ═══════════════════════════════════════════════════════════════

TEST(Algorithms, PrefixesCount) {
    int data[] = { 1, 2, 3 };
    MutableArraySequence<int> seq(data, 3);
    auto* prefixes = GetPrefixes(seq);
    EXPECT_EQ(prefixes->GetLength(), 3);
    EXPECT_EQ(prefixes->Get(0)->GetLength(), 1);
    EXPECT_EQ(prefixes->Get(2)->GetLength(), 3);
    for (int i = 0; i < prefixes->GetLength(); ++i) delete prefixes->Get(i);
    delete prefixes;
}

TEST(Algorithms, SuffixesCount) {
    int data[] = { 1, 2, 3 };
    MutableArraySequence<int> seq(data, 3);
    auto* suffixes = GetSuffixes(seq);
    EXPECT_EQ(suffixes->GetLength(), 3);
    EXPECT_EQ(suffixes->Get(0)->GetLength(), 3);
    EXPECT_EQ(suffixes->Get(2)->GetLength(), 1);
    for (int i = 0; i < suffixes->GetLength(); ++i) delete suffixes->Get(i);
    delete suffixes;
}

TEST(Algorithms, MovingAverageBoundary) {
    int data[] = { 1, 2, 3 };
    MutableArraySequence<int> seq(data, 3);
    auto* avg = GetMovingAverage(seq);
    // первый элемент: (1+2)/2 = 1.5
    EXPECT_DOUBLE_EQ(avg->Get(0), 1.5);
    // средний: (1+2+3)/3 = 2.0
    EXPECT_DOUBLE_EQ(avg->Get(1), 2.0);
    delete avg;
}

TEST(Algorithms, MirrorSum) {
    int data[] = { 1, 2, 3 };
    MutableArraySequence<int> seq(data, 3);
    auto* mirror = GetMirrorSum(seq);
    EXPECT_EQ(mirror->Get(0), 1 + 3); // a[0]+a[2]
    EXPECT_EQ(mirror->Get(1), 2 + 2); // a[1]+a[1]
    delete mirror;
}


// ═══════════════════════════════════════════════════════════════
//  BitSequence — базовые операции
// ═══════════════════════════════════════════════════════════════

TEST(BitSequence, DefaultConstructorEmpty) {
    BitSequence bs;
    EXPECT_EQ(bs.GetLength(), 0);
}

TEST(BitSequence, ConstructorWithSize) {
    BitSequence bs(8);
    EXPECT_EQ(bs.GetLength(), 8);
    for (int i = 0; i < 8; ++i)
        EXPECT_EQ(bs.Get(i), 0); // все биты = 0
}

TEST(BitSequence, SetAndGet) {
    BitSequence bs(8);
    bs.Set(0, 1);
    bs.Set(3, 1);
    EXPECT_EQ(bs.Get(0), 1);
    EXPECT_EQ(bs.Get(1), 0);
    EXPECT_EQ(bs.Get(3), 1);
}

TEST(BitSequence, Flip) {
    BitSequence bs(4);
    bs.Set(1, 1);
    bs.Flip(1);
    EXPECT_EQ(bs.Get(1), 0);
    bs.Flip(1);
    EXPECT_EQ(bs.Get(1), 1);
}

TEST(BitSequence, CopyConstructorDeep) {
    BitSequence a(4);
    a.Set(0, 1);
    BitSequence b(a);
    b.Set(0, 0);
    EXPECT_EQ(a.Get(0), 1); // оригинал не изменился
}

TEST(BitSequence, AssignmentDeep) {
    BitSequence a(4);
    a.Set(2, 1);
    BitSequence b(4);
    b = a;
    b.Set(2, 0);
    EXPECT_EQ(a.Get(2), 1);
}

TEST(BitSequence, OperatorBracket) {
    BitSequence bs(4);
    bs.Set(2, 1);
    EXPECT_EQ(bs[2], 1);
}

TEST(BitSequence, OperatorEqual) {
    BitSequence a(4), b(4);
    a.Set(1, 1);
    b.Set(1, 1);
    EXPECT_TRUE(a == b);
    b.Set(0, 1);
    EXPECT_FALSE(a == b);
}

// ═══════════════════════════════════════════════════════════════
//  BitSequence — побитовые операции
// ═══════════════════════════════════════════════════════════════

TEST(BitSequence, AND) {
    BitSequence a(4), b(4);
    a.Set(0, 1); a.Set(1, 1);
    b.Set(1, 1); b.Set(2, 1);
    auto result = a.AND(b);
    EXPECT_EQ(result.Get(0), 0);
    EXPECT_EQ(result.Get(1), 1);
    EXPECT_EQ(result.Get(2), 0);
}

TEST(BitSequence, OR) {
    BitSequence a(4), b(4);
    a.Set(0, 1);
    b.Set(1, 1);
    auto result = a.OR(b);
    EXPECT_EQ(result.Get(0), 1);
    EXPECT_EQ(result.Get(1), 1);
    EXPECT_EQ(result.Get(2), 0);
}

TEST(BitSequence, XOR) {
    BitSequence a(4), b(4);
    a.Set(0, 1); a.Set(1, 1);
    b.Set(1, 1); b.Set(2, 1);
    auto result = a.XOR(b);
    EXPECT_EQ(result.Get(0), 1);
    EXPECT_EQ(result.Get(1), 0);
    EXPECT_EQ(result.Get(2), 1);
}

TEST(BitSequence, NOT) {
    BitSequence a(4);
    a.Set(0, 1);
    a.Set(2, 1);
    auto result = a.NOT();
    EXPECT_EQ(result.Get(0), 0);
    EXPECT_EQ(result.Get(1), 1);
    EXPECT_EQ(result.Get(2), 0);
    EXPECT_EQ(result.Get(3), 1);
}

TEST(BitSequence, OperatorAnd) {
    BitSequence a(4), b(4);
    a.Set(0, 1); b.Set(0, 1);
    auto result = a & b;
    EXPECT_EQ(result.Get(0), 1);
}

TEST(BitSequence, OperatorOr) {
    BitSequence a(4), b(4);
    a.Set(0, 1); b.Set(1, 1);
    auto result = a | b;
    EXPECT_EQ(result.Get(0), 1);
    EXPECT_EQ(result.Get(1), 1);
}

TEST(BitSequence, OperatorXor) {
    BitSequence a(4), b(4);
    a.Set(0, 1); b.Set(0, 1);
    auto result = a ^ b;
    EXPECT_EQ(result.Get(0), 0);
}

TEST(BitSequence, OperatorNot) {
    BitSequence a(4);
    a.Set(0, 1);
    auto result = ~a;
    EXPECT_EQ(result.Get(0), 0);
    EXPECT_EQ(result.Get(1), 1);
}

// ═══════════════════════════════════════════════════════════════
//  BitSequence — использование как маска
// ═══════════════════════════════════════════════════════════════

TEST(BitSequence, MaskAND) {
    // маска 1010 применяется к данным 1111
    BitSequence data(4), mask(4);
    data.Set(0, 1); data.Set(1, 1); data.Set(2, 1); data.Set(3, 1);
    mask.Set(0, 1); mask.Set(2, 1);
    auto result = data & mask;
    EXPECT_EQ(result.Get(0), 1);
    EXPECT_EQ(result.Get(1), 0);
    EXPECT_EQ(result.Get(2), 1);
    EXPECT_EQ(result.Get(3), 0);
}

TEST(BitSequence, NOTPreservesLength) {
    BitSequence a(9); // нестандартная длина — не кратна 8
    auto result = a.NOT();
    EXPECT_EQ(result.GetLength(), 9);
    // лишние биты в последнем байте не должны проникнуть
    for (int i = 0; i < 9; ++i)
        EXPECT_EQ(result.Get(i), 1);
}

// ═══════════════════════════════════════════════════════════════
//  BitSequence — пограничные ситуации
// ═══════════════════════════════════════════════════════════════

TEST(BitSequence, SingleBit) {
    BitSequence bs(1);
    bs.Set(0, 1);
    EXPECT_EQ(bs.Get(0), 1);
    bs.Flip(0);
    EXPECT_EQ(bs.Get(0), 0);
}

TEST(BitSequence, LengthNotMultipleOf8) {
    BitSequence bs(13);
    EXPECT_EQ(bs.GetLength(), 13);
    bs.Set(12, 1);
    EXPECT_EQ(bs.Get(12), 1);
}

// ═══════════════════════════════════════════════════════════════
//  BitSequence — некорректные ситуации
// ═══════════════════════════════════════════════════════════════

TEST(BitSequence, NegativeSizeThrows) {
    EXPECT_THROW(BitSequence(-1), std::invalid_argument);
}

TEST(BitSequence, GetOutOfRangeThrows) {
    BitSequence bs(4);
    EXPECT_THROW(bs.Get(-1), std::out_of_range);
    EXPECT_THROW(bs.Get(4), std::out_of_range);
}

TEST(BitSequence, SetOutOfRangeThrows) {
    BitSequence bs(4);
    EXPECT_THROW(bs.Set(-1, 1), std::out_of_range);
    EXPECT_THROW(bs.Set(4, 1), std::out_of_range);
}

TEST(BitSequence, SetInvalidBitValueThrows) {
    BitSequence bs(4);
    EXPECT_THROW(bs.Set(0, 2), std::invalid_argument);
    EXPECT_THROW(bs.Set(0, -1), std::invalid_argument);
}

TEST(BitSequence, FlipOutOfRangeThrows) {
    BitSequence bs(4);
    EXPECT_THROW(bs.Flip(10), std::out_of_range);
}

TEST(BitSequence, ANDDifferentLengthThrows) {
    BitSequence a(4), b(8);
    EXPECT_THROW(a.AND(b), std::invalid_argument);
}

TEST(BitSequence, ORDifferentLengthThrows) {
    BitSequence a(4), b(8);
    EXPECT_THROW(a.OR(b), std::invalid_argument);
}

TEST(BitSequence, XORDifferentLengthThrows) {
    BitSequence a(4), b(8);
    EXPECT_THROW(a.XOR(b), std::invalid_argument);
}

// ═══════════════════════════════════════════════════════════════
//  AdaptiveSequence — базовые операции
// ═══════════════════════════════════════════════════════════════

TEST(AdaptiveSequence, DefaultStartsAsArray) {
    AdaptiveSequence<int> seq;
    EXPECT_TRUE(seq.IsArray());
    EXPECT_EQ(seq.GetLength(), 0);
}

TEST(AdaptiveSequence, AppendAndGet) {
    AdaptiveSequence<int> seq;
    seq.Append(1);
    seq.Append(2);
    seq.Append(3);
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(2), 3);
}

TEST(AdaptiveSequence, Prepend) {
    AdaptiveSequence<int> seq;
    seq.Append(2);
    seq.Prepend(1);
    EXPECT_EQ(seq.GetFirst(), 1);
}

TEST(AdaptiveSequence, InsertAt) {
    AdaptiveSequence<int> seq;
    seq.Append(1);
    seq.Append(3);
    seq.InsertAt(2, 1);
    EXPECT_EQ(seq.Get(1), 2);
    EXPECT_EQ(seq.GetLength(), 3);
}

TEST(AdaptiveSequence, GetFirstAndLast) {
    AdaptiveSequence<int> seq;
    seq.Append(10);
    seq.Append(20);
    EXPECT_EQ(seq.GetFirst(), 10);
    EXPECT_EQ(seq.GetLast(), 20);
}

TEST(AdaptiveSequence, OperatorBracket) {
    AdaptiveSequence<int> seq;
    seq.Append(5);
    seq.Append(15);
    EXPECT_EQ(seq[0], 5);
    EXPECT_EQ(seq[1], 15);
}

// ═══════════════════════════════════════════════════════════════
//  AdaptiveSequence — переключение структуры
// ═══════════════════════════════════════════════════════════════

TEST(AdaptiveSequence, SwitchToListAfterManyInserts) {
    AdaptiveSequence<int> seq;
    for (int i = 0; i < 5; ++i)
        seq.Append(i);
    // много InsertAt → должен переключиться на список
    for (int i = 0; i < 15; ++i)
        seq.InsertAt(99, 0);
    EXPECT_FALSE(seq.IsArray());
}

TEST(AdaptiveSequence, SwitchToArrayAfterManyGets) {
    AdaptiveSequence<int> seq;
    for (int i = 0; i < 5; ++i)
        seq.Append(i);
    // принудительно переключим на список
    for (int i = 0; i < 15; ++i)
        seq.InsertAt(0, 0);
    EXPECT_FALSE(seq.IsArray());

    // теперь много Get → должен переключиться обратно на массив
    for (int i = 0; i < 25; ++i)
        seq.Get(0);
    EXPECT_TRUE(seq.IsArray());
}

TEST(AdaptiveSequence, DataPreservedAfterSwitch) {
    AdaptiveSequence<int> seq;
    seq.Append(1); seq.Append(2); seq.Append(3);
    for (int i = 0; i < 15; ++i)
        seq.InsertAt(0, 0);
    // после переключения данные сохраняются
    EXPECT_EQ(seq.Get(seq.GetLength() - 3), 1);
    EXPECT_EQ(seq.Get(seq.GetLength() - 2), 2);
    EXPECT_EQ(seq.Get(seq.GetLength() - 1), 3);
}

// ═══════════════════════════════════════════════════════════════
//  AdaptiveSequence — пограничные ситуации
// ═══════════════════════════════════════════════════════════════

TEST(AdaptiveSequence, SingleElement) {
    AdaptiveSequence<int> seq;
    seq.Append(42);
    EXPECT_EQ(seq.GetFirst(), 42);
    EXPECT_EQ(seq.GetLast(), 42);
    EXPECT_EQ(seq.GetLength(), 1);
}

TEST(AdaptiveSequence, SubsequenceWorks) {
    AdaptiveSequence<int> seq;
    for (int i = 0; i < 5; ++i) seq.Append(i);
    auto* sub = seq.GetSubsequence(1, 3);
    EXPECT_EQ(sub->GetLength(), 3);
    EXPECT_EQ(sub->Get(0), 1);
    delete sub;
}

// ═══════════════════════════════════════════════════════════════
//  AdaptiveSequence — некорректные ситуации
// ═══════════════════════════════════════════════════════════════

TEST(AdaptiveSequence, GetFirstEmptyThrows) {
    AdaptiveSequence<int> seq;
    EXPECT_THROW(seq.GetFirst(), std::out_of_range);
}

TEST(AdaptiveSequence, GetLastEmptyThrows) {
    AdaptiveSequence<int> seq;
    EXPECT_THROW(seq.GetLast(), std::out_of_range);
}

TEST(AdaptiveSequence, GetOutOfRangeThrows) {
    AdaptiveSequence<int> seq;
    seq.Append(1);
    EXPECT_THROW(seq.Get(5), std::out_of_range);
    EXPECT_THROW(seq.Get(-1), std::out_of_range);
}




// ═══════════════════════════════════════════════════════════════
//  MutableListSequence — базовые операции
// ═══════════════════════════════════════════════════════════════

TEST(MutableListSequence, DefaultConstructorEmpty) {
    MutableListSequence<int> seq;
    EXPECT_EQ(seq.GetLength(), 0);
}

TEST(MutableListSequence, ConstructorFromItems) {
    int data[] = { 1, 2, 3 };
    MutableListSequence<int> seq(data, 3);
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.GetFirst(), 1);
    EXPECT_EQ(seq.GetLast(), 3);
}

TEST(MutableListSequence, ConstructorFromLinkedList) {
    int data[] = { 10, 20, 30 };
    LinkedList<int> list(data, 3);
    MutableListSequence<int> seq(list);
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.Get(1), 20);
}

TEST(MutableListSequence, CopyConstructorDeep) {
    int data[] = { 1, 2, 3 };
    MutableListSequence<int> a(data, 3);
    MutableListSequence<int> b(a);
    b.Append(99);
    EXPECT_EQ(a.GetLength(), 3); // оригинал не изменился
}

TEST(MutableListSequence, Append) {
    MutableListSequence<int> seq;
    seq.Append(1);
    seq.Append(2);
    EXPECT_EQ(seq.GetLength(), 2);
    EXPECT_EQ(seq.GetLast(), 2);
}

TEST(MutableListSequence, Prepend) {
    MutableListSequence<int> seq;
    seq.Append(2);
    seq.Prepend(1);
    EXPECT_EQ(seq.GetFirst(), 1);
    EXPECT_EQ(seq.GetLength(), 2);
}

TEST(MutableListSequence, InsertAt) {
    int data[] = { 1, 3 };
    MutableListSequence<int> seq(data, 2);
    seq.InsertAt(2, 1);
    EXPECT_EQ(seq.Get(1), 2);
    EXPECT_EQ(seq.GetLength(), 3);
}

TEST(MutableListSequence, InsertAtBegin) {
    int data[] = { 2, 3 };
    MutableListSequence<int> seq(data, 2);
    seq.InsertAt(1, 0);
    EXPECT_EQ(seq.GetFirst(), 1);
}

TEST(MutableListSequence, InsertAtEnd) {
    int data[] = { 1, 2 };
    MutableListSequence<int> seq(data, 2);
    seq.InsertAt(3, 2);
    EXPECT_EQ(seq.GetLast(), 3);
}

TEST(MutableListSequence, GetSubsequence) {
    int data[] = { 10, 20, 30, 40 };
    MutableListSequence<int> seq(data, 4);
    auto* sub = seq.GetSubsequence(1, 2);
    EXPECT_EQ(sub->GetLength(), 2);
    EXPECT_EQ(sub->Get(0), 20);
    EXPECT_EQ(sub->Get(1), 30);
    delete sub;
}

TEST(MutableListSequence, SubsequenceFullRange) {
    int data[] = { 1, 2, 3 };
    MutableListSequence<int> seq(data, 3);
    auto* sub = seq.GetSubsequence(0, 2);
    EXPECT_EQ(sub->GetLength(), 3);
    delete sub;
}

TEST(MutableListSequence, SubsequenceSingleElement) {
    int data[] = { 1, 2, 3 };
    MutableListSequence<int> seq(data, 3);
    auto* sub = seq.GetSubsequence(2, 2);
    EXPECT_EQ(sub->GetLength(), 1);
    EXPECT_EQ(sub->Get(0), 3);
    delete sub;
}

TEST(MutableListSequence, Concat) {
    int a[] = { 1, 2 }, b[] = { 3, 4 };
    MutableListSequence<int> sa(a, 2), sb(b, 2);
    auto* result = sa.Concat(sb);
    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(2), 3);
    delete result;
}

TEST(MutableListSequence, OperatorPlus) {
    int a[] = { 1, 2 }, b[] = { 3, 4 };
    MutableListSequence<int> sa(a, 2), sb(b, 2);
    auto* result = sa + sb;
    EXPECT_EQ(result->GetLength(), 4);
    delete result;
}

TEST(MutableListSequence, OperatorBracket) {
    int data[] = { 5, 10, 15 };
    MutableListSequence<int> seq(data, 3);
    EXPECT_EQ(seq[0], 5);
    EXPECT_EQ(seq[2], 15);
}

TEST(MutableListSequence, IsMutableInPlace) {
    int data[] = { 1, 2, 3 };
    MutableListSequence<int> seq(data, 3);
    Sequence<int>* result = seq.Append(4);
    EXPECT_EQ(result, &seq); // mutable — тот же объект
    EXPECT_EQ(seq.GetLength(), 4);
}

// ═══════════════════════════════════════════════════════════════
//  MutableListSequence — некорректные ситуации
// ═══════════════════════════════════════════════════════════════

TEST(MutableListSequence, GetFirstEmptyThrows) {
    MutableListSequence<int> seq;
    EXPECT_THROW(seq.GetFirst(), std::out_of_range);
}

TEST(MutableListSequence, GetLastEmptyThrows) {
    MutableListSequence<int> seq;
    EXPECT_THROW(seq.GetLast(), std::out_of_range);
}

TEST(MutableListSequence, GetOutOfRangeThrows) {
    int data[] = { 1, 2 };
    MutableListSequence<int> seq(data, 2);
    EXPECT_THROW(seq.Get(-1), std::out_of_range);
    EXPECT_THROW(seq.Get(5), std::out_of_range);
}

TEST(MutableListSequence, InsertAtOutOfRangeThrows) {
    MutableListSequence<int> seq;
    EXPECT_THROW(seq.InsertAt(1, 5), std::out_of_range);
}

TEST(MutableListSequence, SubsequenceInvalidRangeThrows) {
    int data[] = { 1, 2, 3 };
    MutableListSequence<int> seq(data, 3);
    EXPECT_THROW(seq.GetSubsequence(2, 1), std::out_of_range);
    EXPECT_THROW(seq.GetSubsequence(-1, 1), std::out_of_range);
    EXPECT_THROW(seq.GetSubsequence(0, 5), std::out_of_range);
}

TEST(MutableListSequence, NullptrConstructorThrows) {
    EXPECT_THROW(MutableListSequence<int>(nullptr, 3), std::invalid_argument);
}

TEST(MutableListSequence, NegativeCountThrows) {
    EXPECT_THROW(MutableListSequence<int>(nullptr, -1), std::invalid_argument);
}

// ═══════════════════════════════════════════════════════════════
//  ImmutableListSequence — неизменяемость
// ═══════════════════════════════════════════════════════════════

TEST(ImmutableListSequence, AppendReturnsNewObject) {
    int data[] = { 1, 2, 3 };
    ImmutableListSequence<int> seq(data, 3);
    Sequence<int>* result = seq.Append(4);
    EXPECT_NE(result, &seq);
    EXPECT_EQ(seq.GetLength(), 3);    // оригинал не изменился
    EXPECT_EQ(result->GetLength(), 4);
    delete result;
}

TEST(ImmutableListSequence, PrependReturnsNewObject) {
    int data[] = { 2, 3 };
    ImmutableListSequence<int> seq(data, 2);
    Sequence<int>* result = seq.Prepend(1);
    EXPECT_NE(result, &seq);
    EXPECT_EQ(seq.GetFirst(), 2);
    EXPECT_EQ(result->GetFirst(), 1);
    delete result;
}

TEST(ImmutableListSequence, InsertAtReturnsNewObject) {
    int data[] = { 1, 3 };
    ImmutableListSequence<int> seq(data, 2);
    Sequence<int>* result = seq.InsertAt(2, 1);
    EXPECT_NE(result, &seq);
    EXPECT_EQ(seq.GetLength(), 2);
    EXPECT_EQ(result->GetLength(), 3);
    delete result;
}

TEST(ImmutableListSequence, ChainedOperationsOriginalUnchanged) {
    int data[] = { 1, 2, 3 };
    ImmutableListSequence<int> seq(data, 3);
    auto* r1 = seq.Append(4);
    auto* r2 = seq.Prepend(0);
    EXPECT_EQ(seq.GetLength(), 3); // оригинал всегда 3
    EXPECT_EQ(r1->GetLength(), 4);
    EXPECT_EQ(r2->GetLength(), 4);
    delete r1;
    delete r2;
}

// ═══════════════════════════════════════════════════════════════
//  ListSequence Builder
// ═══════════════════════════════════════════════════════════════

TEST(ListSequenceBuilder, AppendAndBuild) {
    auto* seq = MutableListSequence<int>::Builder()
        .Append(1).Append(2).Append(3)
        .Build();
    EXPECT_EQ(seq->GetLength(), 3);
    EXPECT_EQ(seq->Get(0), 1);
    EXPECT_EQ(seq->Get(2), 3);
    delete seq;
}

TEST(ListSequenceBuilder, AppendAll) {
    int data[] = { 10, 20, 30 };
    auto* seq = MutableListSequence<int>::Builder()
        .AppendAll(data, 3)
        .Build();
    EXPECT_EQ(seq->GetLength(), 3);
    EXPECT_EQ(seq->Get(1), 20);
    delete seq;
}

TEST(ListSequenceBuilder, AppendSequence) {
    int data[] = { 1, 2, 3 };
    MutableListSequence<int> source(data, 3);
    auto* seq = MutableListSequence<int>::Builder()
        .AppendSequence(source)
        .Build();
    EXPECT_EQ(seq->GetLength(), 3);
    EXPECT_EQ(seq->Get(2), 3);
    delete seq;
}

TEST(ListSequenceBuilder, NullptrThrows) {
    MutableListSequence<int>::Builder b;
    EXPECT_THROW(b.AppendAll(nullptr, 3), std::invalid_argument);
}

TEST(ListSequenceBuilder, BuildResetsBuilder) {
    auto builder = MutableListSequence<int>::Builder();
    builder.Append(1).Append(2);
    auto* first = builder.Build();
    builder.Append(99);
    auto* second = builder.Build();
    EXPECT_EQ(first->GetLength(), 2);
    EXPECT_EQ(second->GetLength(), 1); // после Build билдер сбросился
    delete first;
    delete second;
}

// ═══════════════════════════════════════════════════════════════
//  ListSequence Enumerator
// ═══════════════════════════════════════════════════════════════

TEST(ListSequenceEnumerator, IteratesAllElements) {
    int data[] = { 1, 2, 3 };
    MutableListSequence<int> seq(data, 3);
    auto* en = seq.get_enumerator();
    int sum = 0;
    while (en->move_next())
        sum += en->get_current();
    EXPECT_EQ(sum, 6);
    delete en;
}

TEST(ListSequenceEnumerator, ResetWorks) {
    int data[] = { 5, 10 };
    MutableListSequence<int> seq(data, 2);
    auto* en = seq.get_enumerator();
    en->move_next();
    en->reset();
    en->move_next();
    EXPECT_EQ(en->get_current(), 5);
    delete en;
}

TEST(ListSequenceEnumerator, GetCurrentBeforeMoveNextThrows) {
    int data[] = { 1 };
    MutableListSequence<int> seq(data, 1);
    auto* en = seq.get_enumerator();
    EXPECT_THROW(en->get_current(), std::out_of_range);
    delete en;
}

TEST(ListSequenceEnumerator, EmptySequenceNoMove) {
    MutableListSequence<int> seq;
    auto* en = seq.get_enumerator();
    EXPECT_FALSE(en->move_next());
    delete en;
}

