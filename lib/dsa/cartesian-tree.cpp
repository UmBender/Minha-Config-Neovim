// Title: Cartesian tree
// Description: Parent of each index in the Cartesian tree defined by a comparator; root's parent is -1.
// Usage:
//   auto par = cartesianTree(n, [&](int i, int j) { return a[i] < a[j]; });   // min at the root
//   above(i, j) (called with i > j): true if i must be an ancestor of j.
//     a[i] < a[j]  -> min-heap, ties: leftmost is the ancestor
//     a[i] <= a[j] -> min-heap, ties: rightmost is the ancestor
//     a[i] > a[j]  -> max-heap
//   In-order traversal of the tree is 0, 1, ..., n - 1.
//   Plain min / max trees from a vector: the min, max variants in the <leader>rl menu.
// Complexity: O(n).
template <class F> vector<int> cartesianTree(int n, F above) {
    vector<int> par(n, -1), st;
    for (int i = 0; i < n; i++) {
        int last = -1;
        while (!st.empty() && above(i, st.back())) last = st.back(), st.pop_back();
        if (last != -1) par[last] = i;
        if (!st.empty()) par[i] = st.back();
        st.push_back(i);
    }
    return par;
}

