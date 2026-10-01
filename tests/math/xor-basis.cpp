#include "test.h"
#include "math/xor-basis.cpp"
using ull = unsigned long long;

int main() {
    {
        XorBasis<> xb;
        CHECK_EQ(xb.size(), 0);
        CHECK(xb.contains(0) && !xb.contains(1));
        CHECK_EQ(xb.maxXor(), 0ULL);
        CHECK_EQ(xb.kth(0), 0ULL);
        CHECK(!xb.add(0));
        CHECK(xb.add(5) && xb.add(3) && !xb.add(6));
        CHECK_EQ(xb.size(), 2);
        CHECK_EQ(xb.maxXor(), 6ULL);
        CHECK_EQ(xb.maxXor(8), 14ULL);
        CHECK_EQ(xb.minXor(7), 1ULL);
        CHECK(xb.contains(6) && !xb.contains(7));
        CHECK_EQ(xb.kth(1), 3ULL); // span {0, 3, 5, 6}
        CHECK_EQ(xb.kth(3), 6ULL);
    }
    // against the explicit span, small values
    for (int it = 0; it < 2000; it++) {
        XorBasis<6> xb;
        set<ull> span = {0};
        int k = (int)test::rnd(0, 8);
        for (int i = 0; i < k; i++) {
            ull x = test::rnd(0, 63);
            bool grows = !span.count(x);
            CHECK_EQ(xb.add(x), grows);
            set<ull> next = span;
            for (ull v : span) next.insert(v ^ x);
            span = next;
        }
        CHECK_EQ(1ULL << xb.size(), (ull)span.size());
        vector<ull> sorted(span.begin(), span.end());
        for (int i = 0; i < (int)sorted.size(); i++) CHECK_EQ(xb.kth(i), sorted[i]);
        for (ull x = 0; x < 64; x++) {
            CHECK_EQ(xb.contains(x), (bool)span.count(x));
            ull mn = ~0ULL, mx = 0;
            for (ull v : span) mn = min(mn, x ^ v), mx = max(mx, x ^ v);
            CHECK_EQ(xb.minXor(x), mn);
            CHECK_EQ(xb.maxXor(x), mx);
        }
    }
    // 64-bit values (top bit set)
    for (int it = 0; it < 200; it++) {
        XorBasis<> xb;
        vector<ull> vs;
        for (int i = 0; i < 20; i++) {
            ull x = test::gen();
            vs.push_back(x), xb.add(x);
        }
        CHECK_EQ(xb.size(), 20); // random vectors are independent w.h.p.
        ull sub = 0;
        for (ull v : vs)
            if (test::rnd(0, 1)) sub ^= v;
        CHECK(xb.contains(sub));
        CHECK(xb.maxXor() >= sub);
        CHECK_EQ(xb.kth((1ULL << 20) - 1), xb.maxXor());
        CHECK_EQ(xb.kth(0), 0ULL);
        CHECK_EQ(xb.minXor(sub), 0ULL);
    }
    // full rank 64
    XorBasis<> full;
    for (int i = 0; i < 64; i++) CHECK(full.add(1ULL << i | (i ? 1ULL << (i - 1) : 0)));
    CHECK_EQ(full.size(), 64);
    CHECK_EQ(full.maxXor(), ~0ULL);
    CHECK_EQ(full.kth(12345), 12345ULL);
}
