// Problem: Tribonacci T(0) = 0, T(1) = 0, T(2) = 1, T(i) = T(i-1) + T(i-2) + T(i-3): print T(k) mod 1e9+7.
// Input:
//   4
//   2
//   7
//   10
//   1000000000000000000
// Output:
//   1
//   13
//   81
//   913728402
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "math/linear-recurrence.cpp"  // in a solution: <leader>rl -> math/linear-recurrence

int main() {
    int q;
    cin >> q;
    while (q--) {
        ll k;
        cin >> k;
        cout << linRec({0, 0, 1}, {1, 1, 1}, k, 1000000007) << '\n';
    }
}
