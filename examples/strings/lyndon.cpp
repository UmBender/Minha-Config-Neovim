// Problem: Print the Lyndon factorization of s and its lexicographically smallest rotation.
// Input:
//   cabcab
// Output:
//   c abc ab
//   abcabc
#include <bits/stdc++.h>
using namespace std;

#include "strings/lyndon.cpp"  // in a solution: <leader>rl -> strings/lyndon

int main() {
    string s;
    cin >> s;
    vector<int> p = lyndon(s);
    for (int i = 0; i + 1 < (int)p.size(); i++) cout << s.substr(p[i], p[i + 1] - p[i]) << " \n"[i + 2 == (int)p.size()];
    int r = minRotation(s);
    cout << s.substr(r) + s.substr(0, r) << '\n';
}
