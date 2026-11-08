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

// Assessment 01 Q8; see 题面.pdf.
namespace aim::q8 {
struct Telemetry { int id; int hp; };
struct Summary { int valid; int duplicate; int invalid; long long total_hp; std::vector<int> ids; };

// BEGIN IMPLEMENTATION
Summary summarize(const std::vector<Telemetry>& frames) {
    //初始化
    std::array<bool,10000> report_id{};
    Summary s{};
    s.valid = 0;
   s.duplicate = 0;
   s.invalid = 0;
    s.total_hp = 0;
    //遍历Telemetry
    for (const auto&t :frames){
    //只有 id∈[0,9999] 且 hp∈[-1000,1000] 才合法。任一字段范围不满足时计 invalid
        if (t.id < 0 || t.id > 9999 || t.hp < -1000 || t.hp > 1000){
            s.invalid++;
            continue;
        }
        //id and hp 数值合法
        else{
            if (report_id[t.id]){
                s.duplicate++;
                continue;
            }
            else{
                //首次合法记录计 valid；把 hp 夹到 [0,100] 后累加到 long long 的 total_hp，并保存编号
                report_id[t.id] = true;
                s.valid++;
                s.ids.push_back(t.id);
                if (t.hp > 100)
                    s.total_hp += 100;
                else if( t.hp < 0)
                    s.total_hp += 0;
                else
                    s.total_hp += t.hp;
            }
            continue;
        }
    }
    //ids 按编号升序返回
    sort(s.ids.begin(),s.ids.end());
    //return
    if (frames.empty())
        return s;
    else
        return s;
}
// END IMPLEMENTATION
}

#define CHECK(...) do { if (!(__VA_ARGS__)) throw std::runtime_error("check failed"); } while(false)
#define EXPECT_INVALID(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::invalid_argument&) { caught=true; } CHECK(caught); } while(false)
#define EXPECT_RANGE(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::out_of_range&) { caught=true; } CHECK(caught); } while(false)

using namespace aim::q8;
int main() {
    try {
        { aim::q8::Summary r=summarize({{2,200},{1,-5},{2,10},{-1,30}}); CHECK(r.valid==2 && r.duplicate==1 && r.invalid==1 && r.total_hp==100 && (r.ids==std::vector<int>{1,2})); }
        { aim::q8::Summary r=summarize({{1,-1001},{1,30},{1,1001}}); CHECK(r.valid==1 && r.duplicate==0 && r.invalid==2 && r.total_hp==30 && (r.ids==std::vector<int>{1})); }
        std::cout << "PUBLIC 2/2\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}
