#include "test.h"
#include "graph/functional-forest.cpp"

int main() {
    for (int it = 0; it < 500; it++) {
        int n = (int)test::rnd(1, 15);
        vector<int> f(n);
        for (auto &x : f) x = (int)test::rnd(0, n - 1);
        FunctionalGraph fg(f);
        for (int v = 0; v < n; v++) {
            // walk n steps to land on the cycle, then collect it
            int x = v;
            for (int i = 0; i < n; i++) x = f[x];
            set<int> cyc;
            for (int y = x;; y = f[y]) {
                if (cyc.count(y)) break;
                cyc.insert(y);
            }
            CHECK_EQ((bool)fg.onCycle[v], cyc.count(v) > 0);
            int d = 0, y = v;
            while (!cyc.count(y)) y = f[y], d++;
            CHECK_EQ(fg.depth[v], d);
            CHECK_EQ(fg.entry[v], y);
            CHECK_EQ(fg.cycleId[v], fg.cycleId[x]);
            auto &c = fg.cycles[fg.cycleId[v]];
            CHECK_EQ(set<int>(c.begin(), c.end()), cyc);
            for (size_t i = 0; i < c.size(); i++) {
                CHECK_EQ(f[c[i]], c[(i + 1) % c.size()]);
                CHECK_EQ(fg.pos[c[i]], (int)i);
            }
            for (int k = 0; k < 40; k++) {
                int z = v;
                for (int i = 0; i < k; i++) z = f[z];
                CHECK_EQ(fg.jump(v, k), z);
            }
            // huge k: reduce by the cycle length after reaching the cycle
            long long k = (long long)1e18 + test::rnd(0, 1000);
            long long rem = (k - d) % (long long)c.size();
            int z = fg.entry[v];
            for (long long i = 0; i < rem; i++) z = f[z];
            CHECK_EQ(fg.jump(v, k), z);
        }
        int total = 0;
        for (auto &c : fg.cycles) total += (int)c.size();
        CHECK_EQ(total, (int)count(fg.onCycle.begin(), fg.onCycle.end(), 1));
    }
}
