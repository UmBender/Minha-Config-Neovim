#include "test.h"
#include "dsa/mo-queries.cpp"

int main() {
    // order is a permutation
    vector<pair<int, int>> qs = {{0, 3}, {2, 5}, {1, 1}, {0, 6}};
    auto ord = moOrder(qs, 6);
    sort(ord.begin(), ord.end());
    CHECK_EQ(ord, (vector<int>{0, 1, 2, 3}));
    CHECK(moOrder({}, 10).empty());

    // distinct values in [l, r), symmetric add/remove
    for (int it = 0; it < 200; it++) {
        int n = (int)test::rnd(1, 60), q = (int)test::rnd(0, 60);
        vector<int> a = test::rndVec<int>(n, 0, 10);
        vector<pair<int, int>> queries(q);
        for (auto &[l, r] : queries) l = (int)test::rnd(0, n), r = (int)test::rnd(l, n);
        vector<int> cnt(11, 0), ans(q, -1);
        int distinct = 0;
        mo(queries, n,
           [&](int i) { distinct += cnt[a[i]]++ == 0; },
           [&](int i) { distinct -= --cnt[a[i]] == 0; },
           [&](int qi) { ans[qi] = distinct; });
        for (int i = 0; i < q; i++) {
            auto [l, r] = queries[i];
            CHECK_EQ(ans[i], (int)set<int>(a.begin() + l, a.begin() + r).size());
        }
    }

    // order-sensitive: build the window as a deque with separate left/right callbacks
    for (int it = 0; it < 200; it++) {
        int n = (int)test::rnd(1, 40), q = (int)test::rnd(1, 40);
        vector<int> a = test::rndVec<int>(n, 0, 9);
        vector<pair<int, int>> queries(q);
        for (auto &[l, r] : queries) l = (int)test::rnd(0, n), r = (int)test::rnd(l, n);
        deque<int> window;
        vector<vector<int>> ans(q);
        mo(queries, n,
           [&](int i) { window.push_front(a[i]); },  // add on the left
           [&](int i) { window.push_back(a[i]); },   // add on the right
           [&](int) { window.pop_front(); },         // remove on the left
           [&](int) { window.pop_back(); },          // remove on the right
           [&](int qi) { ans[qi] = vector<int>(window.begin(), window.end()); });
        for (int i = 0; i < q; i++) {
            auto [l, r] = queries[i];
            CHECK_EQ(ans[i], vector<int>(a.begin() + l, a.begin() + r));
        }
    }
}
