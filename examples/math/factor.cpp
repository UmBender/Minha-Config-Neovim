// Problem: for each n <= 1e18 print its factorization as "p^e" terms and its number of divisors.
// Input:
//   4
//   1
//   360
//   999999999999999989
//   1000000016000000063
// Output:
//   d=1
//   2^3 3^2 5^1 d=24
//   999999999999999989^1 d=2
//   1000000007^1 1000000009^1 d=4
#include <bits/stdc++.h>
using namespace std;

#include "math/primality.cpp"  // required by math/factor (the picker inserts it)
#include "math/factor.cpp"     // in a solution: <leader>rl -> math/factor

int main() {
    int q;
    cin >> q;
    while (q--) {
        unsigned long long n;
        cin >> n;
        auto fs = factor(n);
        long long divisors = 1;
        for (size_t i = 0, j; i < fs.size(); i = j) {
            for (j = i; j < fs.size() && fs[j] == fs[i];) j++;
            cout << fs[i] << '^' << j - i << ' ';
            divisors *= (long long)(j - i + 1);
        }
        cout << "d=" << divisors << '\n';
    }
}
