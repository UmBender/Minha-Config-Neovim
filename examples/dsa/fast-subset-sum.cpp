// Problem: split n weights into two groups with the smallest possible difference of sums; print
//   the difference and the indices of one group. Then answer q queries "is s a subset sum?".
// Input:
//   5
//   3 1 4 2 9
//   3
//   10 17 20
// Output:
//   1
//   0 2 3
//   YES
//   YES
//   NO
#include <bits/stdc++.h>
using namespace std;

#include "dsa/fast-subset-sum.cpp"  // in a solution: <leader>rl -> dsa/fast-subset-sum

int main() {
    int n;
    cin >> n;
    vector<int> w(n);
    for (auto &x : w) cin >> x;

    SubsetSum ss(w);
    int half = ss.maxAtMost(ss.total / 2);  // closest subset sum not above total / 2
    cout << ss.total - 2 * half << '\n';
    vector<int> group = ss.recover(half);
    sort(group.begin(), group.end());
    for (int i : group) cout << i << ' ';
    cout << '\n';

    int q;
    cin >> q;
    while (q--) {
        int s;
        cin >> s;
        cout << (ss.can(s) ? "YES" : "NO") << '\n';
    }
}
