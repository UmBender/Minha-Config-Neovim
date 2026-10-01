#include "test.h"
#include "strings/manacher.cpp"

template <class S> bool brutePal(const S &s, int l, int r) {
    for (int i = l, j = r - 1; i < j; i++, j--)
        if (s[i] != s[j]) return false;
    return true;
}

template <class S> void check(const S &s) {
    int n = (int)s.size();
    Manacher m(s);
    CHECK_EQ((int)m.odd.size(), n);
    CHECK_EQ((int)m.even.size(), n);
    int best = 0;
    for (int i = 0; i < n; i++) {
        int k = 0;
        while (i - k >= 0 && i + k < n && s[i - k] == s[i + k]) k++;
        CHECK_EQ(m.odd[i], k);
        k = 0;
        while (i - k - 1 >= 0 && i + k < n && s[i - k - 1] == s[i + k]) k++;
        CHECK_EQ(m.even[i], k);
    }
    for (int l = 0; l <= n; l++)
        for (int r = l; r <= n; r++) {
            bool p = brutePal(s, l, r);
            CHECK_EQ(m.isPalindrome(l, r), p);
            if (p) best = max(best, r - l);
        }
    auto [l, r] = m.longest();
    CHECK_EQ(r - l, best);
    CHECK(0 <= l && r <= n && brutePal(s, l, r));
}

int main() {
    Manacher m(string("abaab"));
    CHECK_EQ(m.odd, (vector<int>{1, 2, 1, 1, 1}));
    CHECK_EQ(m.even, (vector<int>{0, 0, 0, 2, 0}));
    CHECK_EQ(m.longest(), (pair<int, int>{1, 5}));
    check(string(""));
    check(string("a"));
    for (int it = 0; it < 2000; it++) {
        int n = (int)test::rnd(0, 30), k = (int)test::rnd(1, 3);
        string s(n, 'a');
        for (auto &c : s) c = char('a' + test::rnd(0, k - 1));
        check(s);
    }
    for (int it = 0; it < 200; it++) check(test::rndVec<int>((int)test::rnd(0, 30), 0, 1));
}
