// Title: Suffix max over keys
// Description: Insert pairs (x, y); query the best y among pairs with x >= X (online, any key type).
// Usage:
//   SuffixMax<int, long long> sm(LLONG_MIN);          // value returned when nothing qualifies
//   sm.add(x, y);  sm.query(X);                        // max y over x >= X
//   SuffixMax<int, long long, greater<long long>> mn(LLONG_MAX);   // min instead of max
//   For x <= X, insert -x and query -X.
// Complexity: O(log n) amortized per operation.
template <class K, class V, class Cmp = less<V>> struct SuffixMax {
    map<K, V> m;  // keys increasing => values strictly worse
    V e;
    Cmp cmp;
    SuffixMax(V e_ = V{}) : e(e_) {}
    void add(K x, V y) {
        auto it = m.lower_bound(x);
        if (it != m.end() && !cmp(it->second, y)) return;
        it = m.insert_or_assign(it, x, y);
        while (it != m.begin()) {
            auto p = prev(it);
            if (cmp(y, p->second)) break;
            m.erase(p);
        }
    }
    V query(K x) const {
        auto it = m.lower_bound(x);
        return it == m.end() ? e : it->second;
    }
};
