#define main aim_embedded_public_main
#include "../src/main/A1_q8.cpp"
#undef main

#include <iostream>
#include <string>
#include <vector>

struct TestCase {
    std::string group;
    std::string name;
    std::vector<aim::q8::Telemetry> frames;
    aim::q8::Summary expected;
};

static bool same(const aim::q8::Summary& left,
                 const aim::q8::Summary& right) {
    return left.valid == right.valid &&
           left.duplicate == right.duplicate &&
           left.invalid == right.invalid &&
           left.total_hp == right.total_hp &&
           left.ids == right.ids;
}

static void show(const aim::q8::Summary& summary) {
    std::cout << "{valid=" << summary.valid
              << ", duplicate=" << summary.duplicate
              << ", invalid=" << summary.invalid
              << ", total_hp=" << summary.total_hp
              << ", ids={";
    for (std::size_t i = 0; i < summary.ids.size(); ++i) {
        if (i != 0) {
            std::cout << ',';
        }
        std::cout << summary.ids[i];
    }
    std::cout << "}}";
}

int main() {
    using aim::q8::summarize;

    const std::vector<TestCase> cases = {
        {"validity", "invalid low hp does not occupy id",
         {{1, -1001}, {1, 50}}, {1, 0, 1, 50, {1}}},
        {"validity", "invalid high hp does not occupy id",
         {{1, 1001}, {1, 200}}, {1, 0, 1, 100, {1}}},
        {"validity", "invalid id range counts both records as invalid",
         {{-1, 0}, {10000, 0}}, {0, 0, 2, 0, {}}},
        {"validity", "id and hp range boundaries are valid",
         {{0, -1000}, {9999, 1000}}, {2, 0, 0, 100, {0, 9999}}},

        {"duplicate", "duplicate does not add hp",
         {{5, 10}, {5, 99}}, {1, 1, 0, 10, {5}}},
        {"duplicate", "invalid record before first valid does not create duplicate",
         {{5, -1001}, {5, 20}, {5, 30}}, {1, 1, 1, 20, {5}}},
        {"duplicate", "duplicate with high hp is still ignored",
         {{7, 5}, {7, 1000}}, {1, 1, 0, 5, {7}}},

        {"total-hp", "negative hp contributes zero",
         {{1, -5}, {2, 0}}, {2, 0, 0, 0, {1, 2}}},
        {"total-hp", "hp above 100 is clamped for every unique id",
         {{1, 200}, {2, 200}}, {2, 0, 0, 200, {1, 2}}},
        {"total-hp", "mixed normalized hp accumulates correctly",
         {{1, -1000}, {2, 0}, {3, 75}, {4, 100}, {5, 1000}},
         {5, 0, 0, 275, {1, 2, 3, 4, 5}}},
    };

    int failed = 0;
    for (const TestCase& test : cases) {
        const aim::q8::Summary actual = summarize(test.frames);
        if (same(actual, test.expected)) {
            std::cout << "PASS [" << test.group << "]: " << test.name
                      << std::endl;
            continue;
        }

        ++failed;
        std::cerr << "FAIL [" << test.group << "]: " << test.name
                  << "\n  expected: ";
        show(test.expected);
        std::cerr << "\n  actual:   ";
        show(actual);
        std::cerr << std::endl;
    }

    std::cout << "\nResult: " << (cases.size() - failed)
              << " passed, " << failed << " failed" << std::endl;
    return failed == 0 ? 0 : 1;
}
