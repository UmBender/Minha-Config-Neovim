#include "test.h"
#include "strings/lcp.cpp"

template <class S> vector<int> bruteSA(const S &s) {
    int n = (int)s.size();
    vector<int> sa(n);
    iota(sa.begin(), sa.end(), 0);
    sort(sa.begin(), sa.end(), [&](int i, int j) {
        return lexicographical_compare(s.begin() + i, s.end(), s.begin() + j, s.end());
    });
    return sa;
}

template <class S> vector<int> bruteLcp(const S &s, const vector<int> &sa) {
    int n = (int)s.size();
    vector<int> lcp(n);
    for (int i = 1; i < n; i++) {
        int a = sa[i - 1], b = sa[i];
        while (max(a, b) + lcp[i] < n && s[a + lcp[i]] == s[b + lcp[i]]) lcp[i]++;
    }
    return lcp;
}

int main() {
    string e;
    CHECK_EQ(lcpArray(e, vector<int>{}), vector<int>{});
    string b = "banana";
    CHECK_EQ(lcpArray(b, vector<int>{5, 3, 1, 0, 4, 2}), (vector<int>{0, 1, 3, 0, 0, 2}));
    for (int it = 0; it < 2000; it++) {
        int n = (int)test::rnd(0, 40), k = (int)test::rnd(1, 3);
        string s(n, 'a');
        for (auto &c : s) c = char('a' + test::rnd(0, k - 1));
        auto sa = bruteSA(s);
        CHECK_EQ(lcpArray(s, sa), bruteLcp(s, sa));
    }
    for (int it = 0; it < 300; it++) {
        auto s = test::rndVec<int>((int)test::rnd(0, 40), 0, 1);
        auto sa = bruteSA(s);
        CHECK_EQ(lcpArray(s, sa), bruteLcp(s, sa));
    }
    string big(200000, 'a');  // Kasai's amortization: dropping k (O(n^2)) times out here
    vector<int> sa(big.size());
    iota(sa.rbegin(), sa.rend(), 0);
    auto lcp = lcpArray(big, sa);
    for (int i = 1; i < (int)big.size(); i++) CHECK_EQ(lcp[i], i);
}
