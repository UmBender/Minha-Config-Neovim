#include "test.h"
#include "dsa/offline-deletions.connectivity.cpp"

// brute force: plain DSU over the alive edges
struct Plain {
    vector<int> p;
    int comps;
    Plain(int n) : p(n), comps(n) { iota(p.begin(), p.end(), 0); }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    void unite(int a, int b) {
        a = find(a), b = find(b);
        if (a != b) p[a] = b, comps--;
    }
};

int main() {
    for (int it = 0; it < 300; it++) {
        int n = (int)test::rnd(1, 12);
        DynamicConnectivity dc(n);
        multiset<pair<int, int>> alive;  // normalized a <= b
        vector<int> wantCount, wantSame;
        for (int op = 0; op < 60; op++) {
            int type = (int)test::rnd(0, 2);
            int a = (int)test::rnd(0, n - 1), b = (int)test::rnd(0, n - 1);
            if (type == 0) {
                dc.addEdge(a, b), alive.insert(minmax(a, b));
            } else if (type == 1) {
                if (alive.empty() || test::rnd(0, 4) == 0) {
                    dc.removeEdge(a, b);  // may be absent: ignored
                    if (alive.count(minmax(a, b))) alive.erase(alive.find(minmax(a, b)));
                } else {
                    auto e = *next(alive.begin(), test::rnd(0, (long long)alive.size() - 1));
                    if (test::rnd(0, 1)) dc.removeEdge(e.second, e.first);  // either orientation
                    else dc.removeEdge(e.first, e.second);
                    alive.erase(alive.find(e));
                }
            } else {
                Plain d(n);
                for (auto [x, y] : alive) d.unite(x, y);
                wantCount.push_back(d.comps), wantSame.push_back(d.find(a) == d.find(b));
                dc.query(a, b);
            }
        }
        dc.run();
        CHECK_EQ(dc.comps, wantCount);
        CHECK_EQ(dc.connected, wantSame);
    }
    DynamicConnectivity none(3);
    none.run();
    CHECK(none.comps.empty());
}
