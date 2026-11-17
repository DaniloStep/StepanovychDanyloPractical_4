
#include <iostream>
#include <vector>

using namespace std;

struct Statistics {
    long long comparisons = 0;
    long long moves = 0;
};

struct TestCase {
    string name;
    vector<int> data;
};

vector<int> halvingGaps(int n) {
    vector<int> gaps;

    for (int gap = n / 2; gap > 0; gap /= 2) {
        gaps.push_back(gap);
    }

    return gaps;
}

vector<int> tripleGaps(int n) {
    vector<int> gaps;
    long long gap = 1;

    while (gap < n) {
        gaps.push_back((int)gap);
        gap = 3 * gap + 1;
    }

    vector<int> result;

    for (int i = (int)gaps.size() - 1; i >= 0; i--) {
        result.push_back(gaps[i]);
    }

    return result;
}

void printArray(const vector<int>& a) {
    cout << "[ ";

    for (int x : a) {
        cout << x << " ";
    }

    cout << "]\n";
}

void printGaps(const vector<int>& gaps) {
    cout << "Gaps: ";

    if (gaps.empty()) {
        cout << "none";
    }

    for (int gap : gaps) {
        cout << gap << " ";
    }

    cout << "\n";
}

bool isSorted(const vector<int>& a) {
    for (int i = 1; i < (int)a.size(); i++) {
        if (a[i - 1] > a[i]) {
            return false;
        }
    }

    return true;
}

long long sumArray(const vector<int>& a) {
    long long sum = 0;

    for (int x : a) {
        sum += x;
    }

    return sum;
}

long long squareSum(const vector<int>& a) {
    long long sum = 0;

    for (int x : a) {
        sum += 1LL * x * x;
    }

    return sum;
}

bool validate(
    const vector<int>& original,
    const vector<int>& sorted
) {
    if (original.size() != sorted.size()) {
        return false;
    }

    if (!isSorted(sorted)) {
        return false;
    }

    if (sumArray(original) != sumArray(sorted)) {
        return false;
    }

    if (squareSum(original) != squareSum(sorted)) {
        return false;
    }

    return true;
}

void shellSort(
    vector<int>& a,
    const vector<int>& gaps,
    Statistics& stats,
    bool showSteps = false
) {
    for (int gap : gaps) {
        for (int i = gap; i < (int)a.size(); i++) {
            int key = a[i];
            int j = i;

            while (j >= gap) {
                stats.comparisons++;

                if (a[j - gap] > key) {
                    a[j] = a[j - gap];
                    stats.moves++;
                    j -= gap;
                }
                else {
                    break;
                }
            }

            a[j] = key;
            stats.moves++;
        }

        if (showSteps) {
            cout << "After gap " << gap << ": ";
            printArray(a);
        }
    }
}

void insertionSort(
    vector<int>& a,
    Statistics& stats
) {
    for (int i = 1; i < (int)a.size(); i++) {
        int key = a[i];
        int j = i;

        while (j > 0) {
            stats.comparisons++;

            if (a[j - 1] > key) {
                a[j] = a[j - 1];
                stats.moves++;
                j--;
            }
            else {
                break;
            }
        }

        a[j] = key;
        stats.moves++;
    }
}

void runSingleSort(
    const vector<int>& original,
    const vector<int>& gaps,
    const string& title,
    bool showSteps
) {
    vector<int> a = original;
    Statistics stats;

    cout << "\n" << title << "\n";
    printGaps(gaps);

    shellSort(a, gaps, stats, showSteps);

    cout << "Result: ";
    printArray(a);

    cout << "Comparisons: " << stats.comparisons << "\n";
    cout << "Moves: " << stats.moves << "\n";

    cout << "Validation: "
        << (validate(original, a) ? "SUCCESS" : "ERROR")
        << "\n";
}

