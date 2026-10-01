// Problem: items arrive online as "+ w v" (weight, value); a query "? W" prints the
//   smallest value among items with weight >= W (-1 if none).
// Input:
//   6
//   + 5 10
//   ? 3
//   + 2 7
//   ? 3
//   + 8 4
//   ? 6
// Output:
//   10
//   10
//   4
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "dsa/suffix-max.suffix-min.cpp"  // in a solution: <leader>rl -> dsa/suffix-max -> suffix-min

int main() {
    int q;
    cin >> q;
    SuffixMin s(-1LL);  // long long weights and values, -1 when nothing qualifies
    while (q--) {
        char op;
        cin >> op;
        if (op == '+') {
            ll w, v;
            cin >> w >> v;
            s.add(w, v);
        } else {
            ll w;
            cin >> w;
            cout << s.query(w) << '\n';
        }
    }
}
