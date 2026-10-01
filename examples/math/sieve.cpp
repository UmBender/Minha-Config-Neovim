// Problem: given n, print how many primes are below n and the sum of phi(x) for 1 <= x < n,
//   computed from each factorization; then factor each query x < n.
// Input:
//   100 3
//   1 84 97
// Output:
//   25 3004
//   1:
//   84: 2 2 3 7
//   97: 97
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "math/sieve.cpp"  // in a solution: <leader>rl -> math/sieve

int main() {
    int n, q;
    cin >> n >> q;
    Sieve sv(n);
    ll phiSum = 0;
    for (int x = 1; x < n; x++) {
        ll phi = x;
        auto fs = sv.factor(x);
        for (size_t i = 0; i < fs.size(); i++)
            if (i == 0 || fs[i] != fs[i - 1]) phi = phi / fs[i] * (fs[i] - 1);
        phiSum += phi;
    }
    cout << sv.primes.size() << ' ' << phiSum << '\n';
    while (q--) {
        int x;
        cin >> x;
        cout << x << ':';
        for (int p : sv.factor(x)) cout << ' ' << p;
        cout << '\n';
    }
}
