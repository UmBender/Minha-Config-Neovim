// Title: Persistent RBST (sequence)
// Description: Persistent randomized BST over a sequence: split/merge by position, every op returns a new root.
// Usage:
//   PersistentRBST<char> t;                 // roots are ints, 0 = empty sequence
//   int r = t.build(v);                     // from a vector
//   r2 = t.insert(r, k, x);  t.erase(r, k);  t.set(r, k, x);  t.get(r, k);  t.size(r);
//   auto [a, b] = t.split(r, k);            // first k elements, rest
//   int c = t.merge(a, b);                  // a or b may be the same version (sharing is fine:
//                                           // sizes are long long, e.g. doubling a string 60 times)
//   t.toVector(r)
//   Old roots stay valid. Memory grows by O(log n) nodes per operation.
// Complexity: O(log n) expected per operation.
// Verify: https://atcoder.jp/contests/abc417/tasks/abc417_g
// Pending: example + presets (T-009..T-011), remove when done
template <class T> struct PersistentRBST {
    vector<int> L{0}, R{0};
    vector<long long> cnt{0};
    vector<T> val{T{}};
    mt19937_64 rng{(unsigned long long)chrono::steady_clock::now().time_since_epoch().count()};
    long long size(int x) const { return cnt[x]; }
    int make(const T &v) {
        L.push_back(0), R.push_back(0), cnt.push_back(1), val.push_back(v);
        return (int)cnt.size() - 1;
    }
    int copy(int x) {
        L.push_back(L[x]), R.push_back(R[x]), cnt.push_back(cnt[x]), val.push_back(val[x]);
        return (int)cnt.size() - 1;
    }
    int pull(int x) {
        cnt[x] = cnt[L[x]] + cnt[R[x]] + 1;
        return x;
    }
    int merge(int a, int b) {
        if (!a || !b) return a ? a : b;
        if (rng() % (unsigned long long)(cnt[a] + cnt[b]) < (unsigned long long)cnt[a]) {
            int x = copy(a);
            int r = merge(R[x], b);
            R[x] = r;
            return pull(x);
        }
        int x = copy(b);
        int l = merge(a, L[x]);
        L[x] = l;
        return pull(x);
    }
    pair<int, int> split(int x, long long k) {  // first k, rest
        if (!x) return {0, 0};
        int y = copy(x);
        if (k <= cnt[L[y]]) {
            auto [a, b] = split(L[y], k);
            L[y] = b;
            return {a, pull(y)};
        }
        auto [a, b] = split(R[y], k - cnt[L[y]] - 1);
        R[y] = a;
        return {pull(y), b};
    }
    int build(const vector<T> &v, int l = 0, int r = -1) {
        if (r < 0) r = (int)v.size();
        if (l >= r) return 0;
        int m = (l + r) / 2;
        int lc = build(v, l, m), rc = build(v, m + 1, r);
        int x = make(v[m]);
        L[x] = lc, R[x] = rc;
        return pull(x);
    }
    T get(int x, long long k) const {
        while (true) {
            if (k < cnt[L[x]]) x = L[x];
            else if (k == cnt[L[x]]) return val[x];
            else k -= cnt[L[x]] + 1, x = R[x];
        }
    }
    int insert(int x, long long k, const T &v) {
        auto [a, b] = split(x, k);
        return merge(merge(a, make(v)), b);
    }
    int erase(int x, long long k) {
        auto [a, b] = split(x, k);
        return merge(a, split(b, 1).second);
    }
    int set(int x, long long k, const T &v) {
        int y = copy(x);
        if (k < cnt[L[y]]) {
            int l = set(L[y], k, v);
            L[y] = l;
        } else if (k == cnt[L[y]]) {
            val[y] = v;
        } else {
            int r = set(R[y], k - cnt[L[y]] - 1, v);
            R[y] = r;
        }
        return y;
    }
    vector<T> toVector(int x) const {
        vector<T> out;
        vector<int> st;
        while (x || !st.empty()) {
            while (x) st.push_back(x), x = L[x];
            x = st.back(), st.pop_back();
            out.push_back(val[x]);
            x = R[x];
        }
        return out;
    }
};
