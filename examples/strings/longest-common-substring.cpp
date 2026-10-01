// Problem: Print the longest common substring of two strings and where it starts in each.
// Input:
//   competitive
//   repetition
// Output:
//   petiti 3 2
#include <bits/stdc++.h>
using namespace std;

#include "strings/suffix-automaton.cpp"          // required by strings/longest-common-substring
#include "strings/longest-common-substring.cpp"  // in a solution: <leader>rl -> strings/longest-common-substring

int main() {
    string a, b;
    cin >> a >> b;
    auto [i, j, len] = longestCommonSubstring(a, b);
    cout << a.substr(i, len) << ' ' << i << ' ' << j << '\n';
}
