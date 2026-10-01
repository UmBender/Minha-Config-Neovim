// Title: Suffix min over keys
// Description: Insert pairs (x, y); query the minimum y among pairs with x >= X (online).
// Usage:
//   SuffixMin<long long, long long> s;   // K, V default to long long
//   SuffixMin<int, long long> s(-1);     // value returned when nothing qualifies (default: max())
//   s.add(x, y);  s.query(X);
// Complexity: O(log n) amortized per operation.
template <class K = long long, class V = long long> struct SuffixMin {
    map<K, V> m;  // only pairs no other pair beats: as x grows, y grows strictly
    V e;
    SuffixMin(V e_ = numeric_limits<V>::max()) : e(e_) {}
    void add(K x, V y) {
        auto it = m.lower_bound(x);
        if (it != m.end() && it->second <= y) return;  // (x', y') with x' >= x, y' <= y beats it
        it = m.insert_or_assign(it, x, y);
        while (it != m.begin() && prev(it)->second >= y) m.erase(prev(it));
    }
    V query(K x) const {
        auto it = m.lower_bound(x);
        return it == m.end() ? e : it->second;
    }
};
