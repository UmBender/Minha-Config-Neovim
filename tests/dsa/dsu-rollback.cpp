#include "test.h"
#include "dsa/dsu-rollback.cpp"

int main() {
    for (int it = 0; it < 300; it++) {
        int n = (int)test::rnd(1, 20);
        RollbackDSU dsu(n);
        // brute: list of applied edges; components recomputed from scratch
        vector<pair<int, int>> edges;
        auto comps = [&]() {
            vector<int> c(n);
            iota(c.begin(), c.end(), 0);
            for (int round = 0; round < n; round++)
                for (auto [a, b] : edges) c[a] = c[b] = min(c[a], c[b]);
            return c;
        };
        vector<int> snapshots;
        for (int q = 0; q < 80; q++) {
            int type = (int)test::rnd(0, 3);
            if (type == 0) {
                int a = (int)test::rnd(0, n - 1), b = (int)test::rnd(0, n - 1);
                auto c = comps();
                CHECK_EQ(dsu.unite(a, b), c[a] != c[b]);
                edges.push_back({a, b});
            } else if (type == 1 && !edges.empty()) {
                dsu.undo();
                edges.pop_back();
            } else if (type == 2) {
                snapshots.push_back(dsu.time());
                CHECK_EQ(dsu.time(), (int)edges.size());
            } else if (type == 3 && !snapshots.empty()) {
                int t = snapshots.back();
                snapshots.pop_back();
                if (t <= dsu.time()) {
                    dsu.rollback(t);
                    edges.resize(t);
                }
            }
            auto c = comps();
            for (int a = 0; a < n; a++) {
                int b = (int)test::rnd(0, n - 1);
                CHECK_EQ(dsu.same(a, b), c[a] == c[b]);
                CHECK_EQ(dsu.size(a), (int)count(c.begin(), c.end(), c[a]));
            }
            CHECK_EQ(dsu.count(), (int)set<int>(c.begin(), c.end()).size());
        }
    }
}
