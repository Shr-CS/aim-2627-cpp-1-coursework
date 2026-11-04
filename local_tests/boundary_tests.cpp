// 自补充边界测试：覆盖题面隐藏边界速查与规范中的关键端点。
// 编译方式见同目录 run.sh；不修改任何公开测试与实现接口。
#define main local_main_q1
#include "../src/main/A1_q1.cpp"
#undef main
#define main local_main_q2
#include "../src/main/A1_q2.cpp"
#undef main
#define main local_main_q3
#include "../src/main/A1_q3.cpp"
#undef main
#define main local_main_q4
#include "../src/main/A1_q4.cpp"
#undef main

#undef CHECK
#define CHECK(...) do { if (!(__VA_ARGS__)) { \
    std::cerr << "CHECK failed at line " << __LINE__ << ": " << #__VA_ARGS__ << "\n"; \
    throw std::runtime_error("check failed"); } } while(false)

int main() {
    try {
        // ---- Q1 定点遥测规范化 ----
        CHECK(ratio(-5, 100) == 0);              // 负血量归零
        CHECK(ratio(200, 100) == 100);           // 不超过 100%
        CHECK(ratio(10, 0) == -1);               // max_hp=0 不除法
        CHECK(ratio(10, -7) == -1);              // 负 max_hp 同样无效
        CHECK(ratio(0, 100) == 0);
        CHECK(ratio(1000000000000LL, 1000000000000LL) == 100);  // 10^12 上界
        CHECK(ratio(1, 1000000000000LL) == 0);   // 截断为 0
        CHECK(ratio(2, 3) == 66);                // 截断而非四舍五入
        CHECK(report(0, 4, 0, -7) == "ID=0;HP=INVALID;BAT=0;STATE=LOW");
        CHECK(report(1, 50, 100, 59) == "ID=1;HP=50;BAT=59;STATE=WARNING");
        CHECK(report(1, 50, 100, 60) == "ID=1;HP=50;BAT=60;STATE=OK");
        CHECK(report(1, 50, 100, 19) == "ID=1;HP=50;BAT=19;STATE=LOW");
        CHECK(report(1, 50, 100, 20) == "ID=1;HP=50;BAT=20;STATE=WARNING");
        CHECK(report(1, 1, 2, 120) == "ID=1;HP=50;BAT=100;STATE=OK");   // 电量夹取
        CHECK(report(9999, -5, 3, -1) == "ID=9999;HP=0;BAT=0;STATE=LOW");

        // ---- Q2 设备标签维护 ----
        CHECK(edit_labels("cam2,cam", {{"cam", "imu"}}) == "cam2,imu");  // 完整标签
        CHECK(edit_labels("a,b,a", {{"a", "x"}}) == "x,b,a");            // 仅首次命中
        CHECK(edit_labels("a,b", {{"b", ""}}) == "a");                   // 删尾部逗号
        CHECK(edit_labels("a,b", {{"a", ""}}) == "b");                   // 删首部逗号
        CHECK(edit_labels("a", {{"a", ""}}) == "");                      // 删唯一标签
        CHECK(edit_labels("a,b,c", {{"b", ""}}) == "a,c");               // 删中间
        CHECK(edit_labels("a", {{"a", ""}, {"", "b"}}) == "b");          // 清空后追加
        CHECK(edit_labels("", {{"", "x"}}) == "x");                      // 空名单追加
        CHECK(edit_labels("", {{"a", "b"}}) == "");                      // 空名单查找
        CHECK(edit_labels("a,b", {}) == "a,b");                          // 空编辑
        CHECK(edit_labels("Cam,cam", {{"cam", "x"}}) == "Cam,x");        // 大小写敏感
        CHECK(edit_labels("a", {{"a", "a"}}) == "a");                    // from==to 一次
        CHECK(edit_labels("cam,camera,cam", {{"cam", ""}}) == "camera,cam");
        CHECK(edit_labels("a,b", {{"a", "aaaa"}, {"aaaa", "z"}}) == "z,b");  // 变长改名后继续
        CHECK(edit_labels("a,b", {{"", ""}}) == "a,b");                  // 双空不操作

        // ---- Q3 窗口异常检测 ----
        CHECK(alarms({2, 2}, 2, 4).empty());                     // 相等不报警
        CHECK((alarms({-4, -3, -2}, 2, -6) == std::vector<int>{2}));  // 负和比较
        CHECK(alarms({1}, 0, 0).empty());                        // k=0
        CHECK(alarms({1}, -1, 0).empty());                       // k<1
        CHECK(alarms({1, 2}, 3, -100).empty());                  // k>n
        CHECK((alarms({1, 2}, 1, 1) == std::vector<int>{1}));    // k=1 右端下标
        CHECK((alarms({5}, 1, 4) == std::vector<int>{0}));       // k=n
        CHECK((alarms({1, 2, 3, 4}, 4, 9) == std::vector<int>{3}));
        CHECK(alarms({1, 2, 3, 4}, 4, 10).empty());              // 恰等不报
        CHECK((alarms({1000000000, 1000000000}, 2, 1999999999LL) == std::vector<int>{1}));  // 大整数和
        CHECK((alarms({-1, -1, -1}, 1, -2) == std::vector<int>{0, 1, 2}));

        // ---- Q4 严格命令事务 ----
        {
            aim::q4::CommandResult r = aim::q4::execute({"SET 5", "RESET x"});
            CHECK(r.value == 5 && r.accepted == 1 && r.rejected == 1);
        }
        {
            aim::q4::CommandResult r = aim::q4::execute({"SET 1000000", "ADD 1"});
            CHECK(r.value == 1000000 && r.accepted == 1 && r.rejected == 1);
        }
        {
            aim::q4::CommandResult r = aim::q4::execute({"add 5", ""});
            CHECK(r.value == 0 && r.accepted == 0 && r.rejected == 2);
        }
        {
            aim::q4::CommandResult r = aim::q4::execute({"ADD +2", "RESET"});
            CHECK(r.value == 0 && r.accepted == 2 && r.rejected == 0);
        }
        {
            aim::q4::CommandResult r = aim::q4::execute({"ADD 5", "BOOM", "ADD 3"});
            CHECK(r.value == 8 && r.accepted == 2 && r.rejected == 1);   // 失败后继续
        }
        {
            aim::q4::CommandResult r = aim::q4::execute({"  ADD 5  ", "SET 1000000"});
            CHECK(r.value == 1000000 && r.accepted == 2 && r.rejected == 0);  // 首尾空白
        }
        {
            aim::q4::CommandResult r = aim::q4::execute({"ADD 5.0", "ADD", "RESET 0", "ADD 12x",
                                                         "ADD 2147483648", "ADD 99999999999999999999"});
            CHECK(r.value == 0 && r.accepted == 0 && r.rejected == 6);   // 各类非法
        }
        {
            aim::q4::CommandResult r = aim::q4::execute({"ADD -1000000", "ADD -1"});
            CHECK(r.value == -1000000 && r.accepted == 1 && r.rejected == 1);  // 候选下界
        }
        {
            aim::q4::CommandResult r = aim::q4::execute({"SET -1000000", "ADD 1000000"});
            CHECK(r.value == 0 && r.accepted == 2 && r.rejected == 0);
        }

        std::cout << "LOCAL BOUNDARY ALL PASS\n";
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}
