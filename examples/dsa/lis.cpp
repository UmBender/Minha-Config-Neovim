// Problem: given n numbers, print the length of the longest strictly increasing subsequence,
//   the values of one, the longest non-decreasing and strictly decreasing lengths, and for each
//   position the length of the longest increasing subsequence ending there.
// Input:
//   8
//   3 1 4 1 5 9 2 6
// Output:
//   4
//   1 4 5 6
//   4 2
//   1 1 2 1 3 4 2 4
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "dsa/lis.cpp"  // in a solution: <leader>rl -> dsa/lis -> normal

int main() {
    int n;
    cin >> n;
    vector<ll> a(n);
    for (auto &x : a) cin >> x;

    vector<int> idx = lis(a);  // indices of one LIS
    cout << idx.size() << '\n';
    for (int i : idx) cout << a[i] << ' ';
    cout << '\n';

    cout << lis(a, false).size() << ' ' << lis(a, true, greater<ll>()).size() << '\n';

    for (int len : lisEnding(a)) cout << len << ' ';
    cout << '\n';
}
