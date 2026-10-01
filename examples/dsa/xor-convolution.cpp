// Problem: lists A and B of numbers in [0, 2^k). For every v in [0, 2^k), count the pairs
//   (x from A, y from B) with x ^ y == v, then with x & y == v, then with x | y == v.
// Input:
//   2 4 4
//   1 2 3 3
//   0 2 2 3
// Output:
//   4 6 2 4
//   6 1 7 2
//   0 1 3 12
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "dsa/xor-convolution.cpp"  // in a solution: <leader>rl -> dsa/xor-convolution

void print(const vector<ll> &c) {
    for (size_t i = 0; i < c.size(); i++) cout << c[i] << " \n"[i + 1 == c.size()];
}

int main() {
    int k, n, m;
    cin >> k >> n >> m;
    vector<ll> fa(1 << k), fb(1 << k);  // frequencies: convolving them counts pairs
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        fa[x]++;
    }
    for (int i = 0; i < m; i++) {
        int y;
        cin >> y;
        fb[y]++;
    }
    print(xorConv(fa, fb));
    print(andConv(fa, fb));
    print(orConv(fa, fb));  // xorConv(fa, fb, 998244353) etc. when the counts need a modulus
}
