#include "test.h"
#include "geometry/point.cpp"
#include "geometry/closest-pair.cpp"

template <class T> static T naiveBest(const vector<Point<T>> &p) {
    T best = numeric_limits<T>::max();
    for (int i = 0; i < (int)p.size(); i++)
        for (int j = i + 1; j < (int)p.size(); j++) best = min(best, (p[i] - p[j]).dist2());
    return best;
}

template <class T> static void check(const vector<Point<T>> &p) {
    auto [i, j] = closestPair(p);
    CHECK(0 <= i && i < j && j < (int)p.size());
    if constexpr (is_integral_v<T>) CHECK_EQ((p[i] - p[j]).dist2(), naiveBest(p));
    else CHECK_NEAR((p[i] - p[j]).dist2(), naiveBest(p), 1e-12);
}

using P = Point<long long>;

int main() {
    // fixed cases
    CHECK_EQ(closestPair(vector<P>{{0, 0}, {5, 5}}), make_pair(0, 1));
    CHECK_EQ(closestPair(vector<P>{{0, 0}, {10, 0}, {3, 4}, {11, 1}}), make_pair(1, 3));
    CHECK_EQ(closestPair(vector<P>{{7, 7}, {1, 2}, {9, 9}, {1, 2}}), make_pair(1, 3));  // duplicates
    CHECK_EQ(closestPair(vector<P>{{0, 0}, {0, 1}, {0, 3}, {0, 6}}), make_pair(0, 1));  // vertical line

    for (int it = 0; it < 3000; it++) {
        int n = (int)test::rnd(2, 40);
        long long c = vector<long long>{2, 20, 1000, 1000000000}[it % 4];
        vector<P> p(n);
        for (auto &q : p) q = P{test::rnd(-c, c), test::rnd(-c, c)};
        if (it % 7 == 0)  // all on a vertical line: the sweep window keeps everything
            for (auto &q : p) q.x = 5;
        check(p);
    }
    for (int it = 0; it < 500; it++) {
        int n = (int)test::rnd(2, 40);
        long double c = vector<long double>{1e-3L, 1, 1e6L}[it % 3];
        vector<Point<long double>> p(n);
        for (auto &q : p) q = {(long double)test::rndReal(-c, c), (long double)test::rndReal(-c, c)};
        check(p);
    }
    for (int it = 0; it < 3; it++) {  // big, all coordinates distinct or many duplicates
        int n = 3000;
        long long c = it ? 1000000000 : 40;
        vector<P> p(n);
        for (auto &q : p) q = P{test::rnd(-c, c), test::rnd(-c, c)};
        check(p);
    }
    // a vertical line of 2e5 points must stay fast
    vector<P> p(200000);
    for (int i = 0; i < (int)p.size(); i++) p[i] = P{0, 3LL * i};
    p.push_back(P{1, 300001});
    CHECK_EQ(closestPair(p), make_pair(100000, 200000));
}
