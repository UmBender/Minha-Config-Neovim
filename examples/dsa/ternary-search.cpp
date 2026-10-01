// Problem: n houses at integer points (x[i], y[i]). Print the integer x minimizing the sum of
//   |x - x[i]| (the first one if tied) and that sum, then the minimum sum of Euclidean distances
//   from a real point (x, 0) on the road to the houses, with 4 decimals.
// Input:
//   4
//   1 2
//   3 -1
//   6 0
//   10 4
// Output:
//   3 12
//   14.1112
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "dsa/ternary-search.cpp"  // in a solution: <leader>rl -> dsa/ternary-search

int main() {
    int n;
    cin >> n;
    vector<ll> x(n), y(n);
    for (int i = 0; i < n; i++) cin >> x[i] >> y[i];

    // convex functions work: strictly decreasing, flat only at the minimum, then increasing
    auto cost = [&](ll p) {
        ll s = 0;
        for (int i = 0; i < n; i++) s += llabs(p - x[i]);
        return s;
    };
    ll best = ternaryMin(-1000000000LL, 1000000000LL, cost);
    cout << best << ' ' << cost(best) << '\n';

    auto dist = [&](double p) {
        double s = 0;
        for (int i = 0; i < n; i++) s += hypot(p - x[i], (double)y[i]);
        return s;
    };
    double p = ternaryMinReal(-1e4, 1e4, dist);
    cout << fixed << setprecision(4) << dist(p) << '\n';
}
