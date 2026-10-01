// Problem: Given a text and q queries, print how many times each query occurs in the text and
//   where it first starts (-1 if absent). Then print the number of distinct substrings.
// Input:
//   abcbcb
//   3
//   bcb
//   a
//   ca
// Output:
//   2 1
//   1 0
//   0 -1
//   15
#include <bits/stdc++.h>
using namespace std;

#include "strings/suffix-automaton.cpp"  // in a solution: <leader>rl -> strings/suffix-automaton

int main() {
    string s;
    int q;
    cin >> s >> q;
    SuffixAutomaton sam(s);
    sam.countEndpos();
    while (q--) {
        string p;
        cin >> p;
        cout << sam.occurrences(p) << ' ' << sam.firstOccurrence(p) << '\n';
    }
    cout << sam.distinctSubstrings() << '\n';
}
