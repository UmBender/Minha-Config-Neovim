// Title: Suffix max over keys
// Description: Insert pairs (x, y); query the best y among pairs with x >= X (online, any key type).
// Usage:
//   SuffixMax<int, long long> sm(LLONG_MIN);          // value returned when nothing qualifies
//   sm.add(x, y);  sm.query(X);                        // max y over x >= X
//   SuffixMax<int, long long, greater<long long>> mn(LLONG_MAX);   // min instead of max
//   SuffixMax<K, V, CmpV, greater<K>>                 // keys x <= X instead of x >= X
//   The identity defaults to the worst value for less/greater (lowest() / max()), else V{}.
//   K, V default to long long: SuffixMax sm.  Plain suffix-min, prefix-max, prefix-min: variants.
// Complexity: O(log n) amortized per operation.
template <class K = long long, class V = long long, class Cmp = less<V>, class KCmp = less<K>> struct SuffixMax {
    map<K, V, KCmp> m;  // in map order (keys increasing for less<K>) values strictly worse
    V e;
    Cmp cmp;
    static V worst() {
        if constexpr (is_same_v<Cmp, less<V>>) return numeric_limits<V>::lowest();
        else if constexpr (is_same_v<Cmp, greater<V>>) return numeric_limits<V>::max();
        else return V{};
    }
    SuffixMax(V e_ = worst()) : e(e_) {}
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

