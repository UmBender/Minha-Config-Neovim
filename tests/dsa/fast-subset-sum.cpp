#include "test.h"
#include "dsa/fast-subset-sum.cpp"

int main() {
    SubsetSum none(vector<int>{});
    CHECK(none.can(0));
    CHECK(!none.can(1));
    CHECK(none.recover(0).empty());

    for (int it = 0; it < 300; it++) {
        int n = (int)test::rnd(0, 25);
        vector<int> w = test::rndVec<int>(n, 0, it < 150 ? 20 : 300);
        SubsetSum ss(w);
        int total = accumulate(w.begin(), w.end(), 0);
        vector<char> dp(total + 1, 0);
        dp[0] = 1;
        for (int x : w)
            for (int s = total; s >= x; s--) dp[s] |= dp[s - x];
        for (int s = -2; s <= total + 70; s++) {
            bool want = s >= 0 && s <= total && dp[s];
            CHECK_EQ(ss.can(s), want);
            if (want) {
                auto idx = ss.recover(s);
                set<int> uniq(idx.begin(), idx.end());
                CHECK_EQ(uniq.size(), idx.size());
                long long sum = 0;
                for (int i : idx) CHECK(0 <= i && i < n), sum += w[i];
                CHECK_EQ(sum, (long long)s);
            }
        }
    }
}
