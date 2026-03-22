#include <iostream>
#include <stdexcept>
#include <limits>
#include <windows.h>

#include "ArraySequence.h"
#include "ListSequence.h"
#include "BitSequence.h"
#include "AdaptiveSequence.h"
#include "MapReduce.h"


// Безопасный ввод целого числа с повтором
int readInt(const std::string& prompt) {
    int val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val) {
            return val;
        }
        std::cout << " Invalid input. Please enter an integer.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// Ввод целого числа в диапазоне [a, b] включительно
int readIntRange(const std::string& prompt, int a, int b) {
    while (true) {
        int val = readInt(prompt);
        if (val >= a && val <= b)
            return val;
        std::cout << " Value must be between " << a << " and " << b << ".\n";
    }
}


void PrintSequence(const Sequence<int>* seq) {
    std::cout << "[";
    auto* en = seq->get_enumerator();
    bool first = true;
    while (en->move_next()) {
        if (!first) std::cout << ", ";
        std::cout << en->get_current();
        first = false;
    }
    delete en;
    std::cout << "]" << std::endl;
}


Sequence<int>* CreateSequence() {
    std::cout << "\n=== Select Sequence Type ===" << std::endl;
    std::cout << "1. MutableArraySequence<int>" << std::endl;
    std::cout << "2. MutableListSequence<int>" << std::endl;
    std::cout << "3. ImmutableArraySequence<int>" << std::endl;
    std::cout << "4. ImmutableListSequence<int>" << std::endl;
    std::cout << "5. AdaptiveSequence<int>" << std::endl;

    int type = readIntRange("Choice (1-5): ", 1, 5);

    int n = -1;
    while (n < 0) {
        n = readInt("Enter number of elements: ");
        if (n < 0) std::cout << " Number of elements cannot be negative.\n";
    }

    int* data = new int[n];
    for (int i = 0; i < n; ++i)
        data[i] = readInt("  [" + std::to_string(i) + "]: ");

    Sequence<int>* seq = nullptr;
    switch (type) {
    case 1: seq = new MutableArraySequence<int>(data, n); break;
    case 2: seq = new MutableListSequence<int>(data, n); break;
    case 3: seq = new ImmutableArraySequence<int>(data, n); break;
    case 4: seq = new ImmutableListSequence<int>(data, n); break;
    case 5:
        seq = new AdaptiveSequence<int>();
        for (int i = 0; i < n; ++i) seq->Append(data[i]);
        break;
    }
    delete[] data;
    return seq;
}

