#include "test.h"
#include "math/modint.cpp"
using ll = long long;

ll red(__int128 x, ll m) { return (ll)((x % m + m) % m); }

ll naivePow(ll b, ll e, ll m) {
    ll r = 1 % m;
    while (e--) r = red((__int128)r * b, m);
    return r;
}

// a random value of any sign and size, sometimes near the modulus or at the extremes
ll anyValue(ll m) {
    switch (test::rnd(0, 5)) {
    case 0: return test::rnd(-3, 3);
    case 1: return test::rnd(m - 3, m + 3);
    case 2: return -test::rnd(m - 3, m + 3);
    case 3: return test::rnd(0, 1) ? LLONG_MAX - test::rnd(0, 3) : LLONG_MIN + test::rnd(0, 3);
    default: return test::rnd(LLONG_MIN / 2, LLONG_MAX / 2);
    }
}

// stress every operation of Mint (whose modulus is already set) against __int128 arithmetic
template <class Mint> void stress(int iters) {
    ll m = Mint::mod();
    for (int it = 0; it < iters; it++) {
        ll x = anyValue(m), y = anyValue(m), rx = red(x, m), ry = red(y, m);
        Mint a = x, b = y;
        CHECK_EQ((ll)a.val(), rx);
        CHECK_EQ((ll)(a + b).val(), red((__int128)rx + ry, m));
        CHECK_EQ((ll)(a - b).val(), red((__int128)rx - ry, m));
        CHECK_EQ((ll)(a * b).val(), red((__int128)rx * ry, m));
        CHECK_EQ((ll)(-a).val(), red(-rx, m));
        CHECK_EQ((ll)(+a).val(), rx);
        CHECK_EQ(a == b, rx == ry);
        CHECK_EQ(a != b, rx != ry);
        Mint c = a;
        CHECK_EQ((ll)(++c).val(), red(rx + 1, m));
        CHECK_EQ((ll)(c--).val(), red(rx + 1, m));
        CHECK_EQ((ll)c.val(), rx);
        CHECK_EQ((ll)(--c).val(), red(rx - 1, m));
        CHECK_EQ((ll)(c++).val(), red(rx - 1, m));
        CHECK(c == a);
        // compound assignment and mixed int operands
        c += b, c *= 3, c -= 5, c = 2 * c + 1;
        CHECK_EQ((ll)c.val(), red(((__int128)(rx + ry) * 3 - 5) * 2 + 1, m));
        // inverse: any modulus, 0 when not invertible
        ll e = test::rnd(0, 60);
        CHECK_EQ((ll)a.pow(e).val(), naivePow(rx, e, m));
        if (gcd(rx, m) == 1) {
            CHECK((a.inv() * a).val() == 1 % m);
            CHECK(a.inv() == Mint(1) / a);
            CHECK(b / a * a == b);
            CHECK(a.pow(-e) * a.pow(e) == Mint(1));
            Mint d = b;
            d /= a;
            CHECK(d * a == b);
        } else {
            CHECK_EQ(a.inv().val(), 0);
        }
    }
}

template <class Mint> void fixed() {
    ll m = Mint::mod();
    CHECK_EQ(Mint().val(), 0);
    CHECK_EQ((ll)Mint(-1).val(), m - 1);
    CHECK_EQ((ll)Mint(m).val(), 0LL);
    CHECK_EQ((ll)Mint(m - 1).val(), m - 1);
    CHECK_EQ((ll)Mint(-m).val(), 0LL);
    CHECK_EQ((ll)Mint(LLONG_MIN).val(), red(LLONG_MIN, m));
    CHECK_EQ((ll)Mint(ULLONG_MAX).val(), (ll)(ULLONG_MAX % (unsigned long long)m));
    CHECK_EQ((ll)Mint(4294967295).val(), (ll)(4294967295u % (unsigned long long)m));
    CHECK_EQ((ll)Mint((char)-5).val(), red(-5, m));
    CHECK_EQ(Mint::raw(0).val(), 0);
    CHECK_EQ((ll)Mint::raw((int)(m - 1)).val(), m - 1);
    CHECK(Mint(7).pow(0) == Mint(1));
    CHECK(Mint(0).pow(0) == Mint(1));
    CHECK(Mint(0).inv() == Mint(0));
    // streams
    ostringstream os;
    os << Mint(-1) << ' ' << Mint(5);
    CHECK_EQ(os.str(), to_string(m - 1) + ' ' + to_string(5 % m));
    istringstream is("-7 123456789012345");
    Mint x, y;
    is >> x >> y;
    CHECK_EQ((ll)x.val(), red(-7, m));
    CHECK_EQ((ll)y.val(), red(123456789012345LL, m));
}

int main() {
    // static moduli: primes, composites, 1 and the largest allowed
    using M998 = ModInt<998244353>;
    using M107 = ModInt<1000000007>;
    static_assert(M998::mod() == 998244353 && M107::mod() == 1000000007);
    CHECK_EQ(M998(3).pow(998244352).val(), 1);  // Fermat
    CHECK_EQ(M107(2).inv().val(), 500000004);
    CHECK_EQ(M107(1) / 3 * 3, M107(1));
    CHECK_EQ(ModInt<10>(3).inv().val(), 7);  // composite modulus
    CHECK_EQ(ModInt<10>(4).inv().val(), 0);  // gcd(4, 10) = 2: not invertible
    CHECK_EQ(ModInt<1>(12345).val(), 0);
    CHECK_EQ(ModInt<2147483647>(-1) * ModInt<2147483647>(-1), ModInt<2147483647>(1));
    fixed<M998>(), fixed<M107>(), fixed<ModInt<10>>(), fixed<ModInt<1>>(), fixed<ModInt<2147483647>>();
    stress<M998>(20000), stress<M107>(20000), stress<ModInt<1>>(500), stress<ModInt<2>>(500);
    stress<ModInt<6>>(2000), stress<ModInt<1 << 30>>(5000), stress<ModInt<2147483647>>(20000);
    stress<ModInt<2147483646>>(5000);

    // dynamic moduli: default 998244353, ids are independent
    using D0 = ModInt<0>;
    using D1 = ModInt<-1>;
    CHECK_EQ(D0::mod(), 998244353);
    D0::setMod(1000000007);
    D1::setMod(10);
    CHECK_EQ(D0::mod(), 1000000007);
    CHECK_EQ(D1::mod(), 10);
    CHECK_EQ(D0(2).inv().val(), 500000004);
    CHECK_EQ(D1(3).inv().val(), 7);
    CHECK_EQ((D1(7) * 9).val(), 3);
    fixed<D0>(), fixed<D1>();
    for (int it = 0; it < 300; it++) {
        ll m = it % 3 == 0 ? test::rnd(1, 50) : test::rnd(1, 2147483647LL);
        D0::setMod((int)m);
        CHECK_EQ((ll)D0::mod(), m);
        stress<D0>(200);
        fixed<D0>();
    }
    for (int m : {1, 2, 3, 4, 998244353, 1000000007, 2147483647}) D1::setMod(m), stress<D1>(2000);
}
