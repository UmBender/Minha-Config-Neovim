// Title: LIS length
// Description: Length of the longest strictly increasing (or non-decreasing) subsequence.
// Usage:
//   int len = lisLength(a);          // strictly increasing
//   int len = lisLength(a, false);   // non-decreasing
//   Decreasing: negate the values (or reverse the array).
// Complexity: O(n log n).
template <class T> int lisLength(const vector<T> &a, bool strict = true) {
    vector<T> tails;  // tails[k]: smallest last value of an increasing subsequence of length k + 1
    for (const T &x : a) {
        auto it = strict ? lower_bound(tails.begin(), tails.end(), x) : upper_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) tails.push_back(x);
        else *it = x;
    }
    return (int)tails.size();
}
