#include "test.h"
#include "strings/lyndon.cpp"

template <class S> bool isLyndon(const S &w) {  // strictly smaller than every proper suffix
    for (int i = 1; i < (int)w.size(); i++)
        if (!lexicographical_compare(w.begin(), w.end(), w.begin() + i, w.end())) return false;
    return !w.empty();
}

template <class S> void check(const S &s) {
    int n = (int)s.size();
    auto p = lyndon(s);
    CHECK(!p.empty() && p.front() == 0 && p.back() == n);
    S prev;
    for (int i = 0; i + 1 < (int)p.size(); i++) {
        CHECK(p[i] < p[i + 1]);
        S w(s.begin() + p[i], s.begin() + p[i + 1]);
        CHECK(isLyndon(w));
        if (i) CHECK(!(prev < w));  // non-increasing factors: the factorization is unique
        prev = w;
    }
    int best = 0;
    auto rot = [&](int i) {
        S r(s.begin() + i, s.end());
        r.insert(r.end(), s.begin(), s.begin() + i);
        return r;
    };
    for (int i = 1; i < n; i++)
        if (rot(i) < rot(best)) best = i;
    CHECK_EQ(minRotation(s), best);
}

int main() {
    CHECK_EQ(lyndon(string("")), vector<int>{0});
    CHECK_EQ(lyndon(string("abbaabb")), (vector<int>{0, 3, 7}));
    CHECK_EQ(lyndon(string("ccba")), (vector<int>{0, 1, 2, 3, 4}));
    CHECK_EQ(minRotation(string("bca")), 2);
    CHECK_EQ(minRotation(string("abab")), 0);
    for (int it = 0; it < 3000; it++) {
        int n = (int)test::rnd(0, 30), k = (int)test::rnd(1, 3);
        string s(n, 'a');
        for (auto &c : s) c = char('a' + test::rnd(0, k - 1));
        check(s);
    }
    for (int it = 0; it < 300; it++) check(test::rndVec<int>((int)test::rnd(0, 30), -1, 1));
}
