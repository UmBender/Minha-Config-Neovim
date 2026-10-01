// Problem: given a[1..n] and b[1..n], print c[k] = sum over lcm(i, j) == k of a[i] b[j] (k <= n),
//   exactly and modulo 7.
// Input:
//   6
//   3 1 4 1 5 9
//   2 6 5 3 5 8
// Output:
//   6 26 43 23 50 282
//   6 5 1 2 1 2
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "dsa/zeta.lcm-conv.cpp"  // in a solution: <leader>rl -> dsa/zeta -> lcm-conv

void print(const vector<ll> &c) {
    for (int i = 1; i < (int)c.size(); i++) cout << c[i] << " \n"[i + 1 == (int)c.size()];
}

int main() {
    int n;
    cin >> n;
    vector<ll> a(n + 1), b(n + 1);  // indices 1..n
    for (int i = 1; i <= n; i++) cin >> a[i];
    for (int i = 1; i <= n; i++) cin >> b[i];
    print(lcmConv(a, b));
    print(lcmConv(a, b, 7));
}
