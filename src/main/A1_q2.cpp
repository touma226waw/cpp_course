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

// Assessment 01 Q2; see 题面.pdf.
namespace aim::q2 {
struct LabelEdit { std::string from; std::string to; };

// BEGIN IMPLEMENTATION
std::string edit_labels(std::string labels, const std::vector<LabelEdit>& edits) {
//遍历修改记录
    for(const LabelEdit& edit : edits) {
        //from为空
        if (edit.from.empty()){
            //to不为空，追加标签
            if (!edit.to.empty()) {
                //如果labels不为空，先加逗号
                if (!labels.empty()) {
                    labels += ",";
                }
                labels += edit.to;
            }
            //to空，跳过
            else{
                continue;
            }
        }
        //from不为空
        else{
            //先找到from标签的位置，同时判断左右逗号边界
            size_t from_pos = 0;
            while ((from_pos = labels.find(edit.from, from_pos)) != std::string::npos) {
                //检查左边是否为逗号或字符串开始
                bool left_comma = (from_pos == 0 || labels[from_pos - 1] == ',');
                //检查右边是否为逗号或字符串结束
                bool right_comma = (from_pos + edit.from.length() == labels.length() || labels[from_pos + edit.from.length()] == ',');
            //左右逗号边界通过
            if (left_comma && right_comma) {
                //to不为空，修改标签
                if (!edit.to.empty()) {
                    labels.replace(from_pos, edit.from.length(), edit.to);
                    //修改后跳出循环
                    break;
                }
                //to空，删除标签
                else{
                    labels.erase(from_pos, edit.from.length());
                    //删除后可能会有连续的逗号，需要处理
                    if (from_pos < labels.length() && labels[from_pos] == ',') {
                        labels.erase(from_pos, 1);
                    }
                    else if (from_pos > 0 && labels[from_pos - 1] == ',') {
                        labels.erase(from_pos - 1, 1);
                        from_pos--;
                    }
                    break;
                }
            }
            //左右逗号边界不通过，跳过一个from的距离继续查找
                else{
                from_pos += edit.from.length();
                }
            }
        }
    }
    return labels;
}
// END IMPLEMENTATION
}

#define CHECK(...) do { if (!(__VA_ARGS__)) throw std::runtime_error("check failed"); } while(false)
#define EXPECT_INVALID(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::invalid_argument&) { caught=true; } CHECK(caught); } while(false)
#define EXPECT_RANGE(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::out_of_range&) { caught=true; } CHECK(caught); } while(false)

using namespace aim::q2;
int main() {
    try {
        { CHECK(edit_labels("cam,imu,radar",{{"imu","gyro"},{"cam",""},{"","lidar"}})=="gyro,radar,lidar"); }
        { CHECK(edit_labels("cam,cam2,cam",{{"cam","camera"},{"cam",""}})=="camera,cam2"); }
        std::cout << "PUBLIC 2/2\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}
