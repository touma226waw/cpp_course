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

// Assessment 01 Q7; see 题面.pdf.
namespace aim::q7 {
struct FireResult { int shots; int rejected; int ammo; int heat; };

// BEGIN IMPLEMENTATION
FireResult simulate(int ammo, int heat, int cool, int shot_heat, const std::vector<int>& requests) {
    //初始化数据
    int shots = 0;
    int rejected = 0;
    //遍历每一帧
    for (const auto&frame : requests){
        //第一步，冷却
        heat -= cool;
        if (heat < 0){
            heat = 0;
        }
        //处理请求
        //请求为 0 时，本帧只执行冷却
        if (frame == 0){
            continue;
        }
        //请求为 1 时，仅当 ammo>0 且冷却后的 heat+shot_heat≤100，才实际发射；等于上限 100允许发射
        else if (frame == 1){
            //成功射击时 ammo 减一、heat 增加 shot_heat、shots 加一
            if (ammo > 0 && heat + shot_heat <= 100){
                ammo--;
                heat += shot_heat;
                shots++;
            }
            else{
                rejected++;
            }
        }
    }
    //空请求不发生任何冷却，返回原始 ammo、heat；每次调用独立
    if (requests.empty()){
       return FireResult{0,0,ammo,heat};
    }
    return FireResult{shots,rejected,ammo,heat};
}
// END IMPLEMENTATION
}

#define CHECK(...) do { if (!(__VA_ARGS__)) throw std::runtime_error("check failed"); } while(false)
#define EXPECT_INVALID(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::invalid_argument&) { caught=true; } CHECK(caught); } while(false)
#define EXPECT_RANGE(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::out_of_range&) { caught=true; } CHECK(caught); } while(false)

using namespace aim::q7;
int main() {
    try {
        { aim::q7::FireResult r=simulate(2,90,10,20,{1,1,1}); CHECK(r.shots==2 && r.rejected==1 && r.ammo==0 && r.heat==100); }
        { aim::q7::FireResult r=simulate(3,100,20,10,{0}); CHECK(r.heat==80 && r.rejected==0 && r.ammo==3); }
        std::cout << "PUBLIC 2/2\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}
