// Title: Prefix max over keys
// Description: Insert pairs (x, y); query the maximum y among pairs with x <= X (online).
// Usage:
//   PrefixMax<long long, long long> s;   // K, V default to long long
//   PrefixMax<int, long long> s(-1);     // value returned when nothing qualifies (default: lowest())
//   s.add(x, y);  s.query(X);
// Complexity: O(log n) amortized per operation.
template <class K = long long, class V = long long> struct PrefixMax {
    map<K, V> m;  // only pairs no other pair beats: as x grows, y grows strictly
    V e;
    PrefixMax(V e_ = numeric_limits<V>::lowest()) : e(e_) {}
    void add(K x, V y) {
        auto it = m.upper_bound(x);
        if (it != m.begin() && prev(it)->second >= y) return;  // (x', y') with x' <= x beats it
        it = m.insert_or_assign(it, x, y);
        while (next(it) != m.end() && next(it)->second <= y) m.erase(next(it));
    }
    V query(K x) const {
        auto it = m.upper_bound(x);
        return it == m.begin() ? e : prev(it)->second;
    }
};
