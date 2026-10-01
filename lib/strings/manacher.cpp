// Title: Manacher
// Description: Radius of the longest palindrome at every center, O(1) palindrome checks.
// Usage:
//   Manacher m(s);           // s: string or any vector<T> (only == is used)
//   m.odd[i]: palindromes s[i - k + 1, i + k) for k <= odd[i] (length 2 * odd[i] - 1)
//   m.even[i]: palindromes s[i - k, i + k) for k <= even[i] (length 2 * even[i])
//   m.isPalindrome(l, r)     // is s[l, r) a palindrome
//   auto [l, r] = m.longest();   // a longest palindrome s[l, r) (the leftmost one)
// Complexity: O(n) build, O(1) per query.
// Verify: https://judge.yosupo.jp/problem/enumerate_palindromes
struct Manacher {
    vector<int> odd, even;
    template <class S> Manacher(const S &s) {
        int n = (int)s.size();
        odd.assign(n, 0), even.assign(n, 0);
        // [l, r]: rightmost palindrome found so far; mirror the radius from inside it
        for (int i = 0, l = 0, r = -1; i < n; i++) {
            int k = i > r ? 1 : min(odd[l + r - i], r - i + 1);
            while (i - k >= 0 && i + k < n && s[i - k] == s[i + k]) k++;
            odd[i] = k--;
            if (i + k > r) l = i - k, r = i + k;
        }
        for (int i = 0, l = 0, r = -1; i < n; i++) {
            int k = i > r ? 0 : min(even[l + r - i + 1], r - i + 1);
            while (i - k - 1 >= 0 && i + k < n && s[i - k - 1] == s[i + k]) k++;
            even[i] = k--;
            if (i + k > r) l = i - k - 1, r = i + k;
        }
    }
    bool isPalindrome(int l, int r) const {
        int len = r - l;
        if (len <= 1) return true;
        if (len & 1) return odd[(l + r) / 2] >= (len + 1) / 2;
        return even[(l + r) / 2] >= len / 2;
    }
    pair<int, int> longest() const {
        pair<int, int> best{0, 0};
        auto upd = [&](int l, int r) {
            if (r - l > best.second - best.first || (r - l == best.second - best.first && l < best.first))
                best = {l, r};
        };
        for (int i = 0; i < (int)odd.size(); i++) upd(i - odd[i] + 1, i + odd[i]), upd(i - even[i], i + even[i]);
        return best;
    }
};
