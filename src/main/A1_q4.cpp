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
    CommandResult result{0, 0, 0};
    for (const std::string& line : lines) {
        std::istringstream input(line);
        std::string op;
        bool committed = false;
        if (input >> op) {
            if (op == "RESET") {
                std::string extra;
                if (!(input >> extra)) {
                    result.value = 0;
                    committed = true;
                }
            } else if (op == "ADD" || op == "SET") {
                long long x = 0;
                std::string extra;
                if ((input >> x) && !(input >> extra)
                    && x >= -1000000 && x <= 1000000) {
                    const long long candidate = (op == "ADD") ? result.value + x : x;
                    if (candidate >= -1000000 && candidate <= 1000000) {
                        result.value = candidate;
                        committed = true;
                    }
                }
            }
        }
        if (committed) ++result.accepted;
        else ++result.rejected;
    }
    return result;
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
