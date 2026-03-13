#include <iostream>
#include "ArraySequence.h"
#include "ListSequence.h"
#include "MapReduce.h"
#include <clocale>
#include "Pair.h"
#include "BitSequence.h"
#include "Adaptive_Sequence.h"

// Вспомогательная функция для вывода последовательности
void Print(const Sequence<int>* seq) {
    std::cout << "[";
    for (int i = 0; i < seq->GetLength(); ++i) {
        std::cout << seq->Get(i);
        if (i < seq->GetLength() - 1)
            std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}

// Вспомогательная функция для вывода последовательности парами
void PrintPairs(const Sequence<Pair<int, int>>* seq) {
    std::cout << "[";
    for (int i = 0; i < seq->GetLength(); ++i) {
        std::cout << "(" << seq->Get(i).first << "," << seq->Get(i).second << ")";
        if (i < seq->GetLength() - 1)
            std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}

// Функции для MapReduce
int Double(const int& x) { return x * 2; }
bool IsEven(const int& x) { return x % 2 == 0; }
int Sum(const int& acc, const int& x) { return acc + x; }

int main() {
    setlocale(LC_ALL, "Russian");
    std::cout << "=== Тест ArraySequence ===" << std::endl;

    int data[] = { 1, 2, 3, 4, 5 };
    MutableArraySequence<int> seq(data, 5);

    // Базовые операции
    std::cout << "Исходная: ";
    Print(&seq);

    std::cout << "GetFirst: " << seq.GetFirst() << std::endl;  // 1
    std::cout << "GetLast:  " << seq.GetLast() << std::endl;  // 5
    std::cout << "Get(2):   " << seq.Get(2) << std::endl;  // 3
    std::cout << "Length:   " << seq.GetLength() << std::endl; // 5

    // Append
    seq.Append(6);
    std::cout << "После Append(6): ";
    Print(&seq);  // [1,2,3,4,5,6]

    // Prepend
    seq.Prepend(0);
    std::cout << "После Prepend(0): ";
    Print(&seq);  // [0,1,2,3,4,5,6]

    // InsertAt
    seq.InsertAt(99, 3);
    std::cout << "После InsertAt(99, 3): ";
    Print(&seq);  // [0,1,2,99,3,4,5,6]

    // GetSubsequence
    Sequence<int>* sub = seq.GetSubsequence(1, 3);
    std::cout << "GetSubsequence(1,3): ";
    Print(sub);   // [1,2,99]
    delete sub;

    // Concat
    int data2[] = { 10, 20 };
    MutableArraySequence<int> seq2(data2, 2);
    Sequence<int>* concat = seq.Concat(seq2);
    std::cout << "После Concat([10,20]): ";
    Print(concat);
    delete concat;

    std::cout << std::endl;
    std::cout << "=== Тест MapReduce ===" << std::endl;

    int data3[] = { 1, 2, 3, 4, 5 };
    MutableArraySequence<int> seq3(data3, 5);

    // Map
    Sequence<int>* doubled = Map<int, int>(&seq3, Double);
    std::cout << "Map (x*2):      ";
    Print(doubled);   // [2,4,6,8,10]
    delete doubled;

    // Where
    Sequence<int>* evens = Where<int>(&seq3, IsEven);
    std::cout << "Where (чётные): ";
    Print(evens);     // [2,4]
    delete evens;

    // Reduce
    int sum = Reduce<int, int>(&seq3, Sum, 0);
    std::cout << "Reduce (сумма): " << sum << std::endl;  // 15

    std::cout << std::endl;
    std::cout << "=== Тест ListSequence ===" << std::endl;

    int data4[] = { 5, 10, 15 };
    MutableListSequence<int> listSeq(data4, 3);

    std::cout << "Исходная: ";
    Print(&listSeq);           // [5,10,15]

    listSeq.Append(20);
    std::cout << "После Append(20): ";
    Print(&listSeq);           // [5,10,15,20]

    std::cout << "GetFirst: " << listSeq.GetFirst() << std::endl;  // 5
    std::cout << "GetLast:  " << listSeq.GetLast() << std::endl;  // 20

    std::cout << std::endl;

    std::cout << "=== Тест Zip/Unzip ===" << std::endl;

    int dataA[] = { 1, 2, 3 };
    int dataB[] = { 10, 20, 30 };
    MutableArraySequence<int> seqA(dataA, 3);
    MutableArraySequence<int> seqB(dataB, 3);

    // Zip
    Sequence<Pair<int, int>>* zipped = Zip<int, int>(&seqA, &seqB);
    std::cout << "Zip: ";
    PrintPairs(zipped);  // [(1,10), (2,20), (3,30)]

    // Unzip
    Sequence<int>* unzipFirst = nullptr;
    Sequence<int>* unzipSecond = nullptr;
    Unzip<int, int>(zipped, unzipFirst, unzipSecond);

    std::cout << "Unzip первая:  ";
    Print(unzipFirst);   // [1, 2, 3]
    std::cout << "Unzip вторая: ";
    Print(unzipSecond);  // [10, 20, 30]

    delete zipped;
    delete unzipFirst;
    delete unzipSecond;


    std::cout << "=== Тест BitSequence ===" << std::endl;

    BitSequence a(8);  // 8 бит, все 0
    BitSequence b(8);

    // a = 10110100
    a.Set(1, 1);
    a.Set(2, 1);
    a.Set(4, 1);
    a.Set(6, 1);

    // b = 11001010
    b.Set(1, 1);
    b.Set(3, 1);
    b.Set(6, 1);
    b.Set(7, 1);

    std::cout << "A:       "; a.Print();
    std::cout << "B:       "; b.Print();

    BitSequence andResult = a.AND(b);
    std::cout << "A AND B: "; andResult.Print();

    BitSequence orResult = a.OR(b);
    std::cout << "A OR  B: "; orResult.Print();

    BitSequence xorResult = a.XOR(b);
    std::cout << "A XOR B: "; xorResult.Print();

    BitSequence notResult = a.NOT();
    std::cout << "NOT A:   "; notResult.Print();




    std::cout << "=== Тест операторов ===" << std::endl;

    int dataOp[] = { 1, 2, 3, 4, 5 };
    MutableArraySequence<int> seqOp(dataOp, 5);

    std::cout << "seqOp[0]: " << seqOp[0] << std::endl;  // 1
    std::cout << "seqOp[4]: " << seqOp[4] << std::endl;  // 5

    int dataOp2[] = { 6, 7 };
    MutableArraySequence<int> seqOp2(dataOp2, 2);

    Sequence<int>* concatOp = seqOp + seqOp2;
    std::cout << "seqOp + seqOp2: ";
    Print(concatOp);  // [1, 2, 3, 4, 5, 6, 7]
    delete concatOp;

    std::cout << "=== Тест BitSequence операторов ===" << std::endl;

    BitSequence bitA(4), bitB(4);
    // bitA = [1, 0, 1, 0]
    bitA.Set(0, 1);
    bitA.Set(2, 1);

    // bitB = [0, 1, 1, 0]
    bitB.Set(1, 1);
    bitB.Set(2, 1);

    std::cout << "bitA:       "; bitA.Print();         // [1, 0, 1, 0]
    std::cout << "bitB:       "; bitB.Print();         // [0, 1, 1, 0]
    std::cout << "bitA & bitB: "; (bitA& bitB).Print(); // [0, 0, 1, 0]
    std::cout << "bitA | bitB: "; (bitA | bitB).Print(); // [1, 1, 1, 0]
    std::cout << "bitA ^ bitB: "; (bitA^ bitB).Print(); // [1, 1, 0, 0]
    std::cout << "~bitA:       "; (~bitA).Print();        // [0, 1, 0, 1]






    MutableArraySequence<int> seq1;
    seq1.Append(10);
    seq1.Append(20);
    seq1.Append(30);
    seq1.Append(40);

    // Тестируем итератор
    std::cout << "Iterating via IEnumerator: ";
    IEnumerator<int>* en = seq1.get_enumerator();
    while (en->move_next()) {
        std::cout << en->get_current() << " ";
    }
    delete en;
    std::cout << std::endl;

    // Тест reset
    std::cout << "After reset: ";
    en = seq1.get_enumerator();
    en->move_next();
    en->move_next(); // на 20
    std::cout << "Current: " << en->get_current() << std::endl; // 20
    en->reset();
    en->move_next(); // снова на 10
    std::cout << "After reset current: " << en->get_current() << std::endl; // 10
    delete en;




    AdaptiveSequence<int> seq4;
    seq4.Append(1);
    seq4.Append(2);
    seq4.Append(3);

    std::cout << "Initially array: " << seq4.IsArray() << std::endl; // 1

    // Много вставок — должен переключиться на List
    for (int i = 0; i < 15; i++)
        seq4.Prepend(i);

    std::cout << "After many Prepend, is array: " << seq4.IsArray() << std::endl; // 0

    // Много чтений — должен переключиться обратно на Array
    for (int i = 0; i < 25; i++)
        seq4.Get(0);

    std::cout << "After many Get, is array: " << seq4.IsArray() << std::endl; // 1



    // ArraySequence Builder
    MutableArraySequence<int>* seq5 = MutableArraySequence<int>::Builder()
        .Append(1)
        .Append(2)
        .Append(3)
        .Build();
    std::cout << "ArraySequence Builder: ";
    for (int i = 0; i < seq5->GetLength(); i++)
        std::cout << seq5->Get(i) << " ";
    std::cout << std::endl; // 1 2 3

    // ListSequence Builder
    int arr[] = { 10, 20, 30 };
    MutableListSequence<int>* seq6 = MutableListSequence<int>::Builder()
        .AppendAll(arr, 3)
        .Append(40)
        .Build();
    std::cout << "ListSequence Builder: ";
    for (int i = 0; i < seq6->GetLength(); i++)
        std::cout << seq6->Get(i) << " ";
    std::cout << std::endl; // 10 20 30 40

    delete seq6;
    delete seq5;



    int data5[] = { 3, 1, 4, 1, 5, 9, 2, 6 };
    MutableArraySequence<int> testSeq(data5, 8);

    // П-1
    auto stats = GetMinMaxAvg(testSeq);
    std::cout << "Min=" << stats.min << " Max=" << stats.max
        << " Avg=" << stats.avg << std::endl;

    // П-2
    std::cout << "Median=" << GetMedian(testSeq) << std::endl;

    // П-3
    std::cout << "Inversions=" << CountInversions(testSeq) << std::endl;

    // П-5 префиксы
    auto* prefixes = GetPrefixes(testSeq);
    std::cout << "Prefixes:" << std::endl;
    for (int i = 0; i < prefixes->GetLength(); i++) {
        auto* prefix = prefixes->Get(i);
        std::cout << "  [";
        for (int j = 0; j < prefix->GetLength(); j++) {
            std::cout << prefix->Get(j);
            if (j < prefix->GetLength() - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }

    // П-6
    auto* movingAvg = GetMovingAverage(testSeq);
    std::cout << "Moving avg: ";
    for (int i = 0; i < movingAvg->GetLength(); i++)
        std::cout << movingAvg->Get(i) << " ";
    std::cout << std::endl;

    // П-7
    auto* sqrtVar = GetSqrtVariance(testSeq);
    std::cout << "Sqrt variance: ";
    for (int i = 0; i < sqrtVar->GetLength(); i++)
        std::cout << sqrtVar->Get(i) << " ";
    std::cout << std::endl;

    // П-8
    auto* mirrorSum = GetMirrorSum(testSeq);
    std::cout << "Mirror sum: ";
    for (int i = 0; i < mirrorSum->GetLength(); i++)
        std::cout << mirrorSum->Get(i) << " ";
    std::cout << std::endl;

    delete movingAvg;
    delete sqrtVar;
    delete mirrorSum;

    std::cout << "=== Все тесты пройдены ===" << std::endl;

    return 0;
}
