// Problem: static array of n words and q queries "l r": print the number of distinct words in
//   a[l..r).
// Input:
//   6 3
//   red blue red green blue red
//   0 6
//   1 4
//   2 3
// Output:
//   3
//   3
//   1
#include <bits/stdc++.h>
using namespace std;

#include "dsa/wavelet-matrix.distinct.cpp"  // in a solution: <leader>rl -> dsa/wavelet-matrix -> distinct

int main() {
    int n, q;
    cin >> n >> q;
    vector<string> a(n);
    for (auto &s : a) cin >> s;
    DistinctCount dc(a);
    while (q--) {
        int l, r;
        cin >> l >> r;
        cout << dc.query(l, r) << '\n';
    }
}
