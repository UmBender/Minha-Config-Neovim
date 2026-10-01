// Problem: Print every maximal run of s as "period start end" (half-open), then the longest
//   tandem repeat (a string of the form ww) as a substring.
// Input:
//   aabaabab
// Output:
//   1 0 2
//   1 3 5
//   2 4 8
//   3 0 7
//   aabaab
#include <bits/stdc++.h>
using namespace std;

#include "strings/z-function.cpp"  // required by strings/runs (the picker inserts it)
#include "strings/runs.cpp"        // in a solution: <leader>rl -> strings/runs

int main() {
    string s;
    cin >> s;
    int bl = 0, blen = 0;
    for (auto [p, l, r] : runs(s)) {
        cout << p << ' ' << l << ' ' << r << '\n';
        int len = (r - l) / (2 * p) * 2 * p;  // longest even power of the period inside the run
        if (len > blen) bl = l, blen = len;
    }
    cout << s.substr(bl, blen) << '\n';
}
