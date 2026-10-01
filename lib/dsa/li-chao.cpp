// Title: Li Chao tree
// Description: Min (or max) of lines/segments y = a x + b at integer points, any coordinate range (sparse).
// Usage:
//   LiChao<long long> lc(lo, hi);            // x in [lo, hi), e.g. (-1e9, 1e9 + 1); min by default
//   LiChao<long long, true> lc(lo, hi);      // max
//   lc.addLine(a, b);                        // on the whole range
//   lc.addSegment(a, b, l, r);               // only for x in [l, r)
//   lc.query(x)                              // best value at x, LiChao<...>::NONE if no line covers x
//   Watch overflow: |a x + b| must fit in T.
// Presets (T = long long by default; default range x in [-1e9, 1e9], i.e. [-1e9, 1e9 + 1)):
//   LiChao lc;  MinLiChao lc;            // min, long long
//   MaxLiChao lc;  MaxLiChao lc(lo, hi);  // max, long long
//   MaxLiChao<double> lc(lo, hi);        // other types
// Complexity: O(log C) per line and query, O(log^2 C) per segment, C = hi - lo.
// Verify: https://judge.yosupo.jp/problem/line_add_get_min
// Verify: https://judge.yosupo.jp/problem/segment_add_get_min
template <class T = long long, bool MAX = false> struct LiChao {
    static constexpr T NONE = MAX ? numeric_limits<T>::lowest() : numeric_limits<T>::max();
    struct Line {
        T a, b;
        T operator()(long long x) const { return a * x + b; }
    };
    struct Node {
        Line line{};
        bool has = false;
        int l = 0, r = 0;
    };
    long long lo, hi;
    vector<Node> t{Node{}, Node{}};  // t[1] is the root
    LiChao(long long lo_ = -1000000000, long long hi_ = 1000000001) : lo(lo_), hi(hi_) {}
    static bool better(T x, T y) { return MAX ? x > y : x < y; }
    void addLine(T a, T b) { addSegment(a, b, lo, hi); }
    void addSegment(T a, T b, long long l, long long r) {
        if (l < r) insert(1, lo, hi, max(l, lo), min(r, hi), Line{a, b});
    }
    int child(int x, bool right) {
        int c = right ? t[x].r : t[x].l;
        if (!c) {
            c = (int)t.size();
            t.push_back(Node{});
            (right ? t[x].r : t[x].l) = c;
        }
        return c;
    }
    void insert(int x, long long nl, long long nr, long long l, long long r, Line ln) {
        if (r <= nl || nr <= l) return;
        long long m = nl + (nr - nl) / 2;
        if (l <= nl && nr <= r) {
            if (!t[x].has) {
                t[x].line = ln, t[x].has = true;
                return;
            }
            bool left = better(ln(nl), t[x].line(nl)), mid = better(ln(m), t[x].line(m));
            if (mid) swap(t[x].line, ln);
            if (nr - nl == 1) return;
            if (left != mid) insert(child(x, false), nl, m, l, r, ln);
            else insert(child(x, true), m, nr, l, r, ln);
            return;
        }
        if (l < m) insert(child(x, false), nl, m, l, r, ln);
        if (m < r) insert(child(x, true), m, nr, l, r, ln);
    }
    T query(long long x) const {
        T res = NONE;
        long long nl = lo, nr = hi;
        for (int v = 1; v;) {
            if (t[v].has && better(t[v].line(x), res)) res = t[v].line(x);
            long long m = nl + (nr - nl) / 2;
            if (x < m) v = t[v].l, nr = m;
            else v = t[v].r, nl = m;
        }
        return res;
    }
};

template <class T = long long> using MinLiChao = LiChao<T, false>;
template <class T = long long> using MaxLiChao = LiChao<T, true>;
