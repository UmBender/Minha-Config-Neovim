#include "test.h"
#include "strings/suffix-automaton.cpp"

template <class SAM, class S> void check(const S &s) {
    int n = (int)s.size();
    SAM sam(s);
    auto &t = sam.t;
    if (n >= 2) CHECK((int)t.size() <= 2 * n - 1);
    auto ord = sam.ord();
    CHECK_EQ(ord.size(), t.size());
    for (int i = 1; i < (int)ord.size(); i++) CHECK(t[ord[i - 1]].len <= t[ord[i]].len);
    for (int v = 1; v < (int)t.size(); v++) CHECK(t[t[v].link].len < t[v].len);
    sam.countEndpos();
    set<S> subs;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j <= n; j++) subs.insert(S(s.begin() + i, s.begin() + j));
    CHECK_EQ(sam.distinctSubstrings(), (long long)subs.size());
    for (const S &p : subs) {
        int occ = 0, first = -1;
        for (int i = 0; i + (int)p.size() <= n; i++)
            if (equal(p.begin(), p.end(), s.begin() + i)) occ++, first = first == -1 ? i : first;
        CHECK(sam.find(p) != -1);
        CHECK_EQ(sam.occurrences(p), (long long)occ);
        CHECK_EQ(sam.firstOccurrence(p), first);
    }
}

int main() {
    using Sam = SuffixAutomaton<>;
    check<Sam>(string(""));
    check<Sam>(string("a"));
    check<Sam>(string("abcbc"));
    Sam e(string("ab"));
    e.countEndpos();
    CHECK_EQ(e.find(string("ba")), -1);
    CHECK_EQ(e.occurrences(string("ba")), 0LL);
    CHECK_EQ(e.firstOccurrence(string("ba")), -1);
    CHECK_EQ(e.find(string("")), 0);
    for (int it = 0; it < 500; it++) {
        int n = (int)test::rnd(0, 20), k = (int)test::rnd(1, 3);
        string s(n, 'a');
        for (auto &c : s) c = char('a' + test::rnd(0, k - 1));
        check<Sam>(s);
        string absent = s + "z";  // 'z' never occurs in s
        Sam sam(s);
        sam.countEndpos();
        CHECK_EQ(sam.find(absent), -1);
        CHECK_EQ(sam.occurrences(absent), 0LL);
    }
    for (int it = 0; it < 200; it++) {  // custom alphabet: digits
        string s((int)test::rnd(0, 20), '0');
        for (auto &c : s) c = char('0' + test::rnd(0, 9));
        check<SuffixAutomaton<10, '0'>>(s);
    }
    for (int it = 0; it < 200; it++)  // int alphabet {0, 1, 2}
        check<SuffixAutomaton<3, 0>>(test::rndVec<int>((int)test::rnd(0, 20), 0, 2));
    SuffixAutomaton<> big(string(100000, 'a') + string(100000, 'b'));
    big.countEndpos();
    CHECK_EQ(big.occurrences(string("ab")), 1LL);
    CHECK_EQ(big.occurrences(string(3, 'a')), 99998LL);
}
