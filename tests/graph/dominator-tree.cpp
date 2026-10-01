#include "test.h"
#include "graph/dominator-tree.cpp"

int main() {
    for (int it = 0; it < 500; it++) {
        int n = (int)test::rnd(1, 9), m = (int)test::rnd(0, 20), s = (int)test::rnd(0, n - 1);
        vector<vector<int>> g(n);
        for (int e = 0; e < m; e++) g[test::rnd(0, n - 1)].push_back((int)test::rnd(0, n - 1));
        auto reach = [&](int banned) {
            vector<char> vis(n, 0);
            if (banned == s) return vis;
            vector<int> st = {s};
            vis[s] = 1;
            while (!st.empty()) {
                int v = st.back();
                st.pop_back();
                for (int u : g[v])
                    if (u != banned && !vis[u]) vis[u] = 1, st.push_back(u);
            }
            return vis;
        };
        auto all = reach(-1);
        // dom[w] = set of vertices whose removal disconnects w (plus w itself)
        vector<set<int>> dom(n);
        for (int d = 0; d < n; d++) {
            auto r = reach(d);
            for (int w = 0; w < n; w++)
                if (all[w] && (!r[w] || d == w)) dom[w].insert(d);
        }
        auto idom = dominatorTree(g, s);
        for (int w = 0; w < n; w++) {
            if (!all[w]) {
                CHECK_EQ(idom[w], -1);
                continue;
            }
            if (w == s) {
                CHECK_EQ(idom[w], s);
                continue;
            }
            // the immediate dominator is the strict dominator dominated by all the others
            set<int> strict = dom[w];
            strict.erase(w);
            int want = -1;
            for (int d : strict) {
                set<int> sd = dom[d];
                if (sd == strict) want = d;
            }
            CHECK_EQ(idom[w], want);
        }
    }
}
