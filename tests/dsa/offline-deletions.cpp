#include "test.h"
#include "dsa/dsu-rollback.cpp"
#include "dsa/offline-deletions.cpp"

int main() {
    // dynamic connectivity: number of components at each query
    for (int it = 0; it < 200; it++) {
        int n = (int)test::rnd(1, 12);
        OfflineDeletion<pair<int, int>> od;
        multiset<pair<int, int>> alive;
        vector<int> want;
        for (int op = 0; op < 60; op++) {
            int type = (int)test::rnd(0, 2);
            if (type == 0) {
                int a = (int)test::rnd(0, n - 1), b = (int)test::rnd(0, n - 1);
                if (a > b) swap(a, b);
                od.insert({a, b}), alive.insert({a, b});  // duplicates allowed
            } else if (type == 1 && !alive.empty()) {
                auto e = *next(alive.begin(), test::rnd(0, (long long)alive.size() - 1));
                od.remove(e), alive.erase(alive.find(e));
            } else {
                RollbackDSU d(n);
                for (auto [a, b] : alive) d.unite(a, b);
                want.push_back(d.count());
                od.query();
            }
        }
        RollbackDSU dsu(n);
        vector<int> got(want.size(), -1);
        od.run([&](pair<int, int> e) { dsu.unite(e.first, e.second); },
               [&]() { dsu.undo(); },
               [&](int qi) { got[qi] = dsu.count(); });
        CHECK_EQ(got, want);
        CHECK_EQ(dsu.time(), 0);
    }

    // multiset sum with a plain undo stack
    for (int it = 0; it < 200; it++) {
        OfflineDeletion<int> od;
        multiset<int> alive;
        vector<long long> want;
        for (int op = 0; op < 80; op++) {
            int type = (int)test::rnd(0, 2);
            if (type == 0) {
                int x = (int)test::rnd(1, 5);
                od.insert(x), alive.insert(x);
            } else if (type == 1 && !alive.empty()) {
                int x = *next(alive.begin(), test::rnd(0, (long long)alive.size() - 1));
                od.remove(x), alive.erase(alive.find(x));
            } else {
                want.push_back(accumulate(alive.begin(), alive.end(), 0LL));
                od.query();
            }
        }
        long long sum = 0;
        vector<int> stk;
        vector<long long> got(want.size());
        od.run([&](int x) { sum += x, stk.push_back(x); },
               [&]() { sum -= stk.back(), stk.pop_back(); },
               [&](int qi) { got[qi] = sum; });
        CHECK_EQ(got, want);
    }

    // presets: dynamic connectivity (component counts / connectivity of a pair per query)
    for (int it = 0; it < 200; it++) {
        int n = (int)test::rnd(1, 12);
        OfflineDeletion<pair<int, int>> od;
        multiset<pair<int, int>> alive;
        vector<pair<int, int>> ask;
        vector<int> wantCount, wantSame;
        for (int op = 0; op < 60; op++) {
            int type = (int)test::rnd(0, 2);
            if (type == 0) {
                int a = (int)test::rnd(0, n - 1), b = (int)test::rnd(0, n - 1);
                od.insert({a, b}), alive.insert({a, b});
            } else if (type == 1 && !alive.empty()) {
                auto e = *next(alive.begin(), test::rnd(0, (long long)alive.size() - 1));
                od.remove(e), alive.erase(alive.find(e));
            } else {
                RollbackDSU d(n);
                for (auto [a, b] : alive) d.unite(a, b);
                int a = (int)test::rnd(0, n - 1), b = (int)test::rnd(0, n - 1);
                wantCount.push_back(d.count()), wantSame.push_back(d.same(a, b));
                ask.push_back({a, b});
                od.query();
            }
        }
        OfflineDeletion<pair<int, int>> od2 = od;
        RollbackDSU dsu(n);
        CHECK_EQ(componentCounts(od, dsu), wantCount);
        CHECK_EQ(dsu.time(), 0);
        CHECK_EQ(connectedAt(od2, dsu, ask), wantSame);
        CHECK_EQ(dsu.time(), 0);
    }
}
