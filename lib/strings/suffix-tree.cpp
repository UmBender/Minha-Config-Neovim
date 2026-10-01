// Title: Suffix tree
// Description: Compressed trie of all suffixes, built from the suffix array and LCP array.
// Usage:
//   SuffixTree st(s);          // s: string or any vector<T> with < and ==
//   st.t[v]: node v (0 is the root); edge into v is s[l, r); par, depth (characters from the root)
//   st.t[v].ch: children, in lexicographic order of their edges
//   st.t[v].suf: the suffix that ends at v, or -1. Every suffix ends at a node (no sentinel
//     needed), which has children when that suffix is a prefix of another one
//   the string of v is s[r - depth, r); st.sa, st.lcp are kept
//   distinct substrings: sum of r - l over all nodes
// Complexity: O(n log n) (the suffix array), O(n) for the tree itself; at most 2n nodes.
// Requires: strings/suffix-array, strings/lcp
struct SuffixTree {
    struct Node {
        int l, r, par, suf, depth;
        vector<int> ch;
    };
    vector<Node> t;
    vector<int> sa, lcp;
    int addNode(int l, int r, int par, int suf) {
        t.push_back({l, r, par, suf, par == -1 ? 0 : t[par].depth + r - l, {}});
        if (par != -1) t[par].ch.push_back((int)t.size() - 1);
        return (int)t.size() - 1;
    }
    template <class S> SuffixTree(const S &s) : sa(suffixArray(s)), lcp(lcpArray(s, sa)) {
        int n = (int)s.size(), last = addNode(0, 0, -1, -1);
        // insert suffixes in sorted order: the new one leaves the path of the previous one at
        // depth lcp, splitting an edge there if needed
        for (int i = 0; i < n; i++) {
            int x = sa[i], k = lcp[i];
            while (t[last].depth > k) last = t[last].par;
            if (t[last].depth < k) {
                int y = t[last].ch.back(), cut = t[y].l + k - t[last].depth;
                int z = addNode(t[y].l, cut, last, -1);  // appended to last.ch, replaces y there
                t[last].ch.pop_back();
                t[last].ch.back() = z;
                t[y].par = z, t[y].l = cut;
                t[z].ch.push_back(y);
                last = z;
            }
            last = addNode(x + k, n, last, x);
        }
    }
};
