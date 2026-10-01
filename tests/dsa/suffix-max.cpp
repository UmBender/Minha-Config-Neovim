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
}
