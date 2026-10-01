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

#define CHECK(cond)                                                                          \
    do {                                                                                     \
        if (!(cond)) test::fail(__FILE__, __LINE__, #cond);                                  \
    } while (0)

#define CHECK_EQ(got, want)                                                                  \
    do {                                                                                     \
        const auto &got_ = (got);                                                            \
        const auto &want_ = (want);                                                          \
        if (!(got_ == want_))                                                                \
            test::fail(__FILE__, __LINE__,                                                   \
                       string(#got " == " #want "\n  got:  ") + test::show(got_) +           \
                           "\n  want: " + test::show(want_));                                \
    } while (0)

#define CHECK_NEAR(got, want, eps)                                                           \
    do {                                                                                     \
        const long double got_ = (got), want_ = (want);                                      \
        if (!test::near(got_, want_, eps))                                                   \
            test::fail(__FILE__, __LINE__,                                                   \
                       string(#got " ~= " #want "\n  got:  ") + test::show(got_) +           \
                           "\n  want: " + test::show(want_));                                \
    } while (0)
