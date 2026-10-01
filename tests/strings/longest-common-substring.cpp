#include "test.h"
#include "strings/suffix-automaton.cpp"
#include "strings/longest-common-substring.cpp"

template <class S> int bruteLen(const S &a, const S &b) {
    int best = 0;
    vector<vector<int>> dp(a.size() + 1, vector<int>(b.size() + 1));
    for (int i = 1; i <= (int)a.size(); i++)
        for (int j = 1; j <= (int)b.size(); j++)
            if (a[i - 1] == b[j - 1]) best = max(best, dp[i][j] = dp[i - 1][j - 1] + 1);
    return best;
}

template <int A, int Base, class S> void check(const S &a, const S &b) {
    auto [i, j, len] = longestCommonSubstring<A, Base>(a, b);
    CHECK_EQ(len, bruteLen(a, b));
    CHECK(0 <= i && i + len <= (int)a.size());
    CHECK(0 <= j && j + len <= (int)b.size());
    CHECK(equal(a.begin() + i, a.begin() + i + len, b.begin() + j));
}

string rndStr(int n, int k) {
    string s(n, 'a');
    for (auto &c : s) c = char('a' + test::rnd(0, k - 1));
    return s;
}

int main() {
    CHECK_EQ(longestCommonSubstring(string("xabcy"), string("zzabcab")), (array<int, 3>{1, 2, 3}));
    CHECK_EQ(longestCommonSubstring(string("abc"), string("xyz")), (array<int, 3>{0, 0, 0}));
    CHECK_EQ(longestCommonSubstring(string(""), string("abc")), (array<int, 3>{0, 0, 0}));
    for (int it = 0; it < 2000; it++) {
        int k = (int)test::rnd(1, 4);
        check<26, 'a'>(rndStr((int)test::rnd(0, 25), k), rndStr((int)test::rnd(0, 25), k));
    }
    for (int it = 0; it < 300; it++)
        check<2, 0>(test::rndVec<int>((int)test::rnd(0, 25), 0, 1), test::rndVec<int>((int)test::rnd(0, 25), 0, 1));
}
