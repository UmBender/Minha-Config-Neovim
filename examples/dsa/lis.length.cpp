// Problem: given n numbers, print the length of the longest strictly increasing subsequence and
//   of the longest non-decreasing one.
// Input:
//   8
//   3 1 4 1 5 9 2 6
// Output:
//   4 4
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "dsa/lis.length.cpp"  // in a solution: <leader>rl -> dsa/lis -> length

int main() {
    int n;
    cin >> n;
    vector<ll> a(n);
    for (auto &x : a) cin >> x;
    cout << lisLength(a) << ' ' << lisLength(a, false) << '\n';
}
