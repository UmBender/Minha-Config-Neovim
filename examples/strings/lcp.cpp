// Problem: Count the distinct non-empty substrings of s, and find the longest substring that
//   occurs at least twice.
// Input:
//   banana
// Output:
//   15
//   ana
#include <bits/stdc++.h>
using namespace std;

#include "strings/suffix-array.cpp"  // in a solution: <leader>rl -> strings/suffix-array
#include "strings/lcp.cpp"           // in a solution: <leader>rl -> strings/lcp

int main() {
    string s;
    cin >> s;
    long long n = (long long)s.size(), distinct = n * (n + 1) / 2;
    vector<int> sa = suffixArray(s), lcp = lcpArray(s, sa);
    int best = 0;
    for (int i = 1; i < n; i++) {
        distinct -= lcp[i];  // prefixes shared with the previous suffix were already counted
        if (lcp[i] > lcp[best]) best = i;
    }
    cout << distinct << '\n' << s.substr(sa[best], lcp[best]) << '\n';
}
