#include "test.h"
#include "dsa/sparse-table.max.cpp"
using ll = long long;

int main() {
    for (int it = 0; it < 300; it++) {
        int n = (int)test::rnd(1, 70);
        vector<ll> a = test::rndVec<ll>(n, -1e12, 1e12);
        MaxSparseTable st(a);
        for (int q = 0; q < 100; q++) {
            int l = (int)test::rnd(0, n - 1), r = (int)test::rnd(l + 1, n);
            CHECK_EQ(st.query(l, r), *max_element(a.begin() + l, a.begin() + r));
        }
    }
    MaxSparseTable<int> small(vector<int>{5, 3, 9});
    CHECK_EQ(small.query(0, 3), 9);
    CHECK_EQ(small.query(2, 3), 9);
    CHECK_EQ(MaxSparseTable<int>(vector<int>{12, 18, 8}).query(0, 2), 18);
}
