// Title: Cartesian tree (min)
// Description: Parent of each index in the min Cartesian tree of a vector (minimum at the root, -1 for it).
// Usage:
//   vector<int> par = minCartesianTree(a);   // ties: the leftmost one is the ancestor
//   In-order traversal of the tree is 0, 1, ..., n - 1.
// Complexity: O(n).
template <class T> vector<int> minCartesianTree(const vector<T> &a) {
    int n = (int)a.size();
    vector<int> par(n, -1), st;  // st: the right spine, top = deepest
    for (int i = 0; i < n; i++) {
        int last = -1;
        while (!st.empty() && a[st.back()] > a[i]) last = st.back(), st.pop_back();
        if (last != -1) par[last] = i;  // the popped chain becomes i's left subtree
        if (!st.empty()) par[i] = st.back();
        st.push_back(i);
    }
    return par;
}
