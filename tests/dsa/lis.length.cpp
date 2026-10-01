#include "test.h"
#include "dsa/lis.length.cpp"

int brute(const vector<long long> &a, bool strict) {
    int n = (int)a.size(), best = 0;
    vector<int> dp(n, 1);
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < j; i++)
            if (strict ? a[i] < a[j] : a[i] <= a[j]) dp[j] = max(dp[j], dp[i] + 1);
        best = max(best, dp[j]);
    }
    return best;
}

int main() {
    CHECK_EQ(lisLength(vector<int>{}), 0);
    CHECK_EQ(lisLength(vector<int>{5, 1, 6, 2, 7, 3, 8}), 4);
    CHECK_EQ(lisLength(vector<int>{2, 2, 2}), 1);
    CHECK_EQ(lisLength(vector<int>{2, 2, 2}, false), 3);
    for (int it = 0; it < 2000; it++) {
        int n = (int)test::rnd(0, 40);
        vector<long long> a = test::rndVec<long long>(n, -5, 5);
        CHECK_EQ(lisLength(a), brute(a, true));
        CHECK_EQ(lisLength(a, false), brute(a, false));
    }
}
