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
    std::vector<std::vector<int>> result;
    if (grid.empty()) return result;

    const std::size_t cols = grid[0].size();
    if (cols == 0) throw std::invalid_argument("grid has zero columns");
    for (const std::string& row : grid) {
        if (row.size() != cols) throw std::invalid_argument("grid rows have different lengths");
        for (char ch : row) {
            if (ch != '.' && ch != '#') throw std::invalid_argument("grid contains an invalid character");
        }
    }

    const std::size_t rows = grid.size();
    result.assign(rows, std::vector<int>(cols, 0));
    for (std::size_t r = 0; r < rows; ++r) {
        for (std::size_t c = 0; c < cols; ++c) {
            if (grid[r][c] == '#') {
                result[r][c] = -1;
                continue;
            }
            int risk_count = 0;
            if (r > 0 && grid[r - 1][c] == '#') ++risk_count;
            if (r + 1 < rows && grid[r + 1][c] == '#') ++risk_count;
            if (c > 0 && grid[r][c - 1] == '#') ++risk_count;
            if (c + 1 < cols && grid[r][c + 1] == '#') ++risk_count;
            result[r][c] = risk_count;
        }
    }
    return result;
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
