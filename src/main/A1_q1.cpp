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

// Assessment 01 Q1; see 题面.pdf.
namespace aim::q1 {

// BEGIN IMPLEMENTATION
long long ratio(long long hp, long long max_hp) {
if (max_hp <= 0) {
    return -1;
    }
else{
    if (hp < 0) {
        return 0;
        }
    else if (hp > max_hp) {
        return 100;
        }
    else{
        return (hp * 100) / max_hp;
        }
    }
}

std::string report(int id, long long hp, long long max_hp, int battery) {
//初始化数据
int report_ID = id;
int report_HP = ratio(hp, max_hp);
std::string report_BAT;
std::string report_STATE;
//处理电池问题
    //电池电量界限0-100处理
    if (battery < 0) {
        battery = 0;
        report_BAT = std::to_string(battery);
        report_STATE = "LOW";
    }
    else if (battery > 100) {
        battery = 100;
        report_BAT = std::to_string(battery);
        report_STATE = "OK";
    }
    else{
        report_BAT = std::to_string(battery);
        //处理电池状态
        battery = std::clamp(battery, 0, 100);
        report_BAT = std::to_string(battery);
        if(battery < 20){
            report_STATE = "LOW";
        }
        else if(battery >= 20 && battery < 60){
            report_STATE = "WARNING";
        }
        else{
            report_STATE = "OK";
        }
    }
    //处理最大血异常，参考规范2，如果最大生命值小于等于0，返回INVALID
    if (max_hp <= 0){
        return "ID=" + std::to_string(report_ID) + ";HP=" + "INVALID" + ";BAT=" + report_BAT + ";STATE=" + report_STATE;
        }
    //最大血无异常，返回报告
    return "ID=" + std::to_string(report_ID) + ";HP=" + std::to_string(report_HP) + ";BAT=" + report_BAT + ";STATE=" + report_STATE;
}
// END IMPLEMENTATION
}

#define CHECK(...) do { if (!(__VA_ARGS__)) throw std::runtime_error("check failed"); } while(false)
#define EXPECT_INVALID(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::invalid_argument&) { caught=true; } CHECK(caught); } while(false)
#define EXPECT_RANGE(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::out_of_range&) { caught=true; } CHECK(caught); } while(false)

using namespace aim::q1;
int main() {
    try {
        { CHECK(ratio(2,3)==66); }
        { CHECK(report(7,2,3,20)=="ID=7;HP=66;BAT=20;STATE=WARNING"); }
        std::cout << "PUBLIC 2/2\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}
