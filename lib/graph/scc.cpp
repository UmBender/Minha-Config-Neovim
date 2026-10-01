// Title: Strongly connected components
// Description: Tarjan SCC (iterative), components numbered in topological order, condensation DAG.
// Usage:
//   SCC s(g);                  // g: vector<vector<int>> adjacency list
//   s.count; s.comp[v]         // component of v, in [0, count)
//   every edge u -> v has s.comp[u] <= s.comp[v] (ids are a topological order of the DAG)
//   s.components()             // vertices of each component
//   s.condensation(g)          // DAG between components (sorted, no duplicates)
// Complexity: O(n + m).
// Verify: https://judge.yosupo.jp/problem/scc
struct SCC {
    int n, count = 0;
    vector<int> comp;
    SCC(const vector<vector<int>> &g) : n((int)g.size()), comp(n, -1) {
        vector<int> low(n), tin(n, -1), st, it(n, 0), call;
        int timer = 0;
        for (int s = 0; s < n; s++) {
            if (tin[s] != -1) continue;
            call.push_back(s);
            while (!call.empty()) {
                int v = call.back();
                if (it[v] == 0 && tin[v] == -1) tin[v] = low[v] = timer++, st.push_back(v);
                if (it[v] < (int)g[v].size()) {
                    int to = g[v][it[v]++];
                    if (tin[to] == -1) call.push_back(to);
                    else if (comp[to] == -1) low[v] = min(low[v], tin[to]);
                    continue;
                }
                call.pop_back();
                if (!call.empty()) low[call.back()] = min(low[call.back()], low[v]);
                if (low[v] == tin[v]) {
                    int x;
                    do x = st.back(), st.pop_back(), comp[x] = count;
                    while (x != v);
                    count++;
                }
            }
        }
        // Tarjan finds sinks first: reverse to get a topological order
        for (int &c : comp) c = count - 1 - c;
    }
    vector<vector<int>> components() const {
        vector<vector<int>> res(count);
        for (int v = 0; v < n; v++) res[comp[v]].push_back(v);
        return res;
    }
    vector<vector<int>> condensation(const vector<vector<int>> &g) const {
        vector<vector<int>> dag(count);
        for (int u = 0; u < n; u++)
            for (int v : g[u])
                if (comp[u] != comp[v]) dag[comp[u]].push_back(comp[v]);
        for (auto &adj : dag) {
            sort(adj.begin(), adj.end());
            adj.erase(unique(adj.begin(), adj.end()), adj.end());
        }
        return dag;
    }
};
