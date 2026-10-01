#include "test.h"
#include "dsa/dsu.cpp"

int main() {
    for (int it = 0; it < 300; it++) {
        int n = (int)test::rnd(1, 30);
        DSU dsu(n);
        vector<int> comp(n);
        iota(comp.begin(), comp.end(), 0);
        for (int q = 0; q < 100; q++) {
            int a = (int)test::rnd(0, n - 1), b = (int)test::rnd(0, n - 1);
            if (test::rnd(0, 1)) {
                bool want = comp[a] != comp[b];
                CHECK_EQ(dsu.unite(a, b), want);
                int from = comp[b], to = comp[a];
                for (int &c : comp) if (c == from) c = to;
            } else {
                CHECK_EQ(dsu.same(a, b), comp[a] == comp[b]);
                CHECK_EQ(dsu.size(a), (int)count(comp.begin(), comp.end(), comp[a]));
                CHECK_EQ(dsu.find(a), dsu.find(dsu.find(a)));
            }
            CHECK_EQ(dsu.count(), (int)set<int>(comp.begin(), comp.end()).size());
        }
        // groups: every element exactly once, grouped by component
        auto groups = dsu.groups();
        CHECK_EQ((int)groups.size(), dsu.count());
        vector<int> seen(n, 0);
        for (auto &g : groups) {
            CHECK(!g.empty());
            for (int v : g) seen[v]++, CHECK(dsu.same(v, g[0]));
            CHECK_EQ((int)g.size(), dsu.size(g[0]));
        }
        CHECK(all_of(seen.begin(), seen.end(), [](int c) { return c == 1; }));
    }
    // deep chains don't overflow the stack (iterative find)
    int n = 300000;
    DSU big(n);
    for (int i = 1; i < n; i++) big.unite(i - 1, i);
    CHECK_EQ(big.size(0), n);
    CHECK_EQ(big.count(), 1);
}
