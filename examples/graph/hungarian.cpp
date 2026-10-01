// Problem: n workers, m >= n jobs, a[i][j] = time of worker i on job j. Assign distinct jobs to
//   minimize the total time; then, reading the matrix as profits, maximize the total (preset).
// Input:
//   3 4
//   4 1 3 9
//   2 0 5 9
//   3 2 2 9
// Output:
//   5: 1 0 2
//   18: 0 2 3
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "graph/hungarian.cpp"  // in a solution: <leader>rl -> graph/hungarian

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<ll>> a(n, vector<ll>(m));
    for (auto &row : a)
        for (auto &x : row) cin >> x;
    auto [cost, col] = hungarian(a);
    cout << cost << ":";
    for (int j : col) cout << ' ' << j;
    cout << '\n';
    auto [best, col2] = hungarianMax(a);
    cout << best << ":";
    for (int j : col2) cout << ' ' << j;
    cout << '\n';
}
