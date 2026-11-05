#define main aim_embedded_public_main
#include "../src/main/A1_q2.cpp"
#undef main

#include <iostream>
#include <string>
#include <vector>

struct TestCase {
    std::string name;
    std::string labels;
    std::vector<aim::q2::LabelEdit> edits;
    std::string expected;
};

static std::string shown(const std::string& text) {
    return text.empty() ? "<empty>" : '"' + text + '"';
}

int main() {
    using aim::q2::edit_labels;

    const std::vector<TestCase> cases = {
        {"left and right comma boundaries",
         "cam2,cam,camera",
         {{"cam", "imu"}},
         "cam2,imu,camera"},

        {"deletion leaves no extra commas and can become empty",
         "a,b,c",
         {{"b", ""}, {"a", ""}, {"c", ""}},
         ""},

        {"append to empty labels has no leading comma",
         "",
         {{"", "imu"}},
         "imu"},
    };

    int failed = 0;
    for (const TestCase& test : cases) {
        const std::string actual = edit_labels(test.labels, test.edits);
        if (actual == test.expected) {
            std::cout << "PASS: " << test.name << '\n';
            continue;
        }

        ++failed;
        std::cerr << "FAIL: " << test.name << '\n'
                  << "  labels:   " << shown(test.labels) << '\n'
                  << "  expected: " << shown(test.expected) << '\n'
                  << "  actual:   " << shown(actual) << '\n';
    }

    std::cout << "\nResult: " << (cases.size() - failed)
              << " passed, " << failed << " failed\n";
    return failed == 0 ? 0 : 1;
}
