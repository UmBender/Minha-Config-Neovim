// Problem: n machines, machine i makes one product every t[i] seconds. Print the minimum time to
//   make k products, then the integer square root of m (largest x with x * x <= m) and the real
//   cube root of m with 6 decimals.
// Input:
//   3 7 50
//   3 2 5
// Output:
//   8
//   7
//   3.684031
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "dsa/binary-search.cpp"  // in a solution: <leader>rl -> dsa/binary-search

int main() {
    int n;
    ll k, m;
    cin >> n >> k >> m;
    vector<ll> t(n);
    for (auto &x : t) cin >> x;

    // first time in [0, k * min t] at which the machines have made >= k products
    ll hi = k * *min_element(t.begin(), t.end());
    ll T = firstTrue(0LL, hi + 1, [&](ll time) {
        ll made = 0;
        for (ll x : t) made = min(k, made + time / x);  // capped: no overflow
        return made >= k;
    });
    cout << T << '\n';

    // last x in [0, m] with x * x <= m
    cout << lastTrue(0LL, m + 1, [&](ll x) { return x * x <= m; }) << '\n';

    // reals: the boundary of a monotone predicate
    double c = firstTrueReal(0.0, (double)m + 1, [&](double x) { return x * x * x >= m; });
    cout << fixed << setprecision(6) << c << '\n';
}
