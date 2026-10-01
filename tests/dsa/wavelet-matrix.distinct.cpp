#include "test.h"
#include "dsa/wavelet-matrix.distinct.cpp"
using ll = long long;

int main() {
    CHECK_EQ(DistinctCount(vector<int>{}).query(0, 0), 0);
    for (int it = 0; it < 300; it++) {
        int n = (int)test::rnd(1, 60);
        vector<ll> a = test::rndVec<ll>(n, it % 2 ? -3 : -(ll)1e18, it % 2 ? 3 : (ll)1e18);
        DistinctCount dc(a);
        for (int q = 0; q < 50; q++) {
            int l = (int)test::rnd(0, n), r = (int)test::rnd(l, n);
            CHECK_EQ(dc.query(l, r), (int)set<ll>(a.begin() + l, a.begin() + r).size());
        }
    }
    vector<string> words = {"a", "b", "a", "c", "b"};
    DistinctCount dw(words);
    CHECK_EQ(dw.query(0, 5), 3);
    CHECK_EQ(dw.query(1, 3), 2);
    CHECK_EQ(dw.query(2, 2), 0);
}
