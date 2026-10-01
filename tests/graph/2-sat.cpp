#include "test.h"
#include "graph/scc.cpp"
#include "graph/2-sat.cpp"

int main() {
    for (int it = 0; it < 500; it++) {
        int n = (int)test::rnd(1, 8);
        TwoSat ts(n);
        // constraints as predicates over an assignment, checked by brute force
        vector<function<bool(const vector<bool> &)>> cons;
        int k = (int)test::rnd(0, 12);
        for (int c = 0; c < k; c++) {
            int type = (int)test::rnd(0, 5);
            int a = (int)test::rnd(0, n - 1), b = (int)test::rnd(0, n - 1);
            bool va = test::rnd(0, 1), vb = test::rnd(0, 1);
            if (type == 0) {
                ts.either(a, va, b, vb);
                cons.push_back([=](const vector<bool> &x) { return x[a] == va || x[b] == vb; });
            } else if (type == 1) {
                ts.implies(a, va, b, vb);
                cons.push_back([=](const vector<bool> &x) { return x[a] != va || x[b] == vb; });
            } else if (type == 2) {
                ts.mustBe(a, va);
                cons.push_back([=](const vector<bool> &x) { return x[a] == va; });
            } else if (type == 3) {
                ts.equal(a, b);
                cons.push_back([=](const vector<bool> &x) { return x[a] == x[b]; });
            } else if (type == 4) {
                ts.different(a, b);
                cons.push_back([=](const vector<bool> &x) { return x[a] != x[b]; });
            } else {
                vector<pair<int, bool>> lits;
                int cnt = (int)test::rnd(0, 4);
                for (int j = 0; j < cnt; j++) lits.push_back({(int)test::rnd(0, n - 1), (bool)test::rnd(0, 1)});
                ts.atMostOne(lits);
                cons.push_back([=](const vector<bool> &x) {
                    int s = 0;
                    for (auto [v, val] : lits) s += x[v] == val;
                    return s <= 1;
                });
            }
        }
        bool want = false;
        for (int mask = 0; mask < (1 << n) && !want; mask++) {
            vector<bool> x(n);
            for (int i = 0; i < n; i++) x[i] = mask >> i & 1;
            want = all_of(cons.begin(), cons.end(), [&](auto &f) { return f(x); });
        }
        bool got = ts.solve();
        CHECK_EQ(got, want);
        if (got) {
            CHECK((int)ts.value.size() >= n);
            vector<bool> x(ts.value.begin(), ts.value.begin() + n);
            for (auto &f : cons) CHECK(f(x));
        }
    }
}
