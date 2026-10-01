// Problem: given n numbers, print the maximum XOR of a subset, how many distinct values subset XORs
//   take, and for each query k the k-th smallest of them (1-based, -1 if there are fewer).
// Input:
//   4
//   3 10 9 6
//   3
//   1 5 9
// Output:
//   15 8
//   0
//   9
//   -1
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "math/xor-basis.cpp"  // in a solution: <leader>rl -> math/xor-basis

int main() {
    int n, q;
    cin >> n;
    XorBasis<60> xb;  // values < 2^60
    for (int i = 0; i < n; i++) {
        ll x;
        cin >> x;
        xb.add(x);
    }
    ll cnt = 1LL << xb.size();
    cout << xb.maxXor() << ' ' << cnt << '\n';
    cin >> q;
    while (q--) {
        ll k;
        cin >> k;
        cout << (k <= cnt ? (ll)xb.kth(k - 1) : -1) << '\n';
    }
}
