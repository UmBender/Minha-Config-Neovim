// Problem: given a[1..n] and b[1..n], print c[k] = sum over gcd(i, j) == k of a[i] b[j], then the
//   same with lcm(i, j) == k (k <= n). Then, for m masks over k bits with weights, print for every
//   mask S how many of the masks are subsets of S and the largest weight among them (0 if none).
// Input:
//   6
//   3 1 4 1 5 9
//   2 6 5 3 5 8
//   3 5
//   1 7
//   3 2
//   4 5
//   6 1
//   5 4
// Output:
//   358 112 97 3 25 72
//   6 26 43 23 50 282
//   0 1 0 2 1 3 2 5
//   0 7 0 7 5 7 5 7
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "dsa/zeta.cpp"  // in a solution: <leader>rl -> dsa/zeta

void print(const vector<ll> &c, int from) {
    for (int i = from; i < (int)c.size(); i++) cout << c[i] << " \n"[i + 1 == (int)c.size()];
}

int main() {
    int n;
    cin >> n;
    vector<ll> a(n + 1), b(n + 1);  // indices 1..n
    for (int i = 1; i <= n; i++) cin >> a[i];
    for (int i = 1; i <= n; i++) cin >> b[i];
    print(gcdConv(a, b), 1);  // presets; gcdConv(a, b, 998244353) for a modulus
    print(lcmConv(a, b), 1);

    int k, m;
    cin >> k >> m;
    vector<ll> cnt(1 << k), best(1 << k);
    for (int i = 0; i < m; i++) {
        int mask;
        ll w;
        cin >> mask >> w;
        cnt[mask]++, best[mask] = max(best[mask], w);
    }
    print(subsetZeta(cnt), 0);                                             // sum over subsets
    print(subsetZeta(best, [](ll x, ll y) { return max(x, y); }), 0);  // any op: max over subsets
}
