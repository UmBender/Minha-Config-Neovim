// Problem: histogram with n bars of heights h[i] (width 1). Print the parent of each bar in the
//   min and in the max Cartesian tree, then the area of the largest rectangle (each bar is the
//   minimum of the range spanned by its subtree in the min tree).
// Input:
//   7
//   2 1 4 5 1 3 3
// Output:
//   1 -1 4 2 1 4 5
//   2 0 3 -1 5 3 5
//   8
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "dsa/cartesian-tree.cpp"  // in a solution: <leader>rl -> dsa/cartesian-tree -> normal

int main() {
    int n;
    cin >> n;
    vector<ll> h(n);
    for (auto &x : h) cin >> x;

    // parents, min at the root (ties: leftmost is the ancestor), -1 for the root
    vector<int> par = cartesianTree(n, [&](int i, int j) { return h[i] < h[j]; });
    for (int p : par) cout << p << ' ';
    cout << '\n';
    for (int p : cartesianTree(n, [&](int i, int j) { return h[i] > h[j]; })) cout << p << ' ';
    cout << '\n';

    vector<vector<int>> ch(n);
    int root = -1;
    for (int i = 0; i < n; i++) {
        if (par[i] == -1) root = i;
        else ch[par[i]].push_back(i);
    }
    vector<int> L(n), R(n);  // subtree of v = indices [L[v], R[v])
    ll best = 0;
    auto dfs = [&](auto self, int v) -> void {
        L[v] = v, R[v] = v + 1;
        for (int c : ch[v]) self(self, c), L[v] = min(L[v], L[c]), R[v] = max(R[v], R[c]);
        best = max(best, h[v] * (R[v] - L[v]));
    };
    dfs(dfs, root);
    cout << best << '\n';
}
