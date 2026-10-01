// Problem: a factory makes n products; product j earns c[j] per unit and uses A[i][j] units of
//   resource i, of which b[i] are available (fractional amounts allowed). Print the best profit
//   and the amounts. Then minimize 3x + 2y subject to x + y >= 4, x + 3y >= 6 and x, y >= 0.
// Input:
//   2 3
//   1 1 4
//   1 3 6
//   1 0 3
//   3 2
// Output:
//   11.00
//   3.00 1.00
//   8.00
//   0.00 4.00
#include <bits/stdc++.h>
using namespace std;
using ld = long double;

#include "dsa/simplex.cpp"  // in a solution: <leader>rl -> dsa/simplex

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<ld>> A(m, vector<ld>(n));
    vector<ld> b(m), c(n);
    for (int i = 0; i < m; i++) {
        for (auto &v : A[i]) cin >> v;
        cin >> b[i];
    }
    for (auto &v : c) cin >> v;

    cout << fixed << setprecision(2);
    vector<ld> x;
    ld profit = Simplex(A, b, c).solve(x);  // maximize c.x, A x <= b, x >= 0
    cout << profit << '\n';
    for (ld v : x) cout << v << ' ';
    cout << '\n';

    // minimize: negate c (and the answer); a >= row: negate the row
    ld cost = -Simplex({{-1, -1}, {-1, -3}}, {-4, -6}, {-3, -2}).solve(x);
    cout << cost << '\n';
    for (ld v : x) cout << v << ' ';
    cout << '\n';
}
