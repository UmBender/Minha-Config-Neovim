#include "test.h"
#include "dsa/suffix-max.suffix-min.cpp"

int main() {
    for (int it = 0; it < 300; it++) {
        SuffixMin s;
        static_assert(is_same_v<decltype(s), SuffixMin<long long, long long>>);
        vector<pair<long long, long long>> pts;
        for (int q = 0; q < 100; q++) {
            if (test::rnd(0, 1)) {
                long long x = test::rnd(-20, 20), y = test::rnd(-1e12, 1e12);
                if (test::rnd(0, 3) == 0) y = test::rnd(-3, 3);  // ties
                s.add(x, y), pts.push_back({x, y});
            } else {
                long long x = test::rnd(-25, 25), want = LLONG_MAX;
                for (auto [px, py] : pts)
                    if (px >= x) want = min(want, py);
                CHECK_EQ(s.query(x), want);
            }
        }
    }
    SuffixMin<string, int> named(-1);
    named.add("b", 3), named.add("d", 1);
    CHECK_EQ(named.query("e"), -1);
    SuffixMin<int, double> d;
    CHECK_EQ(d.query(0), numeric_limits<double>::max());
}