void runInsertion(
    const vector<int>& original,
    bool showResult = true
) {
    vector<int> a = original;
    Statistics stats;

    insertionSort(a, stats);

    if (showResult) {
        cout << "\nInsertion sort\n";
        cout << "Result: ";
        printArray(a);

        cout << "Comparisons: " << stats.comparisons << "\n";
        cout << "Moves: " << stats.moves << "\n";

        cout << "Validation: "
            << (validate(original, a) ? "SUCCESS" : "ERROR")
            << "\n";
    }
}

bool runTests();

unsigned int randomState = 123456789;

int nextRandom() {
    randomState = randomState * 1103515245u + 12345u;
    return (int)((randomState / 65536u) % 32768u);
}

vector<int> makeRandomArray(int n) {
    vector<int> a;

    for (int i = 0; i < n; i++) {
        a.push_back(nextRandom() % 20001 - 10000);
    }

    return a;
}

vector<int> makeSortedArray(int n) {
    vector<int> a;

    for (int i = 0; i < n; i++) {
        a.push_back(i);
    }

    return a;
}

vector<int> makeReverseArray(int n) {
    vector<int> a;

    for (int i = n - 1; i >= 0; i--) {
        a.push_back(i);
    }

    return a;
}

vector<int> makeNearlySortedArray(int n) {
    vector<int> a = makeSortedArray(n);

    if (n > 1) {
        for (int i = 0; i < n / 10; i++) {
            int x = nextRandom() % n;
            int y = nextRandom() % n;

            int temp = a[x];
            a[x] = a[y];
            a[y] = temp;
        }
    }

    return a;
}

void runOneExperiment(
    const vector<int>& original,
    const string& label
) {
    vector<int> a1 = original;
    vector<int> a2 = original;
    vector<int> a3 = original;

    Statistics s1, s2, s3;

    vector<int> gaps1 = halvingGaps((int)original.size());
    vector<int> gaps2 = tripleGaps((int)original.size());

    shellSort(a1, gaps1, s1);
    shellSort(a2, gaps2, s2);
    insertionSort(a3, s3);

    cout << "\nDataset: " << label << "\n";

    cout << "Algorithm              Comparisons       Moves\n";

    cout << "Shell halving          "
        << s1.comparisons << "              "
        << s1.moves << "\n";

    cout << "Shell triple           "
        << s2.comparisons << "              "
        << s2.moves << "\n";

    cout << "Insertion              "
        << s3.comparisons << "              "
        << s3.moves << "\n";

    long long minComparisons = s1.comparisons;

    if (s2.comparisons < minComparisons)
        minComparisons = s2.comparisons;

    if (s3.comparisons < minComparisons)
        minComparisons = s3.comparisons;

    long long minMoves = s1.moves;

    if (s2.moves < minMoves)
        minMoves = s2.moves;

    if (s3.moves < minMoves)
        minMoves = s3.moves;

    cout << "Best comparisons: ";

    if (s1.comparisons == minComparisons)
        cout << "Shell halving ";

    if (s2.comparisons == minComparisons)
        cout << "Shell triple ";

    if (s3.comparisons == minComparisons)
        cout << "Insertion ";

    cout << "\nBest moves: ";

    if (s1.moves == minMoves)
        cout << "Shell halving ";

    if (s2.moves == minMoves)
        cout << "Shell triple ";

    if (s3.moves == minMoves)
        cout << "Insertion ";

    cout << "\n";

    cout << "Validation: "
        << (isSorted(a1) && isSorted(a2) && isSorted(a3)
            ? "SUCCESS" : "ERROR")
        << "\n";
}

void runExperiments() {
    int sizes[] = { 100, 1000, 10000 };

    for (int n : sizes) {
        cout << "\n====================================\n";
        cout << "ARRAY SIZE: " << n << "\n";
        cout << "====================================\n";

        vector<int> sorted = makeSortedArray(n);
        vector<int> reversed = makeReverseArray(n);
        vector<int> nearly = makeNearlySortedArray(n);
        vector<int> mixed = makeRandomArray(n);

        runOneExperiment(sorted, "Sorted");
        runOneExperiment(reversed, "Reversed");
        runOneExperiment(nearly, "Nearly sorted");
        runOneExperiment(mixed, "Mixed");
    }
}

