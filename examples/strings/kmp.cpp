// Problem: Print every position (0-indexed) where the pattern occurs in the text, then the
//   smallest period of the pattern.
// Input:
//   abababab
//   abab
// Output:
//   0 2 4
//   2
#include <bits/stdc++.h>
using namespace std;

#include "strings/kmp.cpp"  // in a solution: <leader>rl -> strings/kmp

int main() {
    string text, pat;
    cin >> text >> pat;
    for (int i : kmpMatches(text, pat)) cout << i << ' ';
    cout << '\n' << (int)pat.size() - prefixFunction(pat).back() << '\n';
}
