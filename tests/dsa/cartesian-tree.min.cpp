#include "test.h"
#include "dsa/cartesian-tree.min.cpp"

// root of [l, r) = the index that is above every other one under `above`
template <class F> void brute(int l, int r, int parent, F above, vector<int> &par) {
    if (l >= r) return;
    int m = l;
    for (int i = l + 1; i < r; i++)
        if (above(i, m)) m = i;
    par[m] = parent;
    brute(l, m, m, above, par);
    brute(m + 1, r, m, above, par);
}

int main() {
    CHECK(minCartesianTree(vector<int>{}).empty());
    CHECK_EQ(minCartesianTree(vector<long long>{3, 1, 2}), (vector<int>{1, -1, 1}));
    for (int it = 0; it < 500; it++) {
        int n = (int)test::rnd(1, 50);
        vector<int> a = test::rndVec<int>(n, 0, it % 2 ? 5 : 1000);
        vector<int> want(n);
        brute(0, n, -1, [&](int i, int j) { return a[i] < a[j]; }, want);  // ties: the leftmost is the ancestor
        CHECK_EQ(minCartesianTree(a), want);
    }
}