bool runTests() {
    vector<TestCase> tests;

    tests.push_back({ "Empty", {} });
    tests.push_back({ "One element", {5} });
    tests.push_back({ "Two elements", {2, 1} });
    tests.push_back({ "Sorted", {1, 2, 3, 4, 5} });
    tests.push_back({ "Reversed", {5, 4, 3, 2, 1} });
    tests.push_back({ "Duplicates", {3, 1, 3, 2, 1} });
    tests.push_back({ "Negative", {-5, -1, -9, 0, 4} });
    tests.push_back({ "Mixed", {8, 2, 7, 1, 9, 3} });
    tests.push_back({ "All equal", {4, 4, 4, 4, 4} });
    tests.push_back({ "Zero values", {0, 0, 1, -1, 2, -2} });
    tests.push_back({ "Odd length", {9, 3, 7, 1, 5, 2, 8} });
    tests.push_back({ "Long array", {
        20, 4, 18, 2, 16, 6, 14, 8, 12, 10,
        1, 19, 3, 17, 5, 15, 7, 13, 9, 11
    } });

    int passed = 0;
    int total = 0;

    cout << "\n========== AUTOMATIC TESTS ==========\n";

    for (const TestCase& test : tests) {
        vector<int> original = test.data;

        vector<int> a1 = original;
        vector<int> a2 = original;

        Statistics s1, s2;

        vector<int> gaps1 = halvingGaps((int)original.size());
        vector<int> gaps2 = tripleGaps((int)original.size());

        shellSort(a1, gaps1, s1);
        shellSort(a2, gaps2, s2);

        bool result1 = validate(original, a1);
        bool result2 = validate(original, a2);

        total++;

        cout << test.name << " - Halving: "
            << (result1 ? "PASS" : "FAIL") << "\n";

        if (result1) passed++;

        total++;

        cout << test.name << " - Triple: "
            << (result2 ? "PASS" : "FAIL") << "\n";

        if (result2) passed++;
    }

    cout << "\nTests passed: " << passed
        << " / " << total << "\n";

    return passed == total;
}

vector<int> inputArray() {
    int n;

    cout << "Enter array size (0..100000): ";

    while (!(cin >> n) || n < 0 || n > 100000) {
        cout << "Invalid size. Try again: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }

    vector<int> a(n);

    cout << "Enter " << n << " integer values:\n";

    for (int i = 0; i < n; i++) {
        while (!(cin >> a[i])) {
            cout << "Invalid value. Try again: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }

    return a;
}

int main() {
    cout << "SHELL SORT EXPERIMENT\n";
    cout << "=====================\n\n";

    vector<int> original = inputArray();

    cout << "\nOriginal array: ";
    printArray(original);

    cout << "\nChoose demonstration mode:\n";
    cout << "1 - Show intermediate arrays\n";
    cout << "0 - Hide intermediate arrays\n";

    int choice;

    while (!(cin >> choice) || (choice != 0 && choice != 1)) {
        cout << "Enter 0 or 1: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }

    bool showSteps = (choice == 1);

    runSingleSort(
        original,
        halvingGaps((int)original.size()),
        "SHELL SORT - HALVING GAPS",
        showSteps
    );

    runSingleSort(
        original,
        tripleGaps((int)original.size()),
        "SHELL SORT - TRIPLE GAPS",
        showSteps
    );

    runInsertion(original);

    bool testsPassed = runTests();

    if (testsPassed) {
        cout << "\nAll automatic tests passed.\n";
    }
    else {
        cout << "\nSome automatic tests failed.\n";
    }

    cout << "\nRun performance experiments? (1 - Yes, 0 - No): ";

    int experimentChoice;

    while (!(cin >> experimentChoice) ||
        (experimentChoice != 0 && experimentChoice != 1)) {
        cout << "Enter 0 or 1: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }

    if (experimentChoice == 1) {
        runExperiments();
    }

    cout << "\nProgram finished.\n";

    return 0;
}
