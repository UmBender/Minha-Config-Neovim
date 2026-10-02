// Shared helpers for template tests. Each test is one program:
//   #include "test.h"
//   #include "dsa/fenwick-tree.cpp"   // resolved with -I lib
//   int main() { ... CHECK(...); CHECK_EQ(got, want); ... }
// Helpers live in namespace `test` so templates can't accidentally depend on them.
#pragma once
#include <bits/stdc++.h>
using namespace std;

namespace test {

inline mt19937_64 gen(0xC0FFEE);

// uniform integer in [lo, hi]
inline long long rnd(long long lo, long long hi) { return uniform_int_distribution<long long>(lo, hi)(gen); }

// uniform real in [lo, hi)
inline double rndReal(double lo, double hi) { return uniform_real_distribution<double>(lo, hi)(gen); }

template <class T> vector<T> rndVec(int n, T lo, T hi) {
    vector<T> v(n);
    for (auto &x : v) x = T(rnd((long long)lo, (long long)hi));
    return v;
}

// random tree on n vertices as an adjacency list, with shuffled labels and neighbor order.
// The parent of the i-th vertex is drawn from the `span` vertices before it: 1 is a path, a large
// span a shallow tree; span <= 0 picks a random shape.
inline vector<vector<int>> rndTree(int n, int span = 0) {
    if (span <= 0) span = vector<int>{1, 2, 3, n}[rnd(0, 3)];
    vector<int> id(n);
    iota(id.begin(), id.end(), 0);
    shuffle(id.begin(), id.end(), gen);
    vector<vector<int>> g(n);
    for (int i = 1; i < n; i++) {
        int p = (int)rnd(max(0, i - span), i - 1);
        g[id[i]].push_back(id[p]), g[id[p]].push_back(id[i]);
    }
    for (auto &adj : g) shuffle(adj.begin(), adj.end(), gen);
    return g;
}

// printable representation of (nested) values, used by CHECK_EQ
template <class T> string show(const T &x);
inline string show(const string &s) { return '"' + s + '"'; }
inline string show(const char *s) { return show(string(s)); }
template <class A, class B> string show(const pair<A, B> &p) {
    return "(" + show(p.first) + ", " + show(p.second) + ")";
}
template <class T> string show(const T &x) {
    if constexpr (requires { x.begin(); x.end(); }) {
        string s = "[";
        bool first = true;
        for (const auto &y : x) s += (first ? "" : ", ") + show(y), first = false;
        return s + "]";
    } else if constexpr (requires(ostream &os) { os << x; }) {
        ostringstream os;
        os << setprecision(17) << x;
        return os.str();
    } else {
        return "<value>";
    }
}

[[noreturn]] inline void fail(const char *file, int line, const string &msg) {
    cerr << file << ":" << line << ": FAILED: " << msg << endl;
    exit(1);
}

inline bool near(long double a, long double b, long double eps = 1e-6) {
    return fabsl(a - b) <= eps * max((long double)1, fabsl(b));
}

} // namespace test

// All checks are expressions, so they also work in comma expressions and unbraced bodies.
#define CHECK(cond) ((cond) ? (void)0 : test::fail(__FILE__, __LINE__, #cond))

#define CHECK_EQ(got, want)                                                                  \
    ([&](const auto &got_, const auto &want_) {                                              \
        if (!(got_ == want_))                                                                \
            test::fail(__FILE__, __LINE__,                                                   \
                       string(#got " == " #want "\n  got:  ") + test::show(got_) +           \
                           "\n  want: " + test::show(want_));                                \
    }((got), (want)))

#define CHECK_NEAR(got, want, eps)                                                           \
    ([&](long double got_, long double want_) {                                              \
        if (!test::near(got_, want_, (eps)))                                                 \
            test::fail(__FILE__, __LINE__,                                                   \
                       string(#got " ~= " #want "\n  got:  ") + test::show(got_) +           \
                           "\n  want: " + test::show(want_));                                \
    }((got), (want)))
