// Title: KMP (prefix function)
// Description: Prefix function and all occurrences of a pattern in a text (Knuth-Morris-Pratt).
// Usage:
//   vector<int> pi = prefixFunction(s);   // pi[i]: longest proper border of s[0..i]
//   vector<int> at = kmpMatches(text, pat);  // start of every occurrence, overlapping ones too
//   s, text, pat: string or any vector<T> (only == is used); an empty pat matches at 0..|text|
//   smallest period of s: n - pi[n - 1]
// Complexity: O(n) for prefixFunction, O(|text| + |pat|) for kmpMatches.
template <class S> vector<int> prefixFunction(const S &s) {
    int n = (int)s.size();
    vector<int> pi(n);
    for (int i = 1, k = 0; i < n; i++) {
        while (k > 0 && !(s[i] == s[k])) k = pi[k - 1];
        if (s[i] == s[k]) k++;
        pi[i] = k;
    }
    return pi;
}

template <class S> vector<int> kmpMatches(const S &text, const S &pat) {
    int n = (int)text.size(), m = (int)pat.size();
    vector<int> res;
    if (m == 0) {
        res.resize(n + 1);
        iota(res.begin(), res.end(), 0);
        return res;
    }
    vector<int> pi = prefixFunction(pat);
    for (int i = 0, k = 0; i < n; i++) {
        while (k > 0 && !(text[i] == pat[k])) k = pi[k - 1];
        if (text[i] == pat[k]) k++;
        if (k == m) res.push_back(i - m + 1), k = pi[k - 1];
    }
    return res;
}
