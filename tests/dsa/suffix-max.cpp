#include "test.h"
#include "dsa/suffix-max.cpp"

int main() {
    for (int it = 0; it < 300; it++) {
        SuffixMax<int, long long> mx(LLONG_MIN);
        SuffixMax<int, long long, greater<long long>> mn(LLONG_MAX);
        vector<pair<int, long long>> pts;
        for (int q = 0; q < 100; q++) {
            if (test::rnd(0, 1)) {
                int x = (int)test::rnd(-20, 20);
                long long y = test::rnd(-100, 100);
                mx.add(x, y), mn.add(x, y), pts.push_back({x, y});
            } else {
                int x = (int)test::rnd(-25, 25);
                long long wantMax = LLONG_MIN, wantMin = LLONG_MAX;
                for (auto [px, py] : pts)
                    if (px >= x) wantMax = max(wantMax, py), wantMin = min(wantMin, py);
                CHECK_EQ(mx.query(x), wantMax);
                CHECK_EQ(mn.query(x), wantMin);
            }
        }
    }

    // presets: defaults (long long keys and values, identity from the comparator) and
    // SuffixMin / PrefixMax / PrefixMin
    for (int it = 0; it < 300; it++) {
        SuffixMax sMax;
        SuffixMin sMin;
        PrefixMax pMax;
        PrefixMin pMin;
        static_assert(is_same_v<decltype(sMax), SuffixMax<long long, long long>>);
        vector<pair<long long, long long>> pts;
        for (int q = 0; q < 100; q++) {
            if (test::rnd(0, 1)) {
                long long x = test::rnd(-20, 20), y = test::rnd(-1e12, 1e12);
                sMax.add(x, y), sMin.add(x, y), pMax.add(x, y), pMin.add(x, y), pts.push_back({x, y});
            } else {
                long long x = test::rnd(-25, 25);
                long long wsMax = LLONG_MIN, wsMin = LLONG_MAX, wpMax = LLONG_MIN, wpMin = LLONG_MAX;
                for (auto [px, py] : pts) {
                    if (px >= x) wsMax = max(wsMax, py), wsMin = min(wsMin, py);
                    if (px <= x) wpMax = max(wpMax, py), wpMin = min(wpMin, py);
                }
                CHECK_EQ(sMax.query(x), wsMax);
                CHECK_EQ(sMin.query(x), wsMin);
                CHECK_EQ(pMax.query(x), wpMax);
                CHECK_EQ(pMin.query(x), wpMin);
            }
        }
    }
    // other key types, explicit identity still works
    PrefixMax<string, int> ps(-1);
    ps.add("b", 3), ps.add("d", 1);
    CHECK_EQ(ps.query("a"), -1);
    CHECK_EQ(ps.query("c"), 3);
    CHECK_EQ(ps.query("z"), 3);
    SuffixMin<int, double> sd;
    CHECK_EQ(sd.query(0), numeric_limits<double>::max());
}
