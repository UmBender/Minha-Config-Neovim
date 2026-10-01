// Title: LCP array
// Description: Longest common prefix of adjacent suffixes in the suffix array (Kasai).
// Usage:
//   vector<int> lcp = lcpArray(s, sa);   // sa from strings/suffix-array
//   lcp[0] = 0, lcp[i] = lcp(s[sa[i - 1]..], s[sa[i]..]) for i >= 1
//   lcp of any two suffixes: min of lcp over (rank[i], rank[j]], e.g. with dsa/sparse-table
//   distinct substrings: n(n + 1) / 2 - sum(lcp)
// Complexity: O(n).
// Verify: https://judge.yosupo.jp/problem/number_of_substrings
template <class S> vector<int> lcpArray(const S &s, const vector<int> &sa) {
    int n = (int)s.size();
    vector<int> rank(n), lcp(n);
    for (int i = 0; i < n; i++) rank[sa[i]] = i;
    for (int i = 0, k = 0; i < n; i++) {  // the lcp drops by at most 1 from suffix i to i + 1
        if (rank[i] == 0) {
            k = 0;
            continue;
        }
        int j = sa[rank[i] - 1];
        while (i + k < n && j + k < n && s[i + k] == s[j + k]) k++;
        lcp[rank[i]] = k;
        if (k) k--;
    }
    return lcp;
}