void SequenceMenu(Sequence<int>*& seq) {
    int cmd = -1;
    while (cmd != 0) {
        std::cout << "\n=== Sequence Operations ===" << std::endl;
        std::cout << "Current: "; PrintSequence(seq);
        std::cout << "1.  Get element by index\n";
        std::cout << "2.  Append\n";
        std::cout << "3.  Prepend\n";
        std::cout << "4.  InsertAt\n";
        std::cout << "5.  GetSubsequence\n";
        std::cout << "6.  Concat with new sequence\n";
        std::cout << "7.  Map (x * 2)\n";
        std::cout << "8.  Where (even only)\n";
        std::cout << "9.  Reduce (sum)\n";
        std::cout << "10. Zip with new sequence\n";
        std::cout << "11. GetFirst / GetLast\n";
        std::cout << "12. GetLength\n";
        std::cout << "0.  Back\n";

        cmd = readIntRange("Choice (0-12): ", 0, 12);

        try {
            if (cmd == 1) {
                int i = readInt("Index: ");
                std::cout << "Value: " << seq->Get(i) << std::endl;

            }
            else if (cmd == 2) {
                int v = readInt("Value: ");
                Sequence<int>* result = seq->Append(v);
                if (result != seq) { delete seq; seq = result; }
                std::cout << "Done." << std::endl;

            }
            else if (cmd == 3) {
                int v = readInt("Value: ");
                Sequence<int>* result = seq->Prepend(v);
                if (result != seq) { delete seq; seq = result; }
                std::cout << "Done." << std::endl;

            }
            else if (cmd == 4) {
                int v = readInt("Value: ");
                int i = readInt("Index: ");
                Sequence<int>* result = seq->InsertAt(v, i);
                if (result != seq) { delete seq; seq = result; }
                std::cout << "Done." << std::endl;

            }
            else if (cmd == 5) {
                int s = readInt("Start index: ");
                int e = readInt("End index:   ");
                Sequence<int>* sub = seq->GetSubsequence(s, e);
                std::cout << "Subsequence: "; PrintSequence(sub);
                delete sub;

            }
            else if (cmd == 6) {
                Sequence<int>* other = CreateSequence();
                Sequence<int>* result = seq->Concat(*other);
                std::cout << "Result: "; PrintSequence(result);
                delete other; delete result;

            }
            else if (cmd == 7) {
                Sequence<int>* result = Map<int, int>(seq,
                    [](const int& x) { return x * 2; });
                std::cout << "Map (x*2): "; PrintSequence(result);
                delete result;

            }
            else if (cmd == 8) {
                Sequence<int>* result = Where<int>(seq,
                    [](const int& x) { return x % 2 == 0; });
                std::cout << "Where (even): "; PrintSequence(result);
                delete result;

            }
            else if (cmd == 9) {
                int sum = Reduce<int, int>(seq,
                    [](const int& acc, const int& x) { return acc + x; }, 0);
                std::cout << "Sum: " << sum << std::endl;

            }
            else if (cmd == 10) {
                Sequence<int>* other = CreateSequence();
                Sequence<Pair<int, int>>* zipped = Zip<int, int>(seq, other);
                std::cout << "Zip result:" << std::endl;
                for (int i = 0; i < zipped->GetLength(); ++i)
                    std::cout << "  (" << zipped->Get(i).first
                    << ", " << zipped->Get(i).second << ")\n";
                delete other; delete zipped;

            }
            else if (cmd == 11) {
                std::cout << "First: " << seq->GetFirst() << std::endl;
                std::cout << "Last:  " << seq->GetLast() << std::endl;

            }
            else if (cmd == 12) {
                std::cout << "Length: " << seq->GetLength() << std::endl;
            }

        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }
}

void BitSequenceMenu() {
    int n = -1;
    while (n <= 0) {
        n = readInt("Enter number of bits: ");
        if (n <= 0) std::cout << " Must be at least 1.\n";
    }
    BitSequence bs(n);

    int cmd = -1;
    while (cmd != 0) {
        std::cout << "\n=== BitSequence ===" << std::endl;
        std::cout << "Current: "; bs.Print();
        std::cout << "1. Set bit\n";
        std::cout << "2. Get bit\n";
        std::cout << "3. Flip bit\n";
        std::cout << "4. AND with new\n";
        std::cout << "5. OR with new\n";
        std::cout << "6. XOR with new\n";
        std::cout << "7. NOT\n";
        std::cout << "0. Back\n";

        cmd = readIntRange("Choice (0-7): ", 0, 7);

        try {
            if (cmd == 1) {
                int i = readInt("Index: ");
                int b = readIntRange("Bit (0/1): ", 0, 1);
                bs.Set(i, b);

            }
            else if (cmd == 2) {
                int i = readInt("Index: ");
                std::cout << "Bit[" << i << "] = " << bs.Get(i) << std::endl;

            }
            else if (cmd == 3) {
                int i = readInt("Index: ");
                bs.Flip(i);

            }
            else if (cmd == 4) {
                BitSequence other(n);
                std::cout << "Enter " << n << " bits:\n";
                for (int i = 0; i < n; ++i) {
                    int b = readIntRange("  [" + std::to_string(i) + "] (0/1): ", 0, 1);
                    other.Set(i, b);
                }
                BitSequence result(n);
                bs.AND(other, result);
                std::cout << "AND: "; result.Print();

            }
            else if (cmd == 5) {
                BitSequence other(n);
                std::cout << "Enter " << n << " bits:\n";
                for (int i = 0; i < n; ++i) {
                    int b = readIntRange("  [" + std::to_string(i) + "] (0/1): ", 0, 1);
                    other.Set(i, b);
                }
                BitSequence result(n);
                bs.OR(other, result);
                std::cout << "OR: "; result.Print();

            }
            else if (cmd == 6) {
                BitSequence other(n);
                std::cout << "Enter " << n << " bits:\n";
                for (int i = 0; i < n; ++i) {
                    int b = readIntRange("  [" + std::to_string(i) + "] (0/1): ", 0, 1);
                    other.Set(i, b);
                }
                BitSequence result(n);
                bs.XOR(other, result);
                std::cout << "XOR: "; result.Print();

            }
            else if (cmd == 7) {
                BitSequence result(n);
                bs.NOT(result);
                std::cout << "NOT: "; result.Print();
            }

        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }
}


void AlgorithmsMenu() {
    Sequence<int>* seq = CreateSequence();

    int cmd = -1;
    while (cmd != 0) {
        std::cout << "\n=== Algorithms ===" << std::endl;
        std::cout << "Current: "; PrintSequence(seq);
        std::cout << "1. Min / Max / Avg\n";
        std::cout << "2. Median\n";
        std::cout << "3. Count inversions\n";
        std::cout << "4. All prefixes\n";
        std::cout << "5. All suffixes\n";
        std::cout << "6. Moving average\n";
        std::cout << "7. Sqrt variance per element\n";
        std::cout << "8. Mirror sum\n";
        std::cout << "0. Back\n";

        cmd = readIntRange("Choice (0-8): ", 0, 8);

        try {
            if (cmd == 1) {
                auto res = GetMinMaxAvg(*seq);
                std::cout << "Min: " << res.min
                    << "  Max: " << res.max
                    << "  Avg: " << res.avg << std::endl;

            }
            else if (cmd == 2) {
                std::cout << "Median: " << GetMedian(*seq) << std::endl;

            }
            else if (cmd == 3) {
                std::cout << "Inversions: " << CountInversions(*seq) << std::endl;

            }
            else if (cmd == 4) {
                auto* prefixes = GetPrefixes(*seq);
                std::cout << "Prefixes:" << std::endl;
                for (int i = 0; i < prefixes->GetLength(); ++i) {
                    std::cout << "  "; PrintSequence(prefixes->Get(i));
                }
                for (int i = 0; i < prefixes->GetLength(); ++i) delete prefixes->Get(i);
                delete prefixes;

            }
            else if (cmd == 5) {
                auto* suffixes = GetSuffixes(*seq);
                std::cout << "Suffixes:" << std::endl;
                for (int i = 0; i < suffixes->GetLength(); ++i) {
                    std::cout << "  "; PrintSequence(suffixes->Get(i));
                }
                for (int i = 0; i < suffixes->GetLength(); ++i) delete suffixes->Get(i);
                delete suffixes;

            }
            else if (cmd == 6) {
                auto* avg = GetMovingAverage(*seq);
                std::cout << "Moving average: [";
                for (int i = 0; i < avg->GetLength(); ++i) {
                    std::cout << avg->Get(i);
                    if (i < avg->GetLength() - 1) std::cout << ", ";
                }
                std::cout << "]" << std::endl;
                delete avg;

            }
            else if (cmd == 7) {
                auto* sv = GetSqrtVariance(*seq);
                std::cout << "Sqrt variance: [";
                for (int i = 0; i < sv->GetLength(); ++i) {
                    std::cout << sv->Get(i);
                    if (i < sv->GetLength() - 1) std::cout << ", ";
                }
                std::cout << "]" << std::endl;
                delete sv;

            }
            else if (cmd == 8) {
                auto* ms = GetMirrorSum(*seq);
                std::cout << "Mirror sum: "; PrintSequence(ms);
                delete ms;
            }

        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }
    delete seq;
}

int main() {
    SetConsoleOutputCP(65001);
    std::cout << "Hello, it's Lab 2 - Sequences" << std::endl;

    int cmd = -1;
    while (cmd != 0) {
        std::cout << "\n=== Main Menu ===" << std::endl;
        std::cout << "1. Work with Sequence<int>\n";
        std::cout << "2. Work with BitSequence\n";
        std::cout << "3. Algorithms (P1-P8)\n";
        std::cout << "0. Exit\n";

        cmd = readIntRange("Choice (0-3): ", 0, 3);

        if (cmd == 1) {
            Sequence<int>* seq = CreateSequence();
            SequenceMenu(seq);
            delete seq;
        }
        else if (cmd == 2) {
            BitSequenceMenu();
        }
        else if (cmd == 3) {
            AlgorithmsMenu();
        }
    }

    std::cout << "Bye!" << std::endl;
    return 0;
}