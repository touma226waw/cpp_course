#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <limits>
#include <memory>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

// Assessment 01 Q3; see 题面.pdf.
namespace aim::q3 {


// BEGIN IMPLEMENTATION
std::vector<int> alarms(const std::vector<int>& values, int k, long long threshold) {
    //如果k小于0或者大于values的长度，返回空vector
    if(k < 1|| k > static_cast<int>(values.size())) {
       return std::vector<int>{};
    }
    //k正常
    else{
        std::vector<int> result;
        //定义窗口和
        long long sum = 0;
        //计算第一个窗口
        for (int i = 0; i <k; ++i) {
            sum += values[i];
        }
        //如果和大于阈值，记录起始索引
        if (sum > threshold) {
            result.push_back(k-1);
        }
        //迭代，滑动窗口
        int right = static_cast<int>(values.size());
        for (right = k; right < values.size(); ++right){
            sum += values[right] - values[right - k];
            if (sum > threshold) {
                result.push_back(right);
                }
        }
        return result;
    }
}
// END IMPLEMENTATION
}

#define CHECK(...) do { if (!(__VA_ARGS__)) throw std::runtime_error("check failed"); } while(false)
#define EXPECT_INVALID(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::invalid_argument&) { caught=true; } CHECK(caught); } while(false)
#define EXPECT_RANGE(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::out_of_range&) { caught=true; } CHECK(caught); } while(false)

using namespace aim::q3;
int main() {
    try {
        { CHECK((alarms({1,2,3,4},2,4)==std::vector<int>{2,3})); }
        { CHECK(alarms({},1,0).empty()); }
        std::cout << "PUBLIC 2/2\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}
