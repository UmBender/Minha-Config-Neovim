#include "test.h"
#include "dsa/rbst.cpp"

int main() {
    // persistence: every operation returns a new root, old versions stay valid
    for (int it = 0; it < 100; it++) {
        PersistentRBST<int> t;
        vector<int> roots = {t.build({})};
        vector<vector<int>> vers = {{}};
        int init = (int)test::rnd(0, 20);
        auto v0 = test::rndVec<int>(init, 0, 99);
        roots.push_back(t.build(v0)), vers.push_back(v0);
        for (int q = 0; q < 120; q++) {
            int src = (int)test::rnd(0, (long long)roots.size() - 1);
            int root = roots[src];
            vector<int> cur = vers[src];
            int n = (int)cur.size();
            CHECK_EQ(t.size(root), (long long)n);
            int type = (int)test::rnd(0, 5);
            if (type == 0) {
                int p = (int)test::rnd(0, n), x = (int)test::rnd(0, 99);
                root = t.insert(root, p, x), cur.insert(cur.begin() + p, x);
            } else if (type == 1 && n > 0) {
                int p = (int)test::rnd(0, n - 1);
                root = t.erase(root, p), cur.erase(cur.begin() + p);
            } else if (type == 2 && n > 0) {
                int p = (int)test::rnd(0, n - 1);
                CHECK_EQ(t.get(root, p), cur[p]);
            } else if (type == 3) {
                // split + merge two versions (possibly the same one: sharing is fine)
                int other = (int)test::rnd(0, (long long)roots.size() - 1);
                int k = (int)test::rnd(0, n);
                auto [a, b] = t.split(root, k);
                CHECK_EQ(t.toVector(a), vector<int>(cur.begin(), cur.begin() + k));
                CHECK_EQ(t.toVector(b), vector<int>(cur.begin() + k, cur.end()));
                root = t.merge(b, roots[other]);
                vector<int> nxt(cur.begin() + k, cur.end());
                nxt.insert(nxt.end(), vers[other].begin(), vers[other].end());
                cur = nxt;
            } else if (type == 4 && n > 0) {
                int p = (int)test::rnd(0, n - 1), x = (int)test::rnd(0, 99);
                root = t.set(root, p, x), cur[p] = x;
            }
            CHECK_EQ(t.toVector(root), cur);
            roots.push_back(root), vers.push_back(cur);
            if (roots.size() > 40) roots.erase(roots.begin() + 2), vers.erase(vers.begin() + 2);
        }
        for (size_t i = 0; i < roots.size(); i++) CHECK_EQ(t.toVector(roots[i]), vers[i]);
    }

    // preset: T defaults to long long
    {
        PersistentRBST d;
        static_assert(is_same_v<decltype(d), PersistentRBST<long long>>);
        int r0 = d.build({1, 2, 3});
        int r1 = d.set(r0, 1, 5000000000LL);
        CHECK_EQ(d.toVector(r0), (vector<long long>{1, 2, 3}));
        CHECK_EQ(d.toVector(r1), (vector<long long>{1, 5000000000LL, 3}));
    }

    // exponential sharing: sizes beyond 2^40 via repeated self-merge
    PersistentRBST<char> t;
    int r = t.build({'a', 'b'});
    for (int i = 0; i < 45; i++) r = t.merge(r, r);
    CHECK_EQ(t.size(r), 2LL << 45);
    CHECK_EQ(t.get(r, (1LL << 44) + 1), 'b');
    CHECK_EQ(t.get(r, (2LL << 45) - 2), 'a');
}
