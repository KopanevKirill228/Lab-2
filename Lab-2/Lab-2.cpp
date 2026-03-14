#include <iostream>
#include <stdexcept>
#include <windows.h>

#include "ArraySequence.h"
#include "ListSequence.h"
#include "BitSequence.h"
#include "AdaptiveSequence.h"
#include "MapReduce.h"

void PrintSequence(const Sequence<int>* seq) {
	std::cout << "[";
	for (int i = 0; i < seq->GetLength(); ++i) {
		std::cout << seq->Get(i);
		if (i < seq->GetLength() - 1) std::cout << ", ";
	}
	std::cout << "]" << std::endl;
}

Sequence<int>* CreateSequence() {
	std::cout << "\n=== Select Sequence Type ===" << std::endl;
	std::cout << "1. MutableArraySequence<int>" << std::endl;
	std::cout << "2. MutableListSequence<int>" << std::endl;
	std::cout << "3. ImmutableArraySequence<int>" << std::endl;
	std::cout << "4. ImmutableListSequence<int>" << std::endl;
	std::cout << "5. AdaptiveSequence<int>" << std::endl;
	std::cout << "Choice: ";
	int type; std::cin >> type;

	std::cout << "Enter number of elements: ";
	int n; std::cin >> n;

	int* data = new int[n];
	for (int i = 0; i < n; ++i) {
		std::cout << "  [" << i << "]: ";
		std::cin >> data[i];
	}

	Sequence<int>* seq = nullptr;
	switch (type) {
	case 1: seq = new MutableArraySequence<int>(data, n);   break;
	case 2: seq = new MutableListSequence<int>(data, n);    break;
	case 3: seq = new ImmutableArraySequence<int>(data, n); break;
	case 4: seq = new ImmutableListSequence<int>(data, n);  break;
	case 5:
		seq = new AdaptiveSequence<int>();
		for (int i = 0; i < n; ++i) seq->Append(data[i]);
		break;
	default:
		std::cout << "Unknown type, using MutableArraySequence\n";
		seq = new MutableArraySequence<int>(data, n);
	}
	delete[] data;
	return seq;
}

