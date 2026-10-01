#include "test.h"
#include "strings/aho-corasick.cpp"

string rndStr(int n, int k, char base = 'a') {
    string s(n, base);
    for (auto &c : s) c = char(base + test::rnd(0, k - 1));
    return s;
}

template <class AC> void check(const vector<string> &pats, const string &text) {
    AC ac;
    for (int i = 0; i < (int)pats.size(); i++) CHECK_EQ(ac.add(pats[i]), i);
    ac.build();
    CHECK_EQ(ac.end.size(), pats.size());
    CHECK_EQ(ac.ord.size(), ac.t.size() - 1);  // every non-root node, in BFS order
    vector<long long> want(pats.size());
    int v = 0;
    for (int i = 0; i < (int)text.size(); i++) {
        v = ac.next(v, text[i]);
        int ending = 0;  // patterns that end at position i
        for (int j = 0; j < (int)pats.size(); j++) {
            int len = (int)pats[j].size();
            if (len <= i + 1 && text.compare(i + 1 - len, len, pats[j]) == 0) ending++, want[j]++;
        }
        CHECK_EQ(ac.t[v].out, ending);
    }
    CHECK_EQ(ac.count(text), want);
    for (int j = 0; j < (int)pats.size(); j++) {  // end[j] is the node reached by reading pats[j]
        int u = 0;
        for (char c : pats[j]) u = ac.next(u, c);
        CHECK_EQ(u, ac.end[j]);
    }
}

int main() {
    check<AhoCorasick<>>({"he", "she", "his", "hers", "he"}, "ushershishe");
    check<AhoCorasick<>>({"a"}, "");
    for (int it = 0; it < 1500; it++) {
        int k = (int)test::rnd(1, 3), m = (int)test::rnd(1, 8);
        vector<string> pats(m);
        for (auto &p : pats) p = rndStr((int)test::rnd(1, 5), k);
        check<AhoCorasick<>>(pats, rndStr((int)test::rnd(0, 40), k));
    }
    for (int it = 0; it < 300; it++) {  // uppercase alphabet
        int m = (int)test::rnd(1, 6);
        vector<string> pats(m);
        for (auto &p : pats) p = rndStr((int)test::rnd(1, 4), 2, 'A');
        check<AhoCorasick<26, 'A'>>(pats, rndStr((int)test::rnd(0, 40), 2, 'A'));
    }
    // vector<int> patterns
    AhoCorasick<3, 0> ac;
    ac.add(vector<int>{0, 1}), ac.add(vector<int>{1});
    ac.build();
    CHECK_EQ(ac.count(vector<int>{0, 1, 2, 0, 1, 1}), (vector<long long>{2, 3}));
}
