#include "test.h"
#include "geometry/point.cpp"

using P = Point<long long>;
using PD = Point<long double>;

int main() {
    // sgn: exact on integers, EPS-tolerant on floating types
    CHECK_EQ(sgn(0LL), 0), CHECK_EQ(sgn(5LL), 1), CHECK_EQ(sgn(-5), -1);
    CHECK_EQ(sgn((long double)1e-12), 0), CHECK_EQ(sgn(1e-12), 0), CHECK_EQ(sgn(1e-6), 1), CHECK_EQ(sgn(-1e-6), -1);

    // aggregate construction, arithmetic, comparisons
    P a{3, 4}, b{-1, 2};
    CHECK_EQ(P{} == P(0, 0), true);
    CHECK((a + b == P{2, 6})), CHECK((a - b == P{4, 2})), CHECK((-a == P{-3, -4}));
    CHECK((a * 2 == P{6, 8})), CHECK((2 * a == P{6, 8})), CHECK((P{6, 8} / 2 == a));
    CHECK(a != b), CHECK(b < a), CHECK((P{1, 1} < P{1, 2})), CHECK(!(a < a));
    P c = a;
    c += b, CHECK((c == P{2, 6}));
    c -= b, CHECK(c == a);

    // products and lengths
    CHECK_EQ(a.dot(b), 5LL), CHECK_EQ(a.cross(b), 10LL), CHECK_EQ(b.cross(a), -10LL);
    CHECK_EQ(P(0, 0).cross(P{1, 0}, P{0, 1}), 1LL);  // left turn
    CHECK_EQ(P(1, 1).cross(P{2, 2}, P{3, 3}), 0LL);  // collinear
    CHECK_EQ(a.dist2(), 25LL), CHECK_NEAR(a.dist(), 5, 1e-12);
    CHECK((a.perp() == P{-4, 3})), CHECK_EQ(a.perp().dot(a), 0LL);

    // floating helpers
    PD u = PD{3, 4}.unit();
    CHECK_NEAR(u.x, 0.6, 1e-12), CHECK_NEAR(u.y, 0.8, 1e-12);
    PD r = PD{1, 0}.rotate(acosl(-1) / 2);
    CHECK_NEAR(r.x, 0, 1e-12), CHECK_NEAR(r.y, 1, 1e-12);
    CHECK_NEAR(PD(-1, 0).angle(), acosl(-1), 1e-12), CHECK_NEAR(P(0, -2).angle(), -acosl(-1) / 2, 1e-12);

    // conversion between coordinate types
    PD conv = PD(a);
    CHECK_NEAR(conv.x, 3, 0), CHECK_NEAR(conv.y, 4, 0);

    // stream operators
    istringstream in("7 -2");
    P q;
    in >> q;
    CHECK((q == P{7, -2}));
    ostringstream out;
    out << q;
    CHECK_EQ(out.str(), string("7 -2"));

    // random identities
    for (int it = 0; it < 2000; it++) {
        auto rp = [] { return P{test::rnd(-1e9, 1e9), test::rnd(-1e9, 1e9)}; };
        P x = rp(), y = rp(), z = rp();
        CHECK_EQ(x.cross(y), -y.cross(x));
        CHECK_EQ(x.cross(y, z), (y - x).cross(z - x));
        CHECK_EQ(x.cross(y, z), y.cross(z, x));
        CHECK_EQ(x.dot(y), x.x * y.x + x.y * y.y);
        CHECK_EQ((x - y).dist2(), (x - y).dot(x - y));
        PD xd = PD(x).rotate(test::rndReal(-4, 4));
        CHECK_NEAR(xd.dist(), PD(x).dist(), 1e-9);
    }
}
