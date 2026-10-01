#include "test.h"
#include "dsa/lis.cpp"

template <class T, class Cmp> void check(const vector<T> &a, bool strict, Cmp cmp) {
    int n = (int)a.size();
    auto ok = [&](int i, int j) { return strict ? cmp(a[i], a[j]) : !cmp(a[j], a[i]); };
    vector<int> dp(n, 1);
    for (int j = 0; j < n; j++)
        for (int i = 0; i < j; i++)
            if (ok(i, j)) dp[j] = max(dp[j], dp[i] + 1);
    int best = n ? *max_element(dp.begin(), dp.end()) : 0;
    vector<int> idx = lis(a, strict, cmp);
    CHECK_EQ((int)idx.size(), best);
    for (int k = 1; k < (int)idx.size(); k++) {
        CHECK(idx[k - 1] < idx[k]);
        CHECK(ok(idx[k - 1], idx[k]));
    }
    CHECK_EQ(lisEnding(a, strict, cmp), dp);
}

int main() {
    CHECK(lis(vector<int>{}).empty());
    CHECK_EQ(lis(vector<int>{3, 1, 2}), (vector<int>{1, 2}));
    CHECK_EQ((int)lis(vector<int>{2, 2, 2}).size(), 1);
    CHECK_EQ((int)lis(vector<int>{2, 2, 2}, false).size(), 3);
    for (int it = 0; it < 500; it++) {
        int n = (int)test::rnd(0, 40);
        vector<long long> a = test::rndVec<long long>(n, -5, 5);
        check(a, true, less<long long>());
        check(a, false, less<long long>());
        check(a, true, greater<long long>());  // longest decreasing
        check(a, false, greater<long long>());
    }
    vector<string> s = {"b", "a", "c", "ab", "d"};
    check(s, true, less<string>());
}
