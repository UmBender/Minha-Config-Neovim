// Title: Suffix automaton
// Description: Minimal automaton of all substrings: occurrences, first position, distinct count.
// Usage:
//   SuffixAutomaton sam(s);            // or sam.extend(c) one character at a time
//   SuffixAutomaton<10, '0'> d(digits); SuffixAutomaton<3, 0> v(vec);  // alphabet [Base, Base + A)
//   sam.t[v]: len (longest string of v), link (suffix link), next[c - Base] (-1 if none),
//     first (end of the first occurrence of v's strings), cnt (see countEndpos)
//   sam.find(p)                        // state reached by reading p, -1 if p isn't a substring
//   sam.firstOccurrence(p)             // start of the first occurrence of p, or -1
//   sam.countEndpos(); sam.occurrences(p)   // countEndpos once, after the last extend
//   sam.distinctSubstrings()           // non-empty ones
//   sam.ord()                          // states by increasing len (reverse it for DP over links)
// Complexity: O(n * A) to build (at most 2n - 1 states), O(|p|) per query.
// Verify: https://judge.yosupo.jp/problem/number_of_substrings
template <int A = 26, int Base = 'a'> struct SuffixAutomaton {
    struct Node {
        int len = 0, link = -1, first = -1;
        long long cnt = 0;
        array<int, A> next;
        Node() { next.fill(-1); }
    };
    vector<Node> t{Node()};
    int last = 0;
    SuffixAutomaton() {}
    template <class S> SuffixAutomaton(const S &s) {
        for (auto c : s) extend(c);
    }
    void extend(int ch) {
        int c = ch - Base, cur = (int)t.size(), p = last;
        t.emplace_back();
        t[cur].len = t[last].len + 1, t[cur].first = t[last].len, t[cur].cnt = 1;
        for (; p != -1 && t[p].next[c] == -1; p = t[p].link) t[p].next[c] = cur;
        if (p == -1) t[cur].link = 0;
        else {
            int q = t[p].next[c];
            if (t[p].len + 1 == t[q].len) t[cur].link = q;
            else {  // split q: the clone keeps the strings of length <= len(p) + 1
                int cl = (int)t.size();
                t.push_back(t[q]);
                t[cl].len = t[p].len + 1, t[cl].cnt = 0;
                for (; p != -1 && t[p].next[c] == q; p = t[p].link) t[p].next[c] = cl;
                t[q].link = t[cur].link = cl;
            }
        }
        last = cur;
    }
    vector<int> ord() const {  // counting sort by len
        int n = (int)t.size();
        vector<int> cnt(t[last].len + 2), res(n);
        for (auto &u : t) cnt[u.len + 1]++;
        for (int i = 1; i < (int)cnt.size(); i++) cnt[i] += cnt[i - 1];
        for (int v = 0; v < n; v++) res[cnt[t[v].len]++] = v;
        return res;
    }
    void countEndpos() {  // cnt[v] = number of occurrences of v's strings
        auto o = ord();
        for (int i = (int)o.size() - 1; i > 0; i--) t[t[o[i]].link].cnt += t[o[i]].cnt;
    }
    template <class S> int find(const S &p) const {
        int v = 0;
        for (auto c : p) {
            int x = c - Base;
            if (x < 0 || x >= A || (v = t[v].next[x]) == -1) return -1;
        }
        return v;
    }
    template <class S> long long occurrences(const S &p) const {
        int v = find(p);
        return v == -1 ? 0 : t[v].cnt;
    }
    template <class S> int firstOccurrence(const S &p) const {
        int v = find(p);
        return v == -1 ? -1 : t[v].first - (int)p.size() + 1;
    }
    long long distinctSubstrings() const {
        long long res = 0;
        for (int v = 1; v < (int)t.size(); v++) res += t[v].len - t[t[v].link].len;
        return res;
    }
};
