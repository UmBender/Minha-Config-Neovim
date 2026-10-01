// Title: 2-SAT
// Description: 2-SAT with clauses, implications, fixed values and at-most-one constraints.
// Usage:
//   TwoSat ts(n);                     // variables 0..n-1, a literal is (variable, value)
//   ts.either(a, va, b, vb);          // x[a] == va || x[b] == vb
//   ts.implies(a, va, b, vb);         // x[a] == va  =>  x[b] == vb
//   ts.mustBe(a, va); ts.equal(a, b); ts.different(a, b);
//   ts.atMostOne({{a, va}, {b, vb}, ...});   // adds auxiliary variables
//   int v = ts.addVar();
//   if (ts.solve()) ts.value[i];      // one satisfying assignment (value.size() >= n)
// Complexity: O(n + clauses), atMostOne is linear in its size.
// Verify: https://judge.yosupo.jp/problem/two_sat
// Requires: graph/scc
struct TwoSat {
    int n;
    vector<vector<int>> g;
    vector<bool> value;
    TwoSat(int n_ = 0) : n(n_), g(2 * n_) {}
    static int lit(int v, bool val) { return 2 * v + !val; }  // lit ^ 1 is the negation
    int addVar() {
        g.emplace_back(), g.emplace_back();
        return n++;
    }
    void addImpl(int p, int q) {  // p => q and its contrapositive
        g[p].push_back(q), g[q ^ 1].push_back(p ^ 1);
    }
    void either(int a, bool va, int b, bool vb) { addImpl(lit(a, !va), lit(b, vb)); }
    void implies(int a, bool va, int b, bool vb) { addImpl(lit(a, va), lit(b, vb)); }
    void mustBe(int a, bool va) { either(a, va, a, va); }
    void equal(int a, int b) { implies(a, true, b, true), implies(b, true, a, true); }
    void different(int a, int b) { either(a, true, b, true), either(a, false, b, false); }
    void atMostOne(const vector<pair<int, bool>> &lits) {
        if (lits.size() <= 1) return;
        int cur = lit(lits[0].first, lits[0].second);
        for (int i = 1; i < (int)lits.size(); i++) {
            int x = lit(lits[i].first, lits[i].second), pre = lit(addVar(), true);
            addImpl(cur, pre), addImpl(x, pre), addImpl(cur, x ^ 1);
            cur = pre;
        }
    }
    bool solve() {
        SCC s(g);
        value.assign(n, false);
        for (int v = 0; v < n; v++) {
            int t = s.comp[lit(v, true)], f = s.comp[lit(v, false)];
            if (t == f) return false;
            value[v] = t > f;  // the literal later in topological order is true
        }
        return true;
    }
};
