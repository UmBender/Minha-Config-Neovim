// Title: Aho-Corasick
// Description: Automaton matching many patterns at once: per-pattern counts, matches per position.
// Usage:
//   AhoCorasick ac;                    // AhoCorasick<10, '0'>, <3, 0>: alphabet [Base, Base + A)
//   int id = ac.add(pat);              // pat: non-empty string or vector<T>; ids are 0, 1, ...
//   ac.build();                        // after the last add
//   v = ac.next(v, c);                 // start at v = 0 (root), feed the text character by character
//   ac.t[v].out                        // patterns (with multiplicity) ending at the current position
//   ac.count(text)                     // occurrences of each pattern in text, overlapping ones too
//   ac.end[id]: node of pattern id; ac.t[v].link: suffix link; ac.ord: nodes in BFS order
//   (reverse ac.ord to push values from a node to its link, e.g. counts per node)
// Complexity: O(sum |pat| * A) to build, O(1) per next, O(|text| + nodes) for count.
template <int A = 26, int Base = 'a'> struct AhoCorasick {
    struct Node {
        array<int, A> next;
        int link = 0, out = 0;
        Node() { next.fill(-1); }
    };
    vector<Node> t{Node()};
    vector<int> end, ord;
    template <class S> int add(const S &s) {
        int v = 0;
        for (auto ch : s) {
            int c = ch - Base;
            if (t[v].next[c] == -1) t[v].next[c] = (int)t.size(), t.emplace_back();
            v = t[v].next[c];
        }
        t[v].out++;
        end.push_back(v);
        return (int)end.size() - 1;
    }
    void build() {  // BFS: the link of a node is closer to the root, so it is already done
        ord.clear();
        queue<int> q;
        for (int c = 0; c < A; c++) {
            int &u = t[0].next[c];
            if (u == -1) u = 0;
            else q.push(u);
        }
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            ord.push_back(v);
            t[v].out += t[t[v].link].out;
            for (int c = 0; c < A; c++) {
                int &u = t[v].next[c];
                if (u == -1) u = t[t[v].link].next[c];
                else t[u].link = t[t[v].link].next[c], q.push(u);
            }
        }
    }
    int next(int v, int ch) const { return t[v].next[ch - Base]; }
    template <class S> vector<long long> count(const S &text) const {
        vector<long long> hits(t.size()), res(end.size());
        int v = 0;
        for (auto ch : text) hits[v = next(v, ch)]++;
        for (int i = (int)ord.size() - 1; i >= 0; i--) hits[t[ord[i]].link] += hits[ord[i]];
        for (int i = 0; i < (int)end.size(); i++) res[i] = hits[end[i]];
        return res;
    }
};
