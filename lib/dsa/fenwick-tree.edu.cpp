// Title: Fenwick tree
// Description: Same code and API as the normal Fenwick tree, commented: how and why it works.
// Usage:
//   Fenwick<long long> fw(n);   Fenwick<long long> fw(a);   // zeros / from a vector in O(n)
//   fw.add(i, x);  fw.set(i, x);  fw.get(i);
//   fw.sum(r)      sum of [0, r)        fw.sum(l, r)   sum of [l, r)
//   fw.lowerBound(s)  smallest r with sum(r) >= s (all values >= 0); n + 1 if none, 0 if s <= 0
//   T defaults to long long: Fenwick fw(n).  Range add + range sum: the range variant.
// Complexity: O(log n) per operation, O(n) build.
//
// Idea: t[j - 1] (1-indexed j) stores the sum of the block (j - lowbit(j), j], where
// lowbit(j) = j & -j is the lowest set bit of j. Every prefix [0, r) is a union of at most
// log n such blocks: take block r, then block r - lowbit(r), ... (each step clears a bit).
// An element i (1-indexed j = i + 1) is in the blocks j, j + lowbit(j), ... (each step carries
// into a higher bit), so an update also touches O(log n) entries.
template <class T = long long> struct Fenwick {
    int n;
    vector<T> t;  // t[j - 1] = sum of a over (j - lowbit(j), j], 1-indexed
    Fenwick(int n_ = 0) : n(n_), t(n_, T{}) {}
    // O(n) build: start with t = a and push each finished block into the next block containing it
    Fenwick(const vector<T> &a) : n((int)a.size()), t(a) {
        for (int i = 1; i <= n; i++) {
            int j = i + (i & -i);
            if (j <= n) t[j - 1] += t[i - 1];
        }
    }
    // visit every block containing i: j, then j + lowbit(j), ...
    void add(int i, T x) {
        for (i++; i <= n; i += i & -i) t[i - 1] += x;
    }
    // sum of [0, r): blocks r, r - lowbit(r), ... cover it exactly
    T sum(int r) const {
        T s{};
        for (; r > 0; r -= r & -r) s += t[r - 1];
        return s;
    }
    T sum(int l, int r) const { return sum(r) - sum(l); }
    T get(int i) const { return sum(i, i + 1); }
    void set(int i, T x) { add(i, x - get(i)); }
    // Binary lifting instead of a binary search over sum(): build the answer bit by bit from the
    // top. With pos fixed, block pos + pw is exactly (pos, pos + pw], so if its sum is still < s
    // the answer is beyond it: take it. Needs all values >= 0 (prefix sums non-decreasing).
    int lowerBound(T s) const {
        if (!(T{} < s)) return 0;
        int pos = 0, pw = 1;
        while (pw * 2 <= n) pw *= 2;
        for (; pw; pw /= 2)
            if (pos + pw <= n && t[pos + pw - 1] < s) pos += pw, s -= t[pos - 1];
        return pos + 1;
    }
};

