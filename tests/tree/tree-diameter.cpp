#include "test.h"
#include "tree/tree-diameter.cpp"

// weighted distances from s (tree: any traversal order works)
static vector<long long> distFrom(const vector<vector<pair<int, long long>>> &g, int s) {
    vector<long long> d(g.size(), -1);
    vector<int> st{s};
    d[s] = 0;
    while (!st.empty()) {
        int v = st.back();
        st.pop_back();
        for (auto [u, w] : g[v])
            if (d[u] == -1) d[u] = d[v] + w, st.push_back(u);
    }
    return d;
}

static long long weight(const vector<vector<pair<int, long long>>> &g, int a, int b) {
    for (auto [u, w] : g[a])
        if (u == b) return w;
    test::fail(__FILE__, __LINE__, "consecutive path vertices are not adjacent");
}

// checks that p is a simple path of g and returns its length
static long long pathLength(const vector<vector<pair<int, long long>>> &g, const vector<int> &p) {
    CHECK(!p.empty());
    set<int> distinct(p.begin(), p.end());
    CHECK_EQ(distinct.size(), p.size());
    long long len = 0;
    for (int i = 0; i + 1 < (int)p.size(); i++) len += weight(g, p[i], p[i + 1]);
    return len;
}

int main() {
    CHECK_EQ(treeDiameter(vector<vector<int>>(1)), vector<int>{0});
    CHECK_EQ(treeDiameter(vector<vector<pair<int, long long>>>(1)), make_pair(0LL, vector<int>{0}));
    for (int it = 0; it < 1000; it++) {
        int n = (int)test::rnd(1, 25);
        auto g = test::rndTree(n);
        // unweighted
        vector<vector<pair<int, long long>>> wg(n), ug(n);
        for (int v = 0; v < n; v++)
            for (int u : g[v]) ug[v].push_back({u, 1});
        long long best = 0;
        for (int s = 0; s < n; s++) {
            auto d = distFrom(ug, s);
            best = max(best, *max_element(d.begin(), d.end()));
        }
        auto p = treeDiameter(g);
        CHECK_EQ(pathLength(ug, p), best);
        // weighted, zero weights included
        for (int v = 0; v < n; v++)
            for (int u : g[v])
                if (u < v) {
                    long long w = test::rnd(0, 1) ? test::rnd(0, 3) : test::rnd(0, 1000000000000LL);
                    wg[v].push_back({u, w}), wg[u].push_back({v, w});
                }
        best = 0;
        for (int s = 0; s < n; s++) {
            auto d = distFrom(wg, s);
            best = max(best, *max_element(d.begin(), d.end()));
        }
        auto [len, wp] = treeDiameter(wg);
        CHECK_EQ(len, best);
        CHECK_EQ(pathLength(wg, wp), best);
    }
    // double weights
    vector<vector<pair<int, double>>> dg(3);
    dg[0] = {{1, 0.5}}, dg[1] = {{0, 0.5}, {2, 1.25}}, dg[2] = {{1, 1.25}};
    CHECK_NEAR(treeDiameter(dg).first, 1.75, 1e-12);
    // long path: no recursion
    int n = 200000;
    vector<vector<int>> g(n);
    for (int i = 0; i + 1 < n; i++) g[i].push_back(i + 1), g[i + 1].push_back(i);
    CHECK_EQ((int)treeDiameter(g).size(), n);
}
