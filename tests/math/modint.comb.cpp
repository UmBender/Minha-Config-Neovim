#include "test.h"
#include "math/modint.cpp"
#include "math/modint.comb.cpp"
using ll = long long;

template <class Mint> void check(int n, int queries) {
    ll p = Mint::mod();
    vector<vector<ll>> pascal(n + 1, vector<ll>(n + 1, 0));  // brute force: Pascal's triangle mod p
    for (int i = 0; i <= n; i++) {
        pascal[i][0] = 1 % p;
        for (int j = 1; j <= i; j++) pascal[i][j] = (pascal[i - 1][j - 1] + pascal[i - 1][j]) % p;
    }
    Comb<Mint> cb(test::rnd(0, 1) ? n : 0);  // prebuilt or grown on demand
    for (int q = 0; q < queries; q++) {
        int a = (int)test::rnd(-3, n), b = (int)test::rnd(-3, n + 3);
        ll want = a >= 0 && b >= 0 && b <= a ? pascal[a][b] : 0;
        CHECK_EQ((ll)cb.C(a, b).val(), want);
        ll perm = a >= 0 && b >= 0 && b <= a ? 1 : 0;  // a (a-1) ... (a-b+1)
        for (int i = 0; perm && i < b; i++) perm = perm * (a - i) % p;
        CHECK_EQ((ll)cb.P(a, b).val(), perm % p);
        int i = (int)test::rnd(0, n);
        ll f = 1;
        for (int j = 2; j <= i; j++) f = f * j % p;
        CHECK_EQ((ll)cb.fact(i).val(), f % p);
        CHECK(cb.ifact(i) * cb.fact(i) == Mint(1));
        if (i > 0) CHECK(cb.inv(i) * i == Mint(1));
    }
}

int main() {
    using M998 = ModInt<998244353>;
    using M107 = ModInt<1000000007>;
    Comb<M998> cb(10);
    CHECK_EQ(cb.C(5, 2).val(), 10);
    CHECK_EQ(cb.C(0, 0).val(), 1);
    CHECK_EQ(cb.C(3, 4).val(), 0);
    CHECK_EQ(cb.C(3, -1).val(), 0);
    CHECK_EQ(cb.C(-1, 0).val(), 0);
    CHECK_EQ(cb.P(5, 2).val(), 20);
    CHECK_EQ(cb.inv(2).val(), 499122177);
    CHECK_EQ(cb.fact(20).val(), 401576539);  // 20! mod 998244353, beyond the initial size
    CHECK_EQ(cb.C(1000000, 500000), cb.fact(1000000) * cb.ifact(500000) * cb.ifact(500000));
    // inverse table: i * inv(i) = 1 for every i
    Comb<M107> big(200000);
    for (int i = 1; i <= 200000; i++) CHECK(big.inv(i) * i == M107(1));
    check<M998>(300, 20000), check<M107>(300, 20000), check<ModInt<7>>(6, 2000), check<ModInt<2>>(1, 200);
    using D = ModInt<0>;  // dynamic modulus, a fresh table per modulus
    for (int p : {2, 3, 5, 13, 101, 1009, 2147483647}) {
        D::setMod(p);
        check<D>(min(p - 1, 200), 3000);
    }
}
