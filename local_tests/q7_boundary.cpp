#define main aim_embedded_public_main
#include "../src/main/A1_q7.cpp"
#undef main

#include <iostream>
#include <string>
#include <vector>

struct TestCase {
    std::string group;
    std::string name;
    int ammo;
    int heat;
    int cool;
    int shot_heat;
    std::vector<int> requests;
    aim::q7::FireResult expected;
};

static bool same(const aim::q7::FireResult& left,
                 const aim::q7::FireResult& right) {
    return left.shots == right.shots &&
           left.rejected == right.rejected &&
           left.ammo == right.ammo &&
           left.heat == right.heat;
}

static void show(const aim::q7::FireResult& result) {
    std::cout << "{shots=" << result.shots
              << ", rejected=" << result.rejected
              << ", ammo=" << result.ammo
              << ", heat=" << result.heat << '}';
}

int main() {
    using aim::q7::simulate;

    const std::vector<TestCase> cases = {
        {"cooling", "cooling below zero clamps heat to zero",
         2, 5, 10, 20, {0}, {0, 0, 2, 0}},
        {"cooling", "zero initial heat stays zero after cooling",
         1, 0, 25, 50, {0}, {0, 0, 1, 0}},

        {"upper-limit", "heat plus shot heat equal to 100 fires",
         1, 70, 0, 30, {1}, {1, 0, 0, 100}},
        {"upper-limit", "zero shot heat at heat 100 fires",
         1, 100, 0, 0, {1}, {1, 0, 0, 100}},
        {"upper-limit", "heat plus shot heat above 100 is rejected",
         1, 71, 0, 30, {1}, {0, 1, 1, 71}},

        {"empty", "empty requests do not cool",
         4, 80, 10, 30, {}, {0, 0, 4, 80}},
        {"empty", "empty requests preserve initial resources",
         0, 100, 20, 50, {}, {0, 0, 0, 100}},
    };

    int passed = 0;
    int failed = 0;

    for (const TestCase& test : cases) {
        const aim::q7::FireResult actual =
            simulate(test.ammo, test.heat, test.cool,
                     test.shot_heat, test.requests);

        if (same(actual, test.expected)) {
            std::cout << "PASS [" << test.group << "]: " << test.name
                      << std::endl;
            ++passed;
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

    const aim::q7::FireResult first = simulate(2, 90, 10, 20, {1, 1});
    const aim::q7::FireResult second = simulate(3, 40, 5, 10, {});
    const aim::q7::FireResult expected_second{0, 0, 3, 40};

    if (first.ammo == 1 && first.rejected == 1 &&
        same(second, expected_second)) {
        std::cout << "PASS [empty]: calls are independent" << std::endl;
        ++passed;
    } else {
        ++failed;
        std::cerr << "FAIL [empty]: calls are independent"
                  << "\n  expected second: ";
        show(expected_second);
        std::cerr << "\n  actual second:   ";
        show(second);
        std::cerr << std::endl;
    }

    std::cout << "\nResult: " << passed << " passed, "
              << failed << " failed" << std::endl;
    return failed == 0 ? 0 : 1;
}
