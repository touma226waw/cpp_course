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

// Assessment 01 Q5; see 题面.pdf.
namespace aim::q5 {


// BEGIN IMPLEMENTATION
std::vector<std::vector<int>> risk(const std::vector<std::string>& grid) {
     // 空行返回空矩阵
    if (grid.empty()) {
        return {};
    }
    //合法列数1-300，列数不合法
    if (grid.size() > 300|| grid.size() < 1){
        throw std::invalid_argument("grid size must be between 1 and 300");
    }
    //合法行数0-300，行数不合法
    if (grid[0].size() > 300|| grid[0].size() == 0){
        throw std::invalid_argument("grid size must be between 0 and 300");
    }
    //行列数合法检验
    else {
        //行长一致检验
        for (const auto& row : grid) {
            if (row.size() != grid[0].size()) {
                throw std::invalid_argument("all rows must have the same length");
            }
            //字符合法检验
            for (const auto& char1 : row) {
                if (char1 != '.' && char1 != '#') {
                    throw std::invalid_argument("grid can only contain '.' and '#'");
                }
            }
        }
    //行列合法，字符合法，地图合法
        //建立风险矩阵
        int rows = grid.size();//行数
        int cols = grid[0].size();//列数
        std::vector<std::vector<int>> result(rows, std::vector<int>(cols, 0));
    //计算风险值
        //障碍物为-1
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (grid[i][j] == '#') {
                    result[i][j] = -1;
                    continue;
                }
            //周围障碍物数量为风险值
                //初始化风险值
                int risk_value = 0;
                //检查周围4个方向
                //i,j为当前点坐标，di,dj为偏移量
                for (int di = -1; di <= 1; ++di) {
                    for (int dj = -1; dj <= 1; ++dj) {
                        if (di == 0 && dj == 0)
                            continue; //跳过自身
                        if ((di ==1 && dj == 1)||(di == -1 && dj == -1)||(di == 1 && dj == -1)||(di == -1 && dj == 1))
                            continue; //跳过对角线
                        //计算周围点坐标
                        int ni = i + di;
                        int nj = j + dj;
                        if (ni >= 0 && ni < rows && nj >= 0 && nj < cols) {
                            if (grid[ni][nj] == '#') {
                                risk_value++;
                            }
                        }
                    }
                }
                result[i][j] = risk_value;
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

using namespace aim::q5;
int main() {
    try {
        { CHECK((risk({".#",".."})==std::vector<std::vector<int>>{{1,-1},{0,1}})); }
        { CHECK(risk({}).empty()); }
        std::cout << "PUBLIC 2/2\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}
