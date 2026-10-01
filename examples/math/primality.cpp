// Problem: for each n (0 <= n < 2^64) print whether it is prime.
// Input:
//   6
//   1
//   2
//   561
//   998244353
//   3825123056546413051
//   18446744073709551557
// Output:
//   No
//   Yes
//   No
//   Yes
//   No
//   Yes
#include <bits/stdc++.h>
using namespace std;

#include "math/primality.cpp"  // in a solution: <leader>rl -> math/primality

int main() {
    int q;
    cin >> q;
    while (q--) {
        unsigned long long n;
        cin >> n;
        cout << (isPrime(n) ? "Yes" : "No") << '\n';
    }
}
