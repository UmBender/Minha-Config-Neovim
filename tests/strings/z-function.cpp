#include "test.h"
#include "strings/z-function.cpp"

template <class S> vector<int> bruteZ(const S &s) {
    int n = (int)s.size();
    vector<int> z(n);
    for (int i = 0; i < n; i++)
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
    return z;
}

template <class S> vector<int> bruteMatches(const S &t, const S &p) {
    vector<int> res;
    for (int i = 0; i + (int)p.size() <= (int)t.size(); i++)
        if (equal(p.begin(), p.end(), t.begin() + i)) res.push_back(i);
    return res;
}

string rndStr(int n, int k) {
    string s(n, 'a');
    for (auto &c : s) c = char('a' + test::rnd(0, k - 1));
    return s;
}

int main() {
    CHECK_EQ(zFunction(string("")), vector<int>{});
    CHECK_EQ(zFunction(string("aaa")), (vector<int>{3, 2, 1}));
    CHECK_EQ(zFunction(string("abacaba")), (vector<int>{7, 0, 1, 0, 3, 0, 1}));
    CHECK_EQ(zMatches(string("aaaa"), string("aa")), (vector<int>{0, 1, 2}));
    CHECK_EQ(zMatches(string("abc"), string("")), (vector<int>{0, 1, 2, 3}));
    CHECK_EQ(zMatches(string("ab"), string("abc")), vector<int>{});
    for (int it = 0; it < 2000; it++) {
        int k = (int)test::rnd(1, 3);
        string s = rndStr((int)test::rnd(0, 30), k), p = rndStr((int)test::rnd(0, 4), k);
        CHECK_EQ(zFunction(s), bruteZ(s));
        CHECK_EQ(zMatches(s, p), bruteMatches(s, p));
    }
    for (int it = 0; it < 300; it++) {
        auto s = test::rndVec<int>((int)test::rnd(0, 30), 5, 6), p = test::rndVec<int>((int)test::rnd(1, 3), 5, 6);
        CHECK_EQ(zFunction(s), bruteZ(s));
        CHECK_EQ(zMatches(s, p), bruteMatches(s, p));
    }
}
