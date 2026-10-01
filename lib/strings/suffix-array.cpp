// Title: Suffix array
// Description: Suffixes sorted lexicographically (prefix doubling with counting sort).
// Usage:
//   vector<int> sa = suffixArray(s);   // sa[i]: start of the i-th smallest suffix
//   s: string or any vector<T> with < and == (values are compressed first, any range works)
//   LCP of adjacent suffixes: strings/lcp
// Complexity: O(n log n).
// Verify: https://judge.yosupo.jp/problem/suffixarray
template <class S> vector<int> suffixArray(const S &s) {
    int n = (int)s.size();
    vector<int> sa(n), c(n), tmp(n), sb(n), cnt(n + 1);
    if (!n) return sa;
    iota(sa.begin(), sa.end(), 0);
    sort(sa.begin(), sa.end(), [&](int i, int j) { return s[i] < s[j]; });
    c[sa[0]] = 1;  // classes start at 1, 0 means "past the end"
    for (int i = 1; i < n; i++) c[sa[i]] = c[sa[i - 1]] + (s[sa[i - 1]] < s[sa[i]]);
    for (int k = 1; c[sa[n - 1]] < n; k <<= 1) {
        // sort by (c[i], c[i + k]): counting sort by the second key, then stable by the first
        auto byKey = [&](auto key) {
            fill(cnt.begin(), cnt.end(), 0);
            for (int i = 0; i < n; i++) cnt[key(i)]++;
            for (int i = 1; i <= n; i++) cnt[i] += cnt[i - 1];
            for (int i = n - 1; i >= 0; i--) sb[--cnt[key(sa[i])]] = sa[i];
            swap(sa, sb);
        };
        auto second = [&](int i) { return i + k < n ? c[i + k] : 0; };
        byKey(second);
        byKey([&](int i) { return c[i]; });
        tmp[sa[0]] = 1;
        for (int i = 1; i < n; i++) {
            int a = sa[i - 1], b = sa[i];
            tmp[b] = tmp[a] + (c[a] != c[b] || second(a) != second(b));
        }
        swap(c, tmp);
    }
    return sa;
}
