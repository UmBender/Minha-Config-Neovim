#include "test.h"
#include "strings/z-function.cpp"
#include "strings/runs.cpp"

// every maximal run, from the definition: for each p, maximal stretches with s[i] == s[i + p];
// an interval is a run only for its smallest period
template <class S> vector<array<int, 3>> bruteRuns(const S &s) {
    int n = (int)s.size();
    map<pair<int, int>, int> best;
    for (int p = 1; p <= n; p++)
        for (int a = 0; a + p < n;) {
            if (s[a] != s[a + p]) {
                a++;
                continue;
            }
            int b = a;
            while (b + p < n && s[b] == s[b + p]) b++;
            if (b + p - a >= 2 * p && !best.count({a, b + p})) best[{a, b + p}] = p;
            a = b;
        }
    vector<array<int, 3>> res;
    for (auto [lr, p] : best) res.push_back({p, lr.first, lr.second});
    sort(res.begin(), res.end());
    return res;
}

int main() {
    CHECK_EQ(runs(string("")), (vector<array<int, 3>>{}));
    CHECK_EQ(runs(string("ab")), (vector<array<int, 3>>{}));
    CHECK_EQ(runs(string("aabab")), (vector<array<int, 3>>{{1, 0, 2}, {2, 1, 5}}));
    for (int it = 0; it < 3000; it++) {
        int n = (int)test::rnd(0, 40), k = (int)test::rnd(1, 3);
        string s(n, 'a');
        for (auto &c : s) c = char('a' + test::rnd(0, k - 1));
        CHECK_EQ(runs(s), bruteRuns(s));
    }
    string f = "a", g = "ab";  // Fibonacci words: many overlapping runs
    while (g.size() < 300) tie(f, g) = pair{g, g + f};
    CHECK_EQ(runs(g), bruteRuns(g));
    for (int it = 0; it < 200; it++) {
        auto s = test::rndVec<int>((int)test::rnd(0, 40), 0, 1);
        CHECK_EQ(runs(s), bruteRuns(s));
    }
}
