// Title: Z-function
// Description: z[i] = longest common prefix of s and s[i..], plus pattern matching with it.
// Usage:
//   vector<int> z = zFunction(s);          // z[0] = n
//   vector<int> at = zMatches(text, pat);  // start of every occurrence, overlapping ones too
//   s, text, pat: string or any vector<T> (only == is used); an empty pat matches at 0..|text|
// Complexity: O(n) for zFunction, O(|text| + |pat|) for zMatches.
// Verify: https://judge.yosupo.jp/problem/zalgorithm
template <class S> vector<int> zFunction(const S &s) {
    int n = (int)s.size();
    vector<int> z(n);
    if (n) z[0] = n;
    for (int i = 1, l = 0, r = 0; i < n; i++) {  // [l, r): rightmost match found so far
        if (i < r) z[i] = min(r - i, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
        if (i + z[i] > r) l = i, r = i + z[i];
    }
    return z;
}

template <class S> vector<int> zMatches(const S &text, const S &pat) {
    int n = (int)text.size(), m = (int)pat.size();
    vector<int> z(n), zp = zFunction(pat), res;
    // like zFunction(pat + text), without building it: z[i] = lcp(pat, text[i..])
    for (int i = 0, l = 0, r = 0; i < n; i++) {
        if (i < r) z[i] = min(r - i, zp[i - l]);
        while (z[i] < m && i + z[i] < n && pat[z[i]] == text[i + z[i]]) z[i]++;
        if (i + z[i] > r) l = i, r = i + z[i];
        if (z[i] == m) res.push_back(i);
    }
    if (m == 0) res.push_back(n);
    return res;
}
