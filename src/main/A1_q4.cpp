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

// Assessment 01 Q4; see 题面.pdf.
namespace aim::q4 {
struct CommandResult { long long value; int accepted; int rejected; };

// BEGIN IMPLEMENTATION
CommandResult execute(const std::vector<std::string>& lines) {
    //初始化数据
    long long value = 0;
    int accepted = 0;
    int rejected = 0;
    //遍历识别line
    for (const std::string& line : lines) {
        //读取line行
        std::istringstream iss(line);
        std::string command;
        iss >> command;
        //检查line合法
         if (command == ""|| (command != "ADD" && command != "SET" && command != "RESET")) {
            rejected++;
            continue;
         }
        //检查line字符
        //添加字符
        if (command == "ADD") {
            //添加非法
            long long x;
            std::string extra;
            //x范围错误or缺少数据
            if (!(iss >> x) || x < -1000000 || x > 1000000) {
                rejected++;
                continue;
            }
            //溢出
            long long candidate = value + x;
            if (candidate < -1000000 || candidate > 1000000) {
                rejected++;
                continue;
            }
            //多余数据
            if (iss >> extra) {
                rejected++;
                continue;
            }
            //添加合法
            value += x;
            accepted++;
        }
        //设置字符
        else if (command == "SET") {
            //设置非法
            long long x;
            std::string extra;
            //x范围错误or缺少数据
            if (!(iss >> x )|| x < -1000000 || x > 1000000) {
                rejected++;
                continue;
            }
            //多余数据
            else if (iss >> extra) {
                rejected++;
                continue;
            }
            //设置合法
            else {
                value = x;
                accepted++;
            }
        }
        //重置字符
        else if (command == "RESET") {
            //多余数据
            std::string extra;
            if (iss >> extra) {
                rejected++;
                continue;
            }
            //重置合法
            else {
                value = 0;
                accepted++;
            }
        }
    }
    return {value, accepted, rejected};
}
// END IMPLEMENTATION
}

#define CHECK(...) do { if (!(__VA_ARGS__)) throw std::runtime_error("check failed"); } while(false)
#define EXPECT_INVALID(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::invalid_argument&) { caught=true; } CHECK(caught); } while(false)
#define EXPECT_RANGE(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::out_of_range&) { caught=true; } CHECK(caught); } while(false)

using namespace aim::q4;
int main() {
    try {
        { aim::q4::CommandResult r=execute({"ADD 5","SET 8 x","RESET","ADD -2"}); CHECK(r.value==-2 && r.accepted==3 && r.rejected==1); }
        { aim::q4::CommandResult r=execute({}); CHECK(r.value==0 && r.accepted==0 && r.rejected==0); }
        std::cout << "PUBLIC 2/2\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}
