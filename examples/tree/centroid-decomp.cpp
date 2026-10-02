// Problem: count the pairs of vertices u < v whose distance in the tree is exactly k.
// Input:
//   7 3
//   0 1
//   1 2
//   2 3
//   1 4
//   4 5
//   0 6
// Output:
//   6
#include <bits/stdc++.h>
using namespace std;

#include "tree/centroid-decomp.cpp"  // in a solution: <leader>rl -> tree/centroid-decomp

int main() {
    int n, k;
    cin >> n >> k;
    vector<vector<int>> g(n);
    for (int i = 0; i + 1 < n; i++) {
        int a, b;
        cin >> a >> b;
        g[a].push_back(b), g[b].push_back(a);
    }
    CentroidTree ct(g);
    // every path has a unique highest centroid c (lowest level) on it: count it there.
    // For each centroid, walk its piece (vertices of higher level), one child branch at a time.
    long long ans = 0;
    vector<int> cnt(n + 1);
    for (int c : ct.order) {
        vector<int> all{0};  // distances seen in earlier branches (and c itself)
        cnt[0] = 1;
        for (int s : g[c]) {
            if (ct.level[s] < ct.level[c]) continue;  // removed earlier: outside the piece
            vector<int> branch;
            vector<array<int, 3>> st{{s, c, 1}};
            while (!st.empty()) {
                auto [v, p, d] = st.back();
                st.pop_back();
                branch.push_back(d);
                for (int u : g[v])
                    if (u != p && ct.level[u] > ct.level[c]) st.push_back({u, v, d + 1});
            }
            for (int d : branch)
                if (d <= k) ans += cnt[k - d];
            for (int d : branch) cnt[d]++, all.push_back(d);
        }
        for (int d : all) cnt[d] = 0;
    }
    cout << ans << '\n';
}
