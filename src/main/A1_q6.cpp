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

// Assessment 01 Q6; see 题面.pdf.
namespace aim::q6 {
struct Target { int id; int x; int y; int hp; bool visible; };

// BEGIN IMPLEMENTATION
int select_target(const std::vector<Target>& targets, int max_range) {
    //max_range<0 and 空输入返回 -1
    if (max_range < 0 || targets.empty()){
        return -1;
    }
    //射界合法
    //初始化数据
    int cur_hp = 0;
    int cur_id = -1;
    long long cur_dist2;
    //判断是否是第一次
    bool pre = false;
    //遍历targets
    for (const auto& t : targets){
        //定义距离
        long long dist = 1LL*t.x*t.x + 1LL*t.y*t.y;
        //targets合格
        if (t.visible == true && t.hp > 0 && dist <= 1LL*max_range*max_range){
            //数据初始值
            if (!pre){
                cur_hp = t.hp;
                cur_id = t.id;
                cur_dist2 = dist;
                pre = true;
            }
            //先比较 hp，较小者优先
            else{
                if (t.hp < cur_hp){
                    cur_hp = t.hp;
                    cur_id = t.id;
                    cur_dist2 = dist;
                }
                //只有 hp 相同时才比较平方距离，较小者优先
                else if (t.hp == cur_hp){
                    if (dist < cur_dist2){
                        cur_dist2 = dist;
                        cur_id = t.id;
                    }
                    //两者都相同时才比较id，较小者优先
                    else if (dist == cur_dist2){
                        if (t.id < cur_id){
                            cur_id = t.id;
                        }
                    }
                }
            }
        }
    }
    //无目标返回-1，有目标返回id
    if (cur_id == -1){
        return -1;
    }
    else{
        return cur_id;
    }
}
// END IMPLEMENTATION
}

#define CHECK(...) do { if (!(__VA_ARGS__)) throw std::runtime_error("check failed"); } while(false)
#define EXPECT_INVALID(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::invalid_argument&) { caught=true; } CHECK(caught); } while(false)
#define EXPECT_RANGE(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::out_of_range&) { caught=true; } CHECK(caught); } while(false)

using namespace aim::q6;
int main() {
    try {
        { CHECK(select_target({{7,3,4,20,true},{2,0,5,20,true}},5)==2); }
        { CHECK(select_target({},5)==-1); }
        std::cout << "PUBLIC 2/2\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}
