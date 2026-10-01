#include "test.h"
#include "graph/hungarian.cpp"
using ll = long long;

int main() {
    for (int it = 0; it < 500; it++) {
        int n = (int)test::rnd(1, 6), m = (int)test::rnd(n, 7);
        vector<vector<ll>> a(n, vector<ll>(m));
        for (auto &row : a)
            for (auto &x : row) x = test::rnd(-1e9, 1e9);
        ll bestMin = LLONG_MAX, bestMax = LLONG_MIN;
        vector<int> used(m, 0);
        auto rec = [&](auto self, int i, ll s) -> void {
            if (i == n) {
                bestMin = min(bestMin, s), bestMax = max(bestMax, s);
                return;
            }
            for (int j = 0; j < m; j++)
                if (!used[j]) used[j] = 1, self(self, i + 1, s + a[i][j]), used[j] = 0;
        };
        rec(rec, 0, 0);
        for (int maximize = 0; maximize < 2; maximize++) {
            auto [cost, col] = maximize ? hungarianMax(a) : hungarian(a);
            CHECK_EQ(cost, maximize ? bestMax : bestMin);
            CHECK_EQ((int)col.size(), n);
            set<int> cols(col.begin(), col.end());
            CHECK_EQ((int)cols.size(), n);
            ll s = 0;
            for (int i = 0; i < n; i++) CHECK(0 <= col[i] && col[i] < m), s += a[i][col[i]];
            CHECK_EQ(s, cost);
        }
    }
    vector<vector<double>> d = {{1.5, 0.5}, {0.25, 3}};
    CHECK_NEAR(hungarian(d).first, 0.75, 1e-12);
}