void SequenceMenu(Sequence<int>*& seq) {
	int cmd = -1;
	while (cmd != 0) {
		std::cout << "\n=== Sequence Operations ===" << std::endl;
		std::cout << "Current: "; PrintSequence(seq);
		std::cout << "1.  Get element by index" << std::endl;
		std::cout << "2.  Append" << std::endl;
		std::cout << "3.  Prepend" << std::endl;
		std::cout << "4.  InsertAt" << std::endl;
		std::cout << "5.  GetSubsequence" << std::endl;
		std::cout << "6.  Concat with new sequence" << std::endl;
		std::cout << "7.  Map (x * 2)" << std::endl;
		std::cout << "8.  Where (even only)" << std::endl;
		std::cout << "9.  Reduce (sum)" << std::endl;
		std::cout << "10. Zip with new sequence" << std::endl;
		std::cout << "11. GetFirst / GetLast" << std::endl;
		std::cout << "12. GetLength" << std::endl;
		std::cout << "0.  Back" << std::endl;
		std::cout << "Choice: ";
		std::cin >> cmd;

		try {
			if (cmd == 1) {
				std::cout << "Index: "; int i; std::cin >> i;
				std::cout << "Value: " << seq->Get(i) << std::endl;

			}
			else if (cmd == 2) {
				std::cout << "Value: "; int v; std::cin >> v;
				Sequence<int>* result = seq->Append(v);
				if (result != seq) { delete seq; seq = result; }
				std::cout << "Done." << std::endl;

			}
			else if (cmd == 3) {
				std::cout << "Value: "; int v; std::cin >> v;
				Sequence<int>* result = seq->Prepend(v);
				if (result != seq) { delete seq; seq = result; }
				std::cout << "Done." << std::endl;

			}
			else if (cmd == 4) {
				std::cout << "Value: "; int v; std::cin >> v;
				std::cout << "Index: "; int i; std::cin >> i;
				Sequence<int>* result = seq->InsertAt(v, i);
				if (result != seq) { delete seq; seq = result; }
				std::cout << "Done." << std::endl;

			}
			else if (cmd == 5) {
				std::cout << "Start index: "; int s; std::cin >> s;
				std::cout << "End index:   "; int e; std::cin >> e;
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
	std::cout << "Enter number of bits: ";
	int n; std::cin >> n;
	BitSequence bs(n);

	int cmd = -1;
	while (cmd != 0) {
		std::cout << "\n=== BitSequence ===" << std::endl;
		std::cout << "Current: "; bs.Print();
		std::cout << "1. Set bit" << std::endl;
		std::cout << "2. Get bit" << std::endl;
		std::cout << "3. Flip bit" << std::endl;
		std::cout << "4. AND with new" << std::endl;
		std::cout << "5. OR with new" << std::endl;
		std::cout << "6. XOR with new" << std::endl;
		std::cout << "7. NOT" << std::endl;
		std::cout << "0. Back" << std::endl;
		std::cout << "Choice: ";
		std::cin >> cmd;

		try {
			if (cmd == 1) {
				std::cout << "Index: "; int i; std::cin >> i;
				std::cout << "Bit (0/1): "; int b; std::cin >> b;
				bs.Set(i, b);

			}
			else if (cmd == 2) {
				std::cout << "Index: "; int i; std::cin >> i;
				std::cout << "Bit[" << i << "] = " << bs.Get(i) << std::endl;

			}
			else if (cmd == 3) {
				std::cout << "Index: "; int i; std::cin >> i;
				bs.Flip(i);

			}
			else if (cmd == 4) {
				BitSequence other(n);
				std::cout << "Enter " << n << " bits:\n";
				for (int i = 0; i < n; ++i) {
					std::cout << "  [" << i << "]: "; int b; std::cin >> b;
					other.Set(i, b);
				}
				BitSequence result = bs.AND(other);
				std::cout << "AND: "; result.Print();

			}
			else if (cmd == 5) {
				BitSequence other(n);
				std::cout << "Enter " << n << " bits:\n";
				for (int i = 0; i < n; ++i) {
					std::cout << "  [" << i << "]: "; int b; std::cin >> b;
					other.Set(i, b);
				}
				BitSequence result = bs.OR(other);
				std::cout << "OR: "; result.Print();

			}
			else if (cmd == 6) {
				BitSequence other(n);
				std::cout << "Enter " << n << " bits:\n";
				for (int i = 0; i < n; ++i) {
					std::cout << "  [" << i << "]: "; int b; std::cin >> b;
					other.Set(i, b);
				}
				BitSequence result = bs.XOR(other);
				std::cout << "XOR: "; result.Print();

			}
			else if (cmd == 7) {
				BitSequence result = bs.NOT();
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
		std::cout << "1. Min / Max / Avg" << std::endl;
		std::cout << "2. Median" << std::endl;
		std::cout << "3. Count inversions" << std::endl;
		std::cout << "4. All prefixes" << std::endl;
		std::cout << "5. All suffixes" << std::endl;
		std::cout << "6. Moving average" << std::endl;
		std::cout << "7. Sqrt variance per element" << std::endl;
		std::cout << "8. Mirror sum" << std::endl;
		std::cout << "0. Back" << std::endl;
		std::cout << "Choice: ";
		std::cin >> cmd;

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
	std::cout << "=== Lab 2 - Sequences ===" << std::endl;

	int cmd = -1;
	while (cmd != 0) {
		std::cout << "\n=== Main Menu ===" << std::endl;
		std::cout << "1. Work with Sequence<int>" << std::endl;
		std::cout << "2. Work with BitSequence" << std::endl;
		std::cout << "3. Algorithms (P1-P8)" << std::endl;
		std::cout << "0. Exit" << std::endl;
		std::cout << "Choice: ";
		std::cin >> cmd;

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
		else if (cmd != 0) {
			std::cout << "Unknown command." << std::endl;
		}
	}

	std::cout << "Bye!" << std::endl;
	return 0;
}
