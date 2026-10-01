// Problem: Print the suffixes of s in lexicographic order.
// Input:
//   banana
// Output:
//   a
//   ana
//   anana
//   banana
//   na
//   nana
#include <bits/stdc++.h>
using namespace std;

#include "strings/suffix-array.cpp"  // in a solution: <leader>rl -> strings/suffix-array

int main() {
    string s;
    cin >> s;
    for (int i : suffixArray(s)) cout << s.substr(i) << '\n';
}
