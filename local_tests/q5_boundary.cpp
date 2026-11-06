#define main aim_embedded_public_main
#include "../src/main/A1_q5.cpp"
#undef main

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

static std::string shown(const std::vector<std::vector<int>>& matrix) {
    std::ostringstream output;
    output << '{';
    for (std::size_t row = 0; row < matrix.size(); ++row) {
        if (row != 0) {
            output << ',';
        }
        output << '{';
        for (std::size_t column = 0; column < matrix[row].size(); ++column) {
            if (column != 0) {
                output << ',';
            }
            output << matrix[row][column];
        }
        output << '}';
    }
    output << '}';
    return output.str();
}

int main() {
    using aim::q5::risk;

    int passed = 0;
    int failed = 0;

    auto expect_equal = [&](const std::string& group,
                            const std::string& name,
                            const std::vector<std::string>& grid,
                            const std::vector<std::vector<int>>& expected) {
        try {
            const std::vector<std::vector<int>> actual = risk(grid);
            if (actual == expected) {
                std::cout << "PASS [" << group << "]: " << name << std::endl;
                ++passed;
                return;
            }

            std::cerr << "FAIL [" << group << "]: " << name
                      << "\n  expected: " << shown(expected)
                      << "\n  actual:   " << shown(actual) << std::endl;
        } catch (const std::exception& error) {
            std::cerr << "FAIL [" << group << "]: " << name
                      << "\n  unexpected exception: " << error.what()
                      << std::endl;
        }
        ++failed;
    };

    auto expect_invalid = [&](const std::string& group,
                              const std::string& name,
                              const std::vector<std::string>& grid) {
        try {
            const std::vector<std::vector<int>> actual = risk(grid);
            std::cerr << "FAIL [" << group << "]: " << name
                      << "\n  expected invalid_argument"
                      << "\n  actual matrix: " << shown(actual) << std::endl;
        } catch (const std::invalid_argument&) {
            std::cout << "PASS [" << group << "]: " << name << std::endl;
            ++passed;
            return;
        } catch (const std::exception& error) {
            std::cerr << "FAIL [" << group << "]: " << name
                      << "\n  expected invalid_argument"
                      << "\n  actual exception: " << error.what() << std::endl;
        }
        ++failed;
    };

    expect_equal("empty", "empty grid returns an empty matrix", {}, {});

    expect_equal("row-length", "equal row lengths are accepted",
                 {".#", ".."}, {{1, -1}, {0, 1}});
    expect_invalid("row-length", "different row lengths are rejected",
                   {"..", "."});

    expect_equal("limits", "300 rows are allowed",
                 std::vector<std::string>(300, "."),
                 std::vector<std::vector<int>>(300, std::vector<int>(1, 0)));
    expect_equal("limits", "300 columns are allowed",
                 {std::string(300, '.')},
                 std::vector<std::vector<int>>(1, std::vector<int>(300, 0)));
    expect_invalid("limits", "301 rows are rejected",
                   std::vector<std::string>(301, "."));
    expect_invalid("limits", "301 columns are rejected",
                   {std::string(301, '.')});

    std::cout << "\nResult: " << passed << " passed, "
              << failed << " failed" << std::endl;
    return failed == 0 ? 0 : 1;
}
