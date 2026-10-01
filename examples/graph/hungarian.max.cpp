// Problem: n workers, m >= n jobs, a[i][j] = profit of worker i on job j. Assign distinct jobs to
//   maximize the total profit.
// Input:
//   3 4
//   4 1 3 9
//   2 0 5 9
//   3 2 2 9
// Output:
//   18: 0 2 3
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "graph/hungarian.max.cpp"  // in a solution: <leader>rl -> graph/hungarian -> max

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<ll>> a(n, vector<ll>(m));
    for (auto &row : a)
        for (auto &x : row) cin >> x;
    auto [cost, col] = hungarianMax(a);
    cout << cost << ":";
    for (int j : col) cout << ' ' << j;
    cout << '\n';
}
