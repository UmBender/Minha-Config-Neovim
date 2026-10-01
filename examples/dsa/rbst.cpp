// Problem: version 0 is the array a. Each query builds a new version from an older one v:
//   "1 v k x" inserts x before position k, "2 v k" erases position k, "3 v k" moves the first k
//   elements to the end. Print every version at the end (old versions never change).
// Input:
//   3
//   1 2 3
//   4
//   1 0 1 9
//   2 0 0
//   3 1 3
//   1 2 2 7
// Output:
//   1 2 3
//   1 9 2 3
//   2 3
//   3 1 9 2
//   2 3 7
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "dsa/rbst.cpp"  // in a solution: <leader>rl -> dsa/rbst

int main() {
    int n, q;
    cin >> n;
    vector<ll> a(n);
    for (auto &x : a) cin >> x;
    cin >> q;

    PersistentRBST t;  // values are long long (default T) (PersistentRBST<char> for strings)
    vector<int> root = {t.build(a)};
    while (q--) {
        int type, v;
        ll k;
        cin >> type >> v >> k;
        if (type == 1) {
            ll x;
            cin >> x;
            root.push_back(t.insert(root[v], k, x));
        } else if (type == 2) {
            root.push_back(t.erase(root[v], k));
        } else {
            auto [l, r] = t.split(root[v], k);
            root.push_back(t.merge(r, l));
        }
    }
    for (int r : root) {
        vector<ll> s = t.toVector(r);
        for (size_t i = 0; i < s.size(); i++) cout << s[i] << " \n"[i + 1 == s.size()];
        if (s.empty()) cout << '\n';
    }
}
