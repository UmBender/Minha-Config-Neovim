// Problem: a static array and q queries [l, r): print the number of distinct values in a[l..r).
// Input:
//   7 4
//   1 2 1 3 2 2 4
//   0 3
//   2 6
//   0 7
//   4 6
// Output:
//   2
//   3
//   4
//   1
#include <bits/stdc++.h>
using namespace std;

#include "dsa/mo-queries.cpp"  // in a solution: <leader>rl -> dsa/mo-queries

int main() {
    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (auto &x : a) cin >> x;
    vector<pair<int, int>> qs(q);
    for (auto &[l, r] : qs) cin >> l >> r;

    // compress values so cnt can be an array
    vector<int> vals = a;
    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());
    for (auto &x : a) x = int(lower_bound(vals.begin(), vals.end(), x) - vals.begin());

    vector<int> cnt(vals.size()), ans(q);
    int distinct = 0;
    mo(qs, n, [&](int i) { distinct += cnt[a[i]]++ == 0; },  // a[i] enters the window
       [&](int i) { distinct -= --cnt[a[i]] == 0; },          // a[i] leaves the window
       [&](int qi) { ans[qi] = distinct; });                   // window == qs[qi]
    for (int x : ans) cout << x << '\n';
}
