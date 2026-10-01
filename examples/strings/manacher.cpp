// Problem: Print the longest palindromic substring, the number of palindromic substrings
//   (counted by position), and answer queries "l r": is s[l, r) a palindrome?
// Input:
//   abaaba
//   3
//   0 3
//   1 4
//   2 4
// Output:
//   abaaba
//   11
//   yes
//   no
//   yes
#include <bits/stdc++.h>
using namespace std;

#include "strings/manacher.cpp"  // in a solution: <leader>rl -> strings/manacher

int main() {
    string s;
    int q;
    cin >> s >> q;
    Manacher m(s);
    auto [l, r] = m.longest();
    long long count = 0;
    for (int i = 0; i < (int)s.size(); i++) count += m.odd[i] + m.even[i];
    cout << s.substr(l, r - l) << '\n' << count << '\n';
    while (q--) {
        int a, b;
        cin >> a >> b;
        cout << (m.isPalindrome(a, b) ? "yes" : "no") << '\n';
    }
}
