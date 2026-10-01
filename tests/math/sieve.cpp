#include "test.h"
#include "math/sieve.cpp"

int main() {
    {
        Sieve s(0);
        CHECK(s.primes.empty());
        Sieve t(2);
        CHECK(t.primes.empty());
        CHECK(!t.isPrime(0) && !t.isPrime(1));
        Sieve u(3);
        CHECK_EQ(u.primes, (vector<int>{2}));
    }
    Sieve s(30);
    CHECK_EQ(s.primes, (vector<int>{2, 3, 5, 7, 11, 13, 17, 19, 23, 29}));
    CHECK_EQ(s.factor(1), vector<int>{});
    CHECK_EQ(s.factor(24), (vector<int>{2, 2, 2, 3}));
    CHECK_EQ(s.factor(29), (vector<int>{29}));
    for (int n : {1, 2, 3, 10, 97, 100, 1000, 65536, 1000003}) {
        Sieve sv(n);
        vector<int> primes;
        for (int x = 0; x < n; x++) {
            int spf = 0;
            for (int d = 2; d * d <= x && !spf; d++)
                if (x % d == 0) spf = d;
            if (x >= 2 && !spf) spf = x;
            CHECK_EQ(sv.spf[x], spf);
            CHECK_EQ(sv.isPrime(x), x >= 2 && spf == x);
            if (sv.isPrime(x)) primes.push_back(x);
            if (n <= 1000 && x >= 1) {
                vector<int> fs;
                for (int y = x, d = 2; y > 1; d++)
                    while (y % d == 0) fs.push_back(d), y /= d;
                CHECK_EQ(sv.factor(x), fs);
            }
        }
        CHECK_EQ(sv.primes, primes);
    }
    Sieve big(10000001);
    CHECK_EQ((int)big.primes.size(), 664579);
    CHECK_EQ(big.factor(9699690), (vector<int>{2, 3, 5, 7, 11, 13, 17, 19}));
    CHECK_EQ(big.factor(10000000), (vector<int>{2, 2, 2, 2, 2, 2, 2, 5, 5, 5, 5, 5, 5, 5}));
}
