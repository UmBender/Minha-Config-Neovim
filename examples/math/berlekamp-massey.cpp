// Problem: given the first terms of a sequence that follows an unknown linear recurrence, print the
//   recurrence (modulo 998244353) and the k-th term (0-indexed).
// Input:
//   6
//   0 1 4 9 16 25
//   1000000000
// Output:
//   3: 3 998244350 1
//   716070898
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "math/powm.cpp"               // required by math/berlekamp-massey (the picker inserts it)
#include "math/berlekamp-massey.cpp"   // in a solution: <leader>rl -> math/berlekamp-massey
#include "math/linear-recurrence.cpp"  // <leader>rl -> math/linear-recurrence

int main() {
    int n;
    ll k;
    cin >> n;
    vector<ll> s(n);
    for (auto &x : s) cin >> x;
    cin >> k;
    auto c = berlekampMassey(s);  // s[i] = i^2: s[i] = 3 s[i-1] - 3 s[i-2] + s[i-3]
    cout << c.size() << ':';
    for (ll x : c) cout << ' ' << x;
    cout << '\n' << linRec(s, c, k) << '\n';
}
