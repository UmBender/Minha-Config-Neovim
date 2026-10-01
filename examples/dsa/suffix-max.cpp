// Problem: items arrive online as "+ w v" (weight, value); a query "? W" prints the best value among
//   items with weight >= W and the best among items with weight <= W (-1 if none).
// Input:
//   6
//   + 5 10
//   ? 3
//   + 2 7
//   ? 3
//   + 8 4
//   ? 6
// Output:
//   10 -1
//   10 7
//   4 10
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "dsa/suffix-max.cpp"  // in a solution: <leader>rl -> dsa/suffix-max

int main() {
    int q;
    cin >> q;
    SuffixMax heavy(-1LL);  // preset defaults: long long keys/values; max value over weight >= W
    PrefixMax light(-1LL);  // max value over weight <= W
    // also SuffixMin / PrefixMin, and without an argument the identity is the worst value
    while (q--) {
        char op;
        cin >> op;
        if (op == '+') {
            ll w, v;
            cin >> w >> v;
            heavy.add(w, v), light.add(w, v);
        } else {
            ll w;
            cin >> w;
            cout << heavy.query(w) << ' ' << light.query(w) << '\n';
        }
    }
}
