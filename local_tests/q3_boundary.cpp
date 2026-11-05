#define main aim_embedded_public_main
#include "../src/main/A1_q3.cpp"
#undef main

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct TestCase {
    std::string group;
    std::string name;
    std::vector<int> values;
    int k;
    long long threshold;
    std::vector<int> expected;
};

static std::string shown(const std::vector<int>& values) {
    std::ostringstream output;
    output << '{';
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (i != 0) {
            output << ',';
        }
        output << values[i];
    }
    output << '}';
    return output.str();
}

int main() {
    using aim::q3::alarms;

    const std::vector<TestCase> cases = {
        {"k-value", "empty input with k=1", {}, 1, 0, {}},
        {"k-value", "negative k is invalid", {1, 2, 3}, -1, 0, {}},
        {"k-value", "k greater than n is invalid", {1, 2, 3}, 4, 0, {}},
        {"k-value", "k=1 is valid", {5, -2, 4}, 1, 3, {0, 2}},
        {"k-value", "k=n produces one valid window", {2, 3}, 2, 5, {}},
        {"k-value", "k=n can alarm", {2, 3}, 2, 4, {1}},

        {"window", "all overlapping windows", {1, 2, 3, 4}, 2, 4, {2, 3}},
        {"window", "sum equal to threshold does not alarm", {2, 2}, 2, 4, {}},
        {"window", "negative values and threshold", {-4, -3, -2}, 2, -6, {2}},
        {"window", "first window alarms", {4, 1, 1}, 2, 3, {1}},
        {"window", "last window alarms", {1, 1, 4}, 2, 3, {2}},
        {"window", "every window alarms", {5, 5, 5}, 2, 9, {1, 2}},
        {"window", "wide integer sum", {1000000000, 1000000000, 1000000000}, 2,
         1999999999LL, {1, 2}},

        // Keep this last: an implementation that checks k < 0 but not k < 1
        // can access values[values.size()] and terminate the whole test run.
        {"k-value", "k=0 is invalid", {1, 2, 3}, 0, 0, {}},
    };

    int failed = 0;
    for (const TestCase& test : cases) {
        const std::vector<int> actual =
            alarms(test.values, test.k, test.threshold);

        if (actual == test.expected) {
            std::cout << "PASS [" << test.group << "]: " << test.name
                      << std::endl;
            continue;
        }

        ++failed;
        std::cerr << "FAIL [" << test.group << "]: " << test.name << '\n'
                  << "  values:    " << shown(test.values) << '\n'
                  << "  k:         " << test.k << '\n'
                  << "  threshold: " << test.threshold << '\n'
                  << "  expected:  " << shown(test.expected) << '\n'
                  << "  actual:    " << shown(actual) << std::endl;
    }

    std::cout << "\nResult: " << (cases.size() - failed)
              << " passed, " << failed << " failed\n";
    return failed == 0 ? 0 : 1;
}
