#include "test.h"
#include "graph/havel-hakimi.cpp"

bool erdosGallai(vector<int> d) {
    long long sum = 0;
    for (int x : d) {
        if (x < 0 || x >= (int)d.size() + (d.empty() ? 1 : 0)) return x == 0 && d.size() == 1 ? true : false;
        sum += x;
    }
    if (sum % 2) return false;
    sort(d.rbegin(), d.rend());
    int n = (int)d.size();
    long long left = 0;
    for (int k = 1; k <= n; k++) {
        left += d[k - 1];
        long long right = (long long)k * (k - 1);
        for (int i = k; i < n; i++) right += min(d[i], k);
        if (left > right) return false;
    }
    return true;
}

int main() {
    CHECK(havelHakimi({}).has_value());
    CHECK(!havelHakimi({1}).has_value());
    CHECK(!havelHakimi({3, 1, 1}).has_value());
    for (int it = 0; it < 3000; it++) {
        int n = (int)test::rnd(1, 9);
        vector<int> d = test::rndVec<int>(n, 0, n - 1);
        if (test::rnd(0, 20) == 0) d[0] = n;  // impossible degree
        auto res = havelHakimi(d);
        CHECK_EQ(res.has_value(), erdosGallai(d));
        if (!res) continue;
        vector<int> got(n, 0);
        set<pair<int, int>> seen;
        for (auto [u, v] : *res) {
            CHECK(u != v);
            CHECK(seen.insert({min(u, v), max(u, v)}).second);  // simple graph
            got[u]++, got[v]++;
        }
        CHECK_EQ(got, d);
    }
}
