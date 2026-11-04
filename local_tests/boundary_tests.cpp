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
#define main local_main_q5
#include "../src/main/A1_q5.cpp"
#undef main
#define main local_main_q6
#include "../src/main/A1_q6.cpp"
#undef main
#define main local_main_q7
#include "../src/main/A1_q7.cpp"
#undef main
#define main local_main_q8
#include "../src/main/A1_q8.cpp"
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

        // ---- Q5 网格邻域风险图 ----
        CHECK(risk({}).empty());
        EXPECT_INVALID(risk({""}));
        CHECK((risk({".", "#"}) == std::vector<std::vector<int>>{{1}, {-1}}));   // 单列
        CHECK((risk({"#.", ".."}) == std::vector<std::vector<int>>{{-1, 1}, {1, 0}}));  // 对角不计
        CHECK((risk({"."}) == std::vector<std::vector<int>>{{0}}));               // 单格无邻居
        CHECK((risk({"#"}) == std::vector<std::vector<int>>{{-1}}));
        CHECK((risk({".#.", "#.#", ".#."}) ==
               std::vector<std::vector<int>>{{2, -1, 2}, {-1, 4, -1}, {2, -1, 2}}));  // 中心 4
        EXPECT_INVALID(risk({"..", "."}));
        EXPECT_INVALID(risk({"..x"}));                       // 末字符非法也要拒绝
        EXPECT_INVALID(risk({"#.", "x"}));                   // 先整图验证再计算

        // ---- Q6 目标优先级选择 ----
        CHECK(select_target({{9, 0, 0, 3, true}, {1, 0, 0, 3, true}}, 0) == 1);  // 平局按 id
        CHECK(select_target({{1, 3, 4, 1, true}}, 5) == 1);                      // 射程含边界
        CHECK(select_target({{1, 0, 0, 0, true}}, 1) == -1);                     // hp=0
        CHECK(select_target({{1, 0, 0, -5, true}}, 1) == -1);                    // hp<0
        CHECK(select_target({}, 5) == -1);
        CHECK(select_target({{1, 0, 0, 5, true}}, -1) == -1);                    // 负射程
        CHECK(select_target({{1, 0, 0, 5, false}}, 5) == -1);                    // 不可见
        CHECK(select_target({{7, 3, 4, 1, true}, {2, 0, 0, 2, true}}, 5) == 7);  // 血量优先于距离
        CHECK(select_target({{1, 1, 0, 5, true}}, 0) == -1);                     // 超射程
        CHECK(select_target({{1, 1000000, 0, 5, true}}, 1000000) == 1);          // 边界平方 10^12
        CHECK(select_target({{1, 1000000, 1000000, 5, true}}, 1000000) == -1);   // 2*10^12 超界
        CHECK(select_target({{5, 4, 3, 7, true}, {1, 3, 4, 7, true}, {9, 0, 0, 8, true}}, 10) == 1);

        // ---- Q7 热量与弹仓仿真 ----
        {
            aim::q7::FireResult r = aim::q7::simulate(4, 80, 10, 30, {});
            CHECK(r.shots == 0 && r.rejected == 0 && r.ammo == 4 && r.heat == 80);  // 空序列不冷却
        }
        {
            aim::q7::FireResult r = aim::q7::simulate(0, 50, 10, 20, {1});
            CHECK(r.shots == 0 && r.rejected == 1 && r.ammo == 0 && r.heat == 40);  // 无弹先冷却
        }
        {
            aim::q7::FireResult r = aim::q7::simulate(1, 100, 0, 0, {1});
            CHECK(r.shots == 1 && r.rejected == 0 && r.ammo == 0 && r.heat == 100);  // 零射击热量
        }
        {
            aim::q7::FireResult r = aim::q7::simulate(1, 100, 0, 1, {1});
            CHECK(r.shots == 0 && r.rejected == 1 && r.ammo == 1 && r.heat == 100);  // 拒绝不扣弹
        }
        {
            aim::q7::FireResult r = aim::q7::simulate(1, 80, 0, 20, {1});
            CHECK(r.shots == 1 && r.heat == 100 && r.ammo == 0);   // 恰到 100 允许
        }
        {
            aim::q7::FireResult r = aim::q7::simulate(1, 80, 0, 21, {1});
            CHECK(r.shots == 0 && r.rejected == 1 && r.heat == 80);  // 101 拒绝
        }
        {
            aim::q7::FireResult r = aim::q7::simulate(1, 5, 10, 0, {0});
            CHECK(r.heat == 0 && r.ammo == 1);   // 冷却负值归零
        }
        {
            aim::q7::FireResult r = aim::q7::simulate(2, 90, 10, 20, {1, 1, 1});
            CHECK(r.shots == 2 && r.rejected == 1 && r.ammo == 0 && r.heat == 100);
        }

        // ---- Q8 遥测帧综合流水线 ----
        {
            aim::q8::Summary s = aim::q8::summarize({{1, -1001}, {1, 30}});
            CHECK(s.valid == 1 && s.duplicate == 0 && s.invalid == 1 && s.total_hp == 30 &&
                  (s.ids == std::vector<int>{1}));   // 非法不占编号
        }
        {
            aim::q8::Summary s = aim::q8::summarize({{1, 20}, {1, -1001}});
            CHECK(s.valid == 1 && s.duplicate == 0 && s.invalid == 1 && s.total_hp == 20);  // 先合法再重复
        }
        {
            aim::q8::Summary s = aim::q8::summarize({{2, 5}, {1, 9}});
            CHECK(s.total_hp == 14 && (s.ids == std::vector<int>{1, 2}));  // 编号排序
        }
        {
            aim::q8::Summary s = aim::q8::summarize({});
            CHECK(s.valid == 0 && s.duplicate == 0 && s.invalid == 0 && s.total_hp == 0 && s.ids.empty());
        }
        {
            aim::q8::Summary s = aim::q8::summarize({{0, 1000}, {9999, -1000}, {5, 200}, {5, 10},
                                                     {-1, 30}, {10000, 30}, {7, 1001}, {7, 50}});
            // {7,1001} 非法不占编号，随后的 {7,50} 仍是首次合法记录。
            CHECK(s.valid == 4 && s.duplicate == 1 && s.invalid == 3);
            CHECK(s.total_hp == 100 + 0 + 100 + 50 && (s.ids == std::vector<int>{0, 5, 7, 9999}));
            CHECK(s.valid + s.duplicate + s.invalid == 8);   // 数据守恒
            CHECK(s.valid == static_cast<int>(s.ids.size()));
        }
        {
            aim::q8::Summary s = aim::q8::summarize({{1, 5}});
            CHECK(s.total_hp == 5);   // 与上一批独立
        }

        std::cout << "LOCAL BOUNDARY ALL PASS\n";
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}
