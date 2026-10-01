// Problem: for each query n m a b print sum_{0 <= i < n} floor((a*i + b) / m), then the same sum
//   weighted by i and of the squares (the last query has a negative slope).
// Input:
//   4
//   4 10 6 3
//   6 5 4 3
//   1000000000 1000000000 999999999 999999999
//   5 3 -2 1
// Output:
//   3 8 5
//   13 46 39
//   499999999500000000 333333332833333333500000000 333333332833333333500000000
//   -7 -21 15
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "math/floor-sum.cpp"  // in a solution: <leader>rl -> math/floor-sum

string str(__int128 x) {  // cout can't print __int128
    if (x < 0) return "-" + str(-x);
    return (x >= 10 ? str(x / 10) : "") + char('0' + x % 10);
}

int main() {
    int q;
    cin >> q;
    while (q--) {
        ll n, m, a, b;
        cin >> n >> m >> a >> b;
        auto s = floorSums(n, m, a, b);  // {floorSum, floorSumI, floorSumSq}
        cout << str(floorSum(n, m, a, b)) << ' ' << str(s[1]) << ' ' << str(s[2]) << '\n';
    }
}
