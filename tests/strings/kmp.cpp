#include "test.h"
#include "strings/kmp.cpp"

template <class S> vector<int> brutePi(const S &s) {
    int n = (int)s.size();
    vector<int> pi(n);
    for (int i = 0; i < n; i++)
        for (int k = i; k >= 1; k--)
            if (equal(s.begin(), s.begin() + k, s.begin() + i + 1 - k)) {
                pi[i] = k;
                break;
            }
    return pi;
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
    CHECK_EQ(prefixFunction(string("")), vector<int>{});
    CHECK_EQ(prefixFunction(string("aabaaab")), (vector<int>{0, 1, 0, 1, 2, 2, 3}));
    CHECK_EQ(kmpMatches(string("abababa"), string("aba")), (vector<int>{0, 2, 4}));
    CHECK_EQ(kmpMatches(string("abc"), string("")), (vector<int>{0, 1, 2, 3}));
    CHECK_EQ(kmpMatches(string("ab"), string("abc")), vector<int>{});
    CHECK_EQ(kmpMatches(string(""), string("")), vector<int>{0});
    for (int it = 0; it < 2000; it++) {
        int k = (int)test::rnd(1, 3);
        string s = rndStr((int)test::rnd(0, 30), k), p = rndStr((int)test::rnd(0, 4), k);
        CHECK_EQ(prefixFunction(s), brutePi(s));
        CHECK_EQ(kmpMatches(s, p), bruteMatches(s, p));
    }
    for (int it = 0; it < 300; it++) {
        auto s = test::rndVec<int>((int)test::rnd(0, 30), -1, 1), p = test::rndVec<int>((int)test::rnd(1, 3), -1, 1);
        CHECK_EQ(prefixFunction(s), brutePi(s));
        CHECK_EQ(kmpMatches(s, p), bruteMatches(s, p));
    }
}
