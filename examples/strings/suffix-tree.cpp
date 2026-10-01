// Problem: Count the distinct substrings of s that occur at least twice (number of positions
//   where they start). A node's subtree holds one suffix per occurrence of its strings.
// Input:
//   abab
// Output:
//   3
#include <bits/stdc++.h>
using namespace std;

#include "strings/suffix-array.cpp"  // required by strings/suffix-tree (the picker inserts it)
#include "strings/lcp.cpp"           // required by strings/suffix-tree (the picker inserts it)
#include "strings/suffix-tree.cpp"   // in a solution: <leader>rl -> strings/suffix-tree

int main() {
    string s;
    cin >> s;
    SuffixTree st(s);
    auto &t = st.t;
    vector<int> occ(t.size()), order = {0};
    for (int i = 0; i < (int)order.size(); i++)
        for (int c : t[order[i]].ch) order.push_back(c);
    long long ans = 0;
    for (int i = (int)order.size() - 1; i > 0; i--) {  // children before parents
        int v = order[i];
        occ[v] += t[v].suf != -1;
        occ[t[v].par] += occ[v];
        if (occ[v] >= 2) ans += t[v].r - t[v].l;  // every string on the edge into v
    }
    cout << ans << '\n';  // "a", "b", "ab"
}
