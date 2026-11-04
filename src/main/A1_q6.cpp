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
    if (max_range < 0) return -1;
    const long long range_sq = static_cast<long long>(max_range) * static_cast<long long>(max_range);

    bool found = false;
    int best_id = -1;
    long long best_hp = 0;
    long long best_dist_sq = 0;
    for (const Target& target : targets) {
        if (!target.visible || target.hp <= 0) continue;
        const long long dist_sq = static_cast<long long>(target.x) * target.x
                                + static_cast<long long>(target.y) * target.y;
        if (dist_sq > range_sq) continue;
        const bool better = !found
            || target.hp < best_hp
            || (target.hp == best_hp && dist_sq < best_dist_sq)
            || (target.hp == best_hp && dist_sq == best_dist_sq && target.id < best_id);
        if (better) {
            found = true;
            best_id = target.id;
            best_hp = target.hp;
            best_dist_sq = dist_sq;
        }
    }
    return found ? best_id : -1;
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
