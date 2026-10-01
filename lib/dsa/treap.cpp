// Title: Implicit treap (sequence)
// Description: Sequence with insert/erase anywhere, range reverse, range add, range sum and min.
// Usage:
//   ImplicitTreap<long long> t;   ImplicitTreap<long long> t(a);
//   t.insert(pos, x);  t.erase(pos);  t.get(pos);  t.set(pos, x);  t.size();  t.toVector();
//   t.reverse(l, r);  t.add(l, r, x);  t.sum(l, r);  t.min(l, r) (l < r)     // ranges [l, r)
//   Sums of integer types are long long (S).
//   Low level: auto [a, b] = t.split(t.root, k);  t.root = t.merge(a, b);    // roots are node ids
//   To aggregate something else, edit Node, pull() and applyAdd().
// Presets:
//   ImplicitTreap t;   // T defaults to long long  (ImplicitTreap t(a) deduces T from a)
// Complexity: O(log n) expected per operation.
// Verify: https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum
template <class T = long long> struct ImplicitTreap {
    using S = conditional_t<is_integral_v<T>, long long, T>;
    struct Node {
        int l = 0, r = 0, sz = 0;
        unsigned pr = 0;
        T val{}, mn{}, lz{};
        S sum{};
        bool rev = false, hasLz = false;
    };
    vector<Node> t{Node{}};  // t[0] is the empty node
    int root = 0;
    mt19937 rng{(unsigned)chrono::steady_clock::now().time_since_epoch().count()};
    ImplicitTreap() {}
    ImplicitTreap(const vector<T> &a) {
        for (const T &x : a) root = merge(root, make(x));
    }
    int make(const T &v) {
        Node nd;
        nd.sz = 1, nd.pr = rng(), nd.val = nd.sum = nd.mn = v;
        t.push_back(nd);
        return (int)t.size() - 1;
    }
    void applyRev(int x) {
        if (x) swap(t[x].l, t[x].r), t[x].rev ^= 1;
    }
    void applyAdd(int x, const T &v) {
        if (!x) return;
        Node &nd = t[x];
        nd.val += v, nd.sum += (S)v * nd.sz, nd.mn += v;
        nd.lz = nd.hasLz ? nd.lz + v : v, nd.hasLz = true;
    }
    void push(int x) {
        if (t[x].rev) applyRev(t[x].l), applyRev(t[x].r), t[x].rev = false;
        if (t[x].hasLz) applyAdd(t[x].l, t[x].lz), applyAdd(t[x].r, t[x].lz), t[x].hasLz = false;
    }
    int pull(int x) {
        Node &nd = t[x];
        nd.sz = 1 + t[nd.l].sz + t[nd.r].sz;
        nd.sum = (S)nd.val, nd.mn = nd.val;
        if (nd.l) nd.sum = t[nd.l].sum + nd.sum, nd.mn = std::min(t[nd.l].mn, nd.mn);
        if (nd.r) nd.sum = nd.sum + t[nd.r].sum, nd.mn = std::min(nd.mn, t[nd.r].mn);
        return x;
    }
    pair<int, int> split(int x, int k) {  // first k, rest
        if (!x) return {0, 0};
        push(x);
        if (t[t[x].l].sz >= k) {
            auto [a, b] = split(t[x].l, k);
            t[x].l = b;
            return {a, pull(x)};
        }
        auto [a, b] = split(t[x].r, k - t[t[x].l].sz - 1);
        t[x].r = a;
        return {pull(x), b};
    }
    int merge(int a, int b) {
        if (!a || !b) return a ? a : b;
        if (t[a].pr > t[b].pr) {
            push(a);
            t[a].r = merge(t[a].r, b);
            return pull(a);
        }
        push(b);
        t[b].l = merge(a, t[b].l);
        return pull(b);
    }
    template <class F> void onRange(int l, int r, F f) {  // f(node id of the subtree for [l, r))
        auto [a, rest] = split(root, l);
        auto [m, c] = split(rest, r - l);
        f(m);
        root = merge(a, merge(m, c));
    }
    int size() const { return t[root].sz; }
    void insert(int pos, const T &v) {
        auto [a, b] = split(root, pos);
        root = merge(merge(a, make(v)), b);
    }
    void erase(int pos) {
        auto [a, rest] = split(root, pos);
        root = merge(a, split(rest, 1).second);
    }
    T get(int pos) {
        T res{};
        onRange(pos, pos + 1, [&](int x) { res = t[x].val; });
        return res;
    }
    void set(int pos, const T &v) {
        onRange(pos, pos + 1, [&](int x) { t[x].val = v, pull(x); });
    }
    void reverse(int l, int r) {
        onRange(l, r, [&](int x) { applyRev(x); });
    }
    void add(int l, int r, const T &v) {
        onRange(l, r, [&](int x) { applyAdd(x, v); });
    }
    S sum(int l, int r) {
        S res{};
        onRange(l, r, [&](int x) { res = t[x].sum; });
        return res;
    }
    T min(int l, int r) {
        T res{};
        onRange(l, r, [&](int x) { res = t[x].mn; });
        return res;
    }
    vector<T> toVector() {
        vector<T> out;
        auto dfs = [&](auto self, int x) -> void {
            if (!x) return;
            push(x);
            self(self, t[x].l);
            out.push_back(t[x].val);
            self(self, t[x].r);
        };
        dfs(dfs, root);
        return out;
    }
};
