#define main aim_embedded_public_main
#include "../src/main/A1_q4.cpp"
#undef main

#include <iostream>
#include <string>
#include <vector>

struct TestCase {
    std::string group;
    std::string name;
    std::vector<std::string> lines;
    aim::q4::CommandResult expected;
};

static bool same(const aim::q4::CommandResult& left,
                 const aim::q4::CommandResult& right) {
    return left.value == right.value &&
           left.accepted == right.accepted &&
           left.rejected == right.rejected;
}

static void show(const aim::q4::CommandResult& result) {
    std::cout << "{value=" << result.value
              << ", accepted=" << result.accepted
              << ", rejected=" << result.rejected << '}';
}

int main() {
    using aim::q4::execute;

    const std::vector<TestCase> cases = {
        {"range", "SET accepts maximum x", {"SET 1000000"}, {1000000, 1, 0}},
        {"range", "SET rejects x above maximum", {"SET 1000001"}, {0, 0, 1}},
        {"range", "SET accepts minimum x", {"SET -1000000"}, {-1000000, 1, 0}},
        {"range", "SET rejects x below minimum", {"SET -1000001"}, {0, 0, 1}},
        {"range", "ADD accepts maximum x", {"ADD 1000000"}, {1000000, 1, 0}},
        {"range", "ADD rejects x above maximum", {"ADD 1000001"}, {0, 0, 1}},
        {"range", "ADD accepts minimum x", {"ADD -1000000"}, {-1000000, 1, 0}},
        {"range", "ADD rejects x below minimum", {"ADD -1000001"}, {0, 0, 1}},
        {"range", "ADD rejects candidate above maximum",
         {"SET 1000000", "ADD 1"}, {1000000, 1, 1}},
        {"range", "ADD rejects candidate below minimum",
         {"SET -1000000", "ADD -1"}, {-1000000, 1, 1}},
        {"range", "ADD still rejects x outside range when candidate is valid",
         {"SET -1000000", "ADD 2000000"}, {-1000000, 1, 1}},
        {"range", "ADD accepts x inside range when candidate is valid",
         {"SET -1000000", "ADD 1000000"}, {0, 2, 0}},

        {"parsing", "ADD rejects extra field", {"ADD 5 extra"}, {0, 0, 1}},
        {"parsing", "SET rejects extra field", {"SET 5 extra"}, {0, 0, 1}},
        {"parsing", "RESET rejects extra field", {"RESET extra"}, {0, 0, 1}},
        {"parsing", "ADD rejects missing field", {"ADD"}, {0, 0, 1}},
        {"parsing", "SET rejects missing field", {"SET"}, {0, 0, 1}},
        {"parsing", "ADD rejects decimal suffix", {"ADD 1.5"}, {0, 0, 1}},
        {"parsing", "SET rejects decimal suffix", {"SET -2.25"}, {0, 0, 1}},
        {"parsing", "unknown lowercase operation is rejected",
         {"add 5"}, {0, 0, 1}},
        {"parsing", "unknown operation is rejected", {"NOOP 1"}, {0, 0, 1}},
        {"parsing", "numeric overflow is rejected",
         {"ADD 9223372036854775808"}, {0, 0, 1}},

        {"empty", "empty input has no commands", {}, {0, 0, 0}},
        {"empty", "one empty line is rejected", {""}, {0, 0, 1}},
        {"empty", "whitespace-only line is rejected", {"   "}, {0, 0, 1}},
        {"empty", "empty lines are counted among valid commands",
         {"", "ADD 2", "   ", "RESET"}, {0, 2, 2}},
    };

    int failed = 0;
    for (const TestCase& test : cases) {
        const aim::q4::CommandResult actual = execute(test.lines);

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
