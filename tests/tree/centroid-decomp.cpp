#include "test.h"
#include "tree/centroid-decomp.cpp"

// checks the centroid tree of the forest g
static void verify(const vector<vector<int>> &g, const CentroidTree &ct) {
    int n = (int)g.size();
    CHECK_EQ((int)ct.par.size(), n);
    CHECK_EQ((int)ct.level.size(), n);
    vector<vector<int>> kids(n);
    int roots = 0;
    for (int v = 0; v < n; v++) {
        if (ct.par[v] == -1) roots++, CHECK_EQ(ct.level[v], 0);
        else kids[ct.par[v]].push_back(v), CHECK_EQ(ct.level[v], ct.level[ct.par[v]] + 1);
        CHECK((1 << ct.level[v]) <= n);
    }
    // one root per connected component of g
    vector<int> comp(n, -1);
    int comps = 0;
    for (int s = 0; s < n; s++) {
        if (comp[s] != -1) continue;
        vector<int> st{s};
        comp[s] = comps;
        while (!st.empty()) {
            int v = st.back();
            st.pop_back();
            for (int u : g[v])
                if (comp[u] == -1) comp[u] = comps, st.push_back(u);
        }
        comps++;
    }
    CHECK_EQ(roots, comps);
    // S(v) = centroid subtree of v: removing v from S(v) leaves exactly the S(c) of its children
    // (components of the original edges), each with at most |S(v)| / 2 vertices
    vector<int> owner(n);
    for (int v = 0; v < n; v++) {
        vector<int> sub{v};
        for (int i = 0; i < (int)sub.size(); i++)
            for (int c : kids[sub[i]]) sub.push_back(c);
        set<int> in(sub.begin(), sub.end());
        for (int c : kids[v]) {
            vector<int> sc{c};
            for (int i = 0; i < (int)sc.size(); i++)
                for (int d : kids[sc[i]]) sc.push_back(d);
            CHECK(2 * sc.size() <= sub.size());
            set<int> want(sc.begin(), sc.end()), got{c};
            vector<int> st{c};
            while (!st.empty()) {
                int x = st.back();
                st.pop_back();
                for (int u : g[x])
                    if (u != v && in.count(u) && got.insert(u).second) st.push_back(u);
            }
            CHECK_EQ(got, want);
        }
    }
}

int main() {
    CentroidTree one(vector<vector<int>>(1));
    CHECK_EQ(one.par, vector<int>{-1});
    for (int it = 0; it < 800; it++) {
        int n = (int)test::rnd(1, 30);
        auto t = test::rndTree(n);
        if (test::rnd(0, 1)) {
            // forest: drop some edges
            vector<vector<int>> f(n);
            for (int v = 0; v < n; v++)
                for (int u : t[v])
                    if (u < v && test::rnd(0, 4)) f[u].push_back(v), f[v].push_back(u);
            t = f;
        }
        verify(t, CentroidTree(t));
    }
    // long path: no recursion, depth of the centroid tree is logarithmic
    int n = 200000;
    vector<vector<int>> g(n);
    for (int i = 0; i + 1 < n; i++) g[i].push_back(i + 1), g[i + 1].push_back(i);
    CentroidTree ct(g);
    CHECK_EQ(*max_element(ct.level.begin(), ct.level.end()), __lg(n));
}
