// Problem: For each prefix length k of s, print how many times s[0, k) occurs in s.
// Input:
//   abacaba
// Output:
//   4 2 2 1 1 1 1
#include <bits/stdc++.h>
using namespace std;

#include "strings/z-function.cpp"  // in a solution: <leader>rl -> strings/z-function

int main() {
    string s;
    cin >> s;
    int n = (int)s.size();
    vector<int> z = zFunction(s), cnt(n + 2);
    for (int i = 0; i < n; i++) cnt[z[i]]++;  // the prefix of length k occurs at i iff z[i] >= k
    for (int k = n - 1; k >= 1; k--) cnt[k] += cnt[k + 1];
    for (int k = 1; k <= n; k++) cout << cnt[k] << " \n"[k == n];
}
