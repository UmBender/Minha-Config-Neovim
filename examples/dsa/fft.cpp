// Problem: multiply two non-negative integers given in decimal (up to 1e5 digits each), then
//   print the coefficients of the real product (0.5 + x)(1.5 - x).
// Input:
//   12345 6789
// Output:
//   83810205
//   0.75 1.00 -1.00
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "dsa/fft.cpp"  // in a solution: <leader>rl -> dsa/fft

int main() {
    string s, t;
    cin >> s >> t;
    // digits, least significant first: the number is a polynomial evaluated at x = 10
    vector<ll> a, b;
    for (int i = (int)s.size() - 1; i >= 0; i--) a.push_back(s[i] - '0');
    for (int i = (int)t.size() - 1; i >= 0; i--) b.push_back(t[i] - '0');

    vector<ll> c = fftConvInt(a, b);  // exact: digits are small
    for (size_t i = 0; i + 1 < c.size(); i++) c[i + 1] += c[i] / 10, c[i] %= 10;
    while (c.back() >= 10) c.push_back(c.back() / 10), c[c.size() - 2] %= 10;
    while (c.size() > 1 && c.back() == 0) c.pop_back();
    for (int i = (int)c.size() - 1; i >= 0; i--) cout << c[i];
    cout << '\n';

    vector<double> p = fftConv({0.5, 1}, {1.5, -1});  // real coefficients
    cout << fixed << setprecision(2);
    for (size_t i = 0; i < p.size(); i++) cout << p[i] << " \n"[i + 1 == p.size()];
}
