#include "test.h"
#include "strings/suffix-array.cpp"
#include "strings/lcp.cpp"
#include "strings/suffix-tree.cpp"

template <class S> void check(const S &s) {
    int n = (int)s.size();
    SuffixTree st(s);
    auto &t = st.t;
    CHECK(!t.empty());
    CHECK_EQ(t[0].par, -1);
    CHECK_EQ(t[0].depth, 0);
    CHECK_EQ(t[0].l, t[0].r);
    vector<S> path(t.size());  // brute: concatenate edge labels from the root
    vector<int> sufNode(n, -1);
    long long edgeChars = 0;
    set<S> subs;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j <= n; j++) subs.insert(S(s.begin() + i, s.begin() + j));
    vector<int> order = {0};  // BFS from the root, so parents come first
    for (int i = 0; i < (int)order.size(); i++)
        for (int c : t[order[i]].ch) order.push_back(c);
    CHECK_EQ(order.size(), t.size());  // a tree: every node reached exactly once
    CHECK(t.size() <= max<size_t>(1, 2 * n));
    for (int v : order) {
        auto &x = t[v];
        if (v) {
            CHECK(0 <= x.l && x.l < x.r && x.r <= n);
            path[v] = path[x.par];
            path[v].insert(path[v].end(), s.begin() + x.l, s.begin() + x.r);
            CHECK_EQ(x.depth, (int)path[v].size());
            CHECK(path[v] == S(s.begin() + x.r - x.depth, s.begin() + x.r));
            CHECK(subs.count(path[v]));
            edgeChars += x.r - x.l;
            if (x.suf == -1) CHECK(x.ch.size() >= 2);  // internal nodes branch
        }
        if (x.suf != -1) {
            CHECK(0 <= x.suf && x.suf < n && sufNode[x.suf] == -1);
            sufNode[x.suf] = v;
            CHECK(path[v] == S(s.begin() + x.suf, s.end()));
        }
        for (int i = 0; i < (int)x.ch.size(); i++) {
            CHECK_EQ(t[x.ch[i]].par, v);
            if (i) CHECK(s[t[x.ch[i - 1]].l] < s[t[x.ch[i]].l]);  // lexicographic order
        }
    }
    for (int i = 0; i < n; i++) CHECK(sufNode[i] != -1);
    CHECK_EQ(edgeChars, (long long)subs.size());  // every distinct substring exactly once
    CHECK_EQ(st.sa, suffixArray(s));
}

int main() {
    check(string(""));
    check(string("a"));
    check(string("aaaa"));
    check(string("banana"));
    check(string("abcabxabcd"));
    for (int it = 0; it < 1500; it++) {
        int n = (int)test::rnd(1, 25), k = (int)test::rnd(1, 3);
        string s(n, 'a');
        for (auto &c : s) c = char('a' + test::rnd(0, k - 1));
        check(s);
    }
    for (int it = 0; it < 200; it++) check(test::rndVec<int>((int)test::rnd(1, 20), 0, 2));
}
