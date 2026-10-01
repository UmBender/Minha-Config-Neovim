// Problem: Given k patterns and a text, print how many times each pattern occurs, and the
//   number of positions of the text where no pattern ends.
// Input:
//   4
//   he she his hers
//   ushershishe
// Output:
//   2 2 1 1
//   7
#include <bits/stdc++.h>
using namespace std;

#include "strings/aho-corasick.cpp"  // in a solution: <leader>rl -> strings/aho-corasick

int main() {
    int k;
    cin >> k;
    AhoCorasick ac;
    for (int i = 0; i < k; i++) {
        string p;
        cin >> p;
        ac.add(p);
    }
    ac.build();
    string text;
    cin >> text;
    for (long long c : ac.count(text)) cout << c << ' ';
    int v = 0, quiet = 0;
    for (char c : text) quiet += ac.t[v = ac.next(v, c)].out == 0;
    cout << '\n' << quiet << '\n';
}
