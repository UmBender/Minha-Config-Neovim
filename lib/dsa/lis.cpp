// Title: Longest increasing subsequence
// Description: Indices of one LIS (strict or non-strict, any comparator) and LIS length ending at each index.
// Usage:
//   vector<int> idx = lis(a);                  // strictly increasing
//   lis(a, false)                              // non-decreasing
//   lis(a, true, greater<long long>())         // strictly decreasing
//   lisEnding(a, strict, cmp)[i]               // length of the longest one ending at i
//   Only the length: the length variant in the <leader>rl menu.
// Complexity: O(n log n).
template <class T, class Cmp = less<T>> vector<int> lis(const vector<T> &a, bool strict = true, Cmp cmp = Cmp()) {
    int n = (int)a.size();
    vector<int> tails, par(n, -1);  // tails[k]: index of the best tail of a subsequence of length k + 1
    for (int i = 0; i < n; i++) {
        int lo = 0, hi = (int)tails.size();
        while (lo < hi) {
            int m = (lo + hi) / 2;
            bool right = strict ? cmp(a[tails[m]], a[i]) : !cmp(a[i], a[tails[m]]);
            if (right) lo = m + 1;
            else hi = m;
        }
        par[i] = lo ? tails[lo - 1] : -1;
        if (lo == (int)tails.size()) tails.push_back(i);
        else tails[lo] = i;
    }
    vector<int> res;
    for (int i = tails.empty() ? -1 : tails.back(); i != -1; i = par[i]) res.push_back(i);
    return vector<int>(res.rbegin(), res.rend());
}

template <class T, class Cmp = less<T>> vector<int> lisEnding(const vector<T> &a, bool strict = true, Cmp cmp = Cmp()) {
    int n = (int)a.size();
    vector<T> tails;
    vector<int> len(n);
    for (int i = 0; i < n; i++) {
        auto it = strict ? lower_bound(tails.begin(), tails.end(), a[i], cmp)
                         : upper_bound(tails.begin(), tails.end(), a[i], cmp);
        len[i] = int(it - tails.begin()) + 1;
        if (it == tails.end()) tails.push_back(a[i]);
        else *it = a[i];
    }
    return len;
}

