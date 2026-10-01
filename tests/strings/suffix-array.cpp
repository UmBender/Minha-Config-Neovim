#include "test.h"
#include "strings/suffix-array.cpp"

template <class S> vector<int> bruteSA(const S &s) {
    int n = (int)s.size();
    vector<int> sa(n);
    iota(sa.begin(), sa.end(), 0);
    sort(sa.begin(), sa.end(), [&](int i, int j) {
        return lexicographical_compare(s.begin() + i, s.end(), s.begin() + j, s.end());
    });
    return sa;
}

int main() {
    CHECK_EQ(suffixArray(string("")), vector<int>{});
    CHECK_EQ(suffixArray(string("z")), vector<int>{0});
    CHECK_EQ(suffixArray(string("banana")), (vector<int>{5, 3, 1, 0, 4, 2}));
    CHECK_EQ(suffixArray(string("aaaa")), (vector<int>{3, 2, 1, 0}));
    for (int it = 0; it < 2000; it++) {
        int n = (int)test::rnd(0, 40), k = (int)test::rnd(1, 4);
        string s(n, 'a');
        for (auto &c : s) c = char('a' + test::rnd(0, k - 1));
        CHECK_EQ(suffixArray(s), bruteSA(s));
    }
    for (int it = 0; it < 500; it++) {  // values far apart, negative, sparse
        auto s = test::rndVec<long long>((int)test::rnd(0, 40), -1000000000000LL, 1000000000000LL);
        for (auto &x : s)
            if (test::rnd(0, 1)) x = s[0];
        CHECK_EQ(suffixArray(s), bruteSA(s));
    }
    string big(200000, 'a');  // worst case for the number of doubling rounds
    auto sa = suffixArray(big);
    for (int i = 0; i < (int)big.size(); i++) CHECK_EQ(sa[i], (int)big.size() - 1 - i);
}
