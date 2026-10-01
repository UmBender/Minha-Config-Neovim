// Title: Hungarian algorithm
// Description: Min-cost assignment of n rows to distinct columns of an n x m matrix (n <= m).
// Usage:
//   auto [cost, col] = hungarian(a);      // a: vector<vector<T>>, T = long long, double, ...
//   col[i] = column assigned to row i; cost = sum of a[i][col[i]] (minimum)
//   Maximum cost: the max variant in the <leader>rl menu.
// Complexity: O(n^2 m).
// Verify: https://judge.yosupo.jp/problem/assignment
template <class T> pair<T, vector<int>> hungarian(const vector<vector<T>> &a) {
    if (a.empty()) return {T{}, {}};
    int n = (int)a.size(), m = (int)a[0].size();
    const T INF = numeric_limits<T>::max();
    // 1-indexed potentials; p[j] = row matched to column j (0 = none)
    vector<T> u(n + 1), v(m + 1);
    vector<int> p(m + 1, 0), way(m + 1, 0);
    for (int i = 1; i <= n; i++) {
        p[0] = i;
        int j0 = 0;
        vector<T> minv(m + 1, INF);
        vector<char> used(m + 1, 0);
        do {
            used[j0] = 1;
            int i0 = p[j0], j1 = 0;
            T delta = INF;
            for (int j = 1; j <= m; j++) {
                if (used[j]) continue;
                T cur = a[i0 - 1][j - 1] - u[i0] - v[j];
                if (cur < minv[j]) minv[j] = cur, way[j] = j0;
                if (minv[j] < delta) delta = minv[j], j1 = j;
            }
            for (int j = 0; j <= m; j++)
                if (used[j]) u[p[j]] += delta, v[j] -= delta;
                else minv[j] -= delta;
            j0 = j1;
        } while (p[j0] != 0);
        do {
            int j1 = way[j0];
            p[j0] = p[j1], j0 = j1;
        } while (j0);
    }
    vector<int> col(n);
    for (int j = 1; j <= m; j++)
        if (p[j]) col[p[j] - 1] = j - 1;
    T cost{};
    for (int i = 0; i < n; i++) cost += a[i][col[i]];
    return {cost, col};
}

