#include "test.h"
#include "math/perm-int.cpp"
using ll = long long;

int main() {
    CHECK_EQ(perm2int({}), 0LL);
    CHECK(int2perm(0, 0).empty());
    CHECK_EQ(perm2int({0}), 0LL);
    CHECK_EQ(perm2int({2, 1, 0}), 5LL);
    CHECK_EQ(int2perm(3, 3), (vector<int>{1, 2, 0}));
    for (int n = 1; n <= 8; n++) {
        vector<int> p(n);
        iota(p.begin(), p.end(), 0);
        ll k = 0;
        do {
            CHECK_EQ(perm2int(p), k);
            CHECK_EQ(int2perm(n, k), p);
            k++;
        } while (next_permutation(p.begin(), p.end()));
    }
    // n = 20: 20! - 1 still fits in a long long
    ll f = 1;
    for (int i = 2; i <= 20; i++) f *= i;
    vector<int> rev(20);
    iota(rev.rbegin(), rev.rend(), 0);
    CHECK_EQ(perm2int(rev), f - 1);
    CHECK_EQ(int2perm(20, f - 1), rev);
    for (int it = 0; it < 1000; it++) {
        int n = (int)test::rnd(1, 20);
        vector<int> p(n);
        iota(p.begin(), p.end(), 0);
        shuffle(p.begin(), p.end(), test::gen);
        CHECK_EQ(int2perm(n, perm2int(p)), p);
        ll k = test::rnd(0, 2000000000000000000LL);
        if (n == 20) CHECK_EQ(perm2int(int2perm(n, k)), k);
    }
    // order is lexicographic
    for (int it = 0; it < 1000; it++) {
        int n = (int)test::rnd(2, 20);
        vector<int> p(n), q;
        iota(p.begin(), p.end(), 0);
        shuffle(p.begin(), p.end(), test::gen);
        q = p;
        shuffle(q.begin(), q.end(), test::gen);
        CHECK_EQ(p < q, perm2int(p) < perm2int(q));
    }
}
