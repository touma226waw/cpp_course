#define main aim_embedded_public_main
#include "../src/main/A1_q6.cpp"
#undef main

#include <iostream>
#include <string>
#include <vector>

struct TestCase {
    std::string group;
    std::string name;
    std::vector<aim::q6::Target> targets;
    int max_range;
    int expected;
};

int main() {
    using aim::q6::select_target;

    const std::vector<TestCase> cases = {
        {"range", "negative range returns -1",
         {{0, 0, 0, 1, true}}, -1, -1},
        {"range", "zero range includes the origin",
         {{0, 0, 0, 1, true}}, 0, 0},
        {"range", "maximum range includes an exact boundary target",
         {{7, 1000000, 0, 1, true}}, 1000000, 7},
        {"range", "maximum range rejects a target just outside",
         {{7, 1000000, 1, 1, true}}, 1000000, -1},

        {"eligibility", "invisible target is rejected",
         {{1, 0, 0, 10, false}}, 5, -1},
        {"eligibility", "zero hp is rejected",
         {{1, 0, 0, 0, true}}, 5, -1},
        {"eligibility", "negative hp is rejected",
         {{1, 0, 0, -1, true}}, 5, -1},
        {"eligibility", "origin with id zero is eligible",
         {{0, 0, 0, 10, true}}, 5, 0},
        {"eligibility", "exact range boundary is included",
         {{9, 3, 4, 10, true}}, 5, 9},
        {"eligibility", "target outside range is rejected",
         {{9, 0, 6, 10, true}}, 5, -1},
        {"eligibility", "ineligible first target does not end the search",
         {{1, 0, 0, 0, true}, {2, 0, 0, 10, true}}, 5, 2},
        {"eligibility", "out-of-range lower hp target cannot win",
         {{1, 0, 6, 1, true}, {2, 0, 0, 10, true}}, 5, 2},
    };

    int failed = 0;
    for (const TestCase& test : cases) {
        const int actual = select_target(test.targets, test.max_range);
        if (actual == test.expected) {
            std::cout << "PASS [" << test.group << "]: " << test.name
                      << std::endl;
            continue;
        }

        ++failed;
        std::cerr << "FAIL [" << test.group << "]: " << test.name
                  << "\n  max_range: " << test.max_range
                  << "\n  expected:  " << test.expected
                  << "\n  actual:    " << actual << std::endl;
    }

    std::cout << "\nResult: " << (cases.size() - failed)
              << " passed, " << failed << " failed" << std::endl;
    return failed == 0 ? 0 : 1;
}
