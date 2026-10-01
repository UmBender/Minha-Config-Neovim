// Title: Directed MST
// Description: Minimum spanning arborescence (Chu-Liu/Edmonds) with the chosen edge of each vertex.
// Usage:
//   auto res = directedMST(n, root, edges);   // edges: vector<tuple<int, int, T>> (from, to, weight)
//   if (res) { auto [cost, par] = *res; }     // par[v] = id of the edge entering v, par[root] = -1
//   nullopt if some vertex is unreachable from root; negative weights OK
// Complexity: O(n m).
// Verify: https://judge.yosupo.jp/problem/directedmst
template <class T> optional<pair<T, vector<int>>> directedMST(int n, int root, const vector<tuple<int, int, T>> &edges) {
    struct E {
        int u, v;
        T w;
        int prev;  // index in the previous level's edge list
    };
    // returns, per vertex of this level, the index of its chosen edge (-1 for the root)
    auto solve = [&](auto self, int N, int r, const vector<E> &es) -> optional<vector<int>> {
        vector<int> in(N, -1);
        for (int i = 0; i < (int)es.size(); i++) {
            auto &e = es[i];
            if (e.u != e.v && e.v != r && (in[e.v] == -1 || e.w < es[in[e.v]].w)) in[e.v] = i;
        }
        for (int v = 0; v < N; v++)
            if (v != r && in[v] == -1) return nullopt;
        // contract the cycles formed by the cheapest incoming edges
        vector<int> comp(N, -1), seen(N, -1);
        int cnt = 0;
        for (int v = 0; v < N; v++) {
            int x = v;
            while (x != r && seen[x] == -1 && comp[x] == -1) seen[x] = v, x = es[in[x]].u;
            if (x != r && seen[x] == v && comp[x] == -1) {
                for (int y = es[in[x]].u; y != x; y = es[in[y]].u) comp[y] = cnt;
                comp[x] = cnt++;
            }
        }
        if (cnt == 0) return in;
        for (int v = 0; v < N; v++)
            if (comp[v] == -1) comp[v] = cnt++;
        vector<E> next;
        for (int i = 0; i < (int)es.size(); i++) {
            auto &e = es[i];
            if (comp[e.u] != comp[e.v]) next.push_back({comp[e.u], comp[e.v], e.w - (e.v == r ? T{} : es[in[e.v]].w), i});
        }
        auto sub = self(self, cnt, comp[r], next);
        if (!sub) return nullopt;
        // keep the cycle edges, except into the vertex where the cycle is entered
        for (int c = 0; c < cnt; c++)
            if ((*sub)[c] != -1) {
                int i = next[(*sub)[c]].prev;
                in[es[i].v] = i;
            }
        return in;
    };
    vector<E> es;
    for (int i = 0; i < (int)edges.size(); i++) {
        auto [u, v, w] = edges[i];
        es.push_back({u, v, w, i});
    }
    auto par = solve(solve, n, root, es);
    if (!par) return nullopt;
    T cost{};
    for (int v = 0; v < n; v++)
        if (v != root) cost += get<2>(edges[(*par)[v]]);
    return pair{cost, *par};
}
