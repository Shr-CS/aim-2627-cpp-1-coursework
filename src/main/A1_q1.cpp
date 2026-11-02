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
    if(max_hp <= 0) return -1;
    if(hp <= 0) return 0;
    if(max_hp > 0){
        if(hp >= max_hp) return 100;
        else return int(hp*100/max_hp);
    }
}

std::string report(int id, long long hp, long long max_hp, int battery) { 
    std::string STATU;
    std::string HP;
    if(battery < 0) {
        battery = 0;
    }
    else if(battery > 100) {
        battery = 100;
    }
    if(battery >= 60) {
        STATU = "OK";
    }
    else if(battery < 20) {
        STATU = "LOW";
    }
    else {
        STATU = "WARNING";
    }
    if(max_hp < 0) {
        HP = "INVALID";
    }
    else {
        HP = std::to_string(ratio(hp, max_hp));
    }
    return "ID=" + std::to_string(id) + ";HP=" + HP + ";BAT=" + std::to_string(battery) + ";STATE=" + STATU;
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
