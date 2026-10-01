// Title: Lyndon factorization
// Description: Split s into non-increasing Lyndon words (Duval), and the minimum rotation.
// Usage:
//   vector<int> p = lyndon(s);   // split points 0 = p[0] < p[1] < ... < p.back() = n
//   factors s[p[i], p[i + 1]) are Lyndon words (strictly smaller than all their proper suffixes)
//   and lexicographically non-increasing
//   int i = minRotation(s);      // smallest i such that s[i..] + s[..i) is the minimum rotation
//   s: string or any vector<T> with <
// Complexity: O(n).
// Verify: https://judge.yosupo.jp/problem/lyndon_factorization
template <class S> vector<int> lyndon(const S &s) {
    int n = (int)s.size();
    vector<int> p = {0};
    for (int i = 0; i < n;) {
        // s[i, j) is a power of the Lyndon word of length j - k, plus a prefix of it
        int j = i + 1, k = i;
        while (j < n && !(s[j] < s[k])) {
            if (s[k] < s[j]) k = i;
            else k++;
            j++;
        }
        while (i <= k) p.push_back(i += j - k);
    }
    return p;
}

template <class S> int minRotation(const S &s) {
    int n = (int)s.size(), i = 0, res = 0;
    while (i < n) {  // Duval on s + s, the last factor starting before n is the answer
        res = i;
        int j = i + 1, k = i;
        while (j < 2 * n && !(s[j % n] < s[k % n])) {
            if (s[k % n] < s[j % n]) k = i;
            else k++;
            j++;
        }
        while (i <= k) i += j - k;
    }
    return res;
}
