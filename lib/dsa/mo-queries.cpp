// Title: Mo's algorithm
// Description: Answer offline range queries [l, r) by moving a window with add/remove callbacks.
// Usage:
//   vector<pair<int, int>> qs;   // [l, r), 0 <= l <= r <= n
//   mo(qs, n, add, remove, answer);                 // add(i) / remove(i): element i enters / leaves
//   mo(qs, n, addLeft, addRight, removeLeft, removeRight, answer);   // when the side matters
//   answer(qi) is called once per query, when the window is exactly qs[qi]
//   moOrder(qs, n)   just the processing order
//   Example (distinct values):
//     mo(qs, n, [&](int i) { d += cnt[a[i]]++ == 0; }, [&](int i) { d -= --cnt[a[i]] == 0; },
//        [&](int qi) { ans[qi] = d; });
// Complexity: O(n sqrt(q)) callback calls.
inline vector<int> moOrder(const vector<pair<int, int>> &q, int n) {
    int nq = (int)q.size();
    int block = max(1, (int)(n / max(1.0, sqrt((double)nq))));
    vector<int> ord(nq);
    iota(ord.begin(), ord.end(), 0);
    sort(ord.begin(), ord.end(), [&](int i, int j) {
        int bi = q[i].first / block, bj = q[j].first / block;
        if (bi != bj) return bi < bj;
        return bi & 1 ? q[i].second > q[j].second : q[i].second < q[j].second;
    });
    return ord;
}

template <class AL, class AR, class RL, class RR, class Ans>
void mo(const vector<pair<int, int>> &q, int n, AL addLeft, AR addRight, RL removeLeft, RR removeRight, Ans answer) {
    int L = 0, R = 0;
    for (int qi : moOrder(q, n)) {
        auto [l, r] = q[qi];
        while (L > l) addLeft(--L);
        while (R < r) addRight(R++);
        while (L < l) removeLeft(L++);
        while (R > r) removeRight(--R);
        answer(qi);
    }
}

template <class Add, class Rem, class Ans>
void mo(const vector<pair<int, int>> &q, int n, Add add, Rem remove, Ans answer) {
    mo(q, n, add, add, remove, remove, answer);
}
