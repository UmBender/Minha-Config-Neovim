#include "test.h"
#include "graph/st-numbering.cpp"

// G + {s, t} biconnected (connected, no articulation point)?
bool biconnected(int n, const vector<pair<int, int>> &edges) {
    auto connectedWithout = [&](int banned) {
        vector<vector<int>> g(n);
        for (auto [u, v] : edges)
            if (u != banned && v != banned) g[u].push_back(v), g[v].push_back(u);
        int start = banned == 0 ? 1 : 0, cnt = 0;
        vector<char> vis(n, 0);
        vector<int> st = {start};
        vis[start] = 1;
        while (!st.empty()) {
            int v = st.back();
            st.pop_back(), cnt++;
            for (int u : g[v])
                if (!vis[u]) vis[u] = 1, st.push_back(u);
        }
        return cnt == n - (banned >= 0);
    };
    if (!connectedWithout(-1)) return false;
    if (n <= 2) return true;
    for (int v = 0; v < n; v++)
        if (!connectedWithout(v)) return false;
    return true;
}

int main() {
    for (int it = 0; it < 1500; it++) {
        int n = (int)test::rnd(2, 8), m = (int)test::rnd(0, 16);
        int s = (int)test::rnd(0, n - 1), t = (int)test::rnd(0, n - 2);
        if (t >= s) t++;
        vector<vector<int>> g(n);
        set<pair<int, int>> es;
        for (int e = 0; e < m; e++) {
            int u = (int)test::rnd(0, n - 1), v = (int)test::rnd(0, n - 1);
            if (u == v || es.count({min(u, v), max(u, v)})) continue;
            es.insert({min(u, v), max(u, v)});
            g[u].push_back(v), g[v].push_back(u);
        }
        vector<pair<int, int>> withST(es.begin(), es.end());
        withST.push_back({s, t});
        bool want = biconnected(n, withST);
        auto num = stNumbering(g, s, t);
        CHECK_EQ(!num.empty(), want);
        if (num.empty()) continue;
        CHECK_EQ(num[s], 0);
        CHECK_EQ(num[t], n - 1);
        vector<int> sorted = num;
        sort(sorted.begin(), sorted.end());
        for (int i = 0; i < n; i++) CHECK_EQ(sorted[i], i);
        for (int v = 0; v < n; v++) {
            if (v == s || v == t) continue;
            bool lower = false, higher = false;
            for (int u : g[v]) lower |= num[u] < num[v], higher |= num[u] > num[v];
            CHECK(lower && higher);
        }
    }
}
