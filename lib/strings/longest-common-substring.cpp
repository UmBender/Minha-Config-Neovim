// Title: Longest common substring
// Description: Longest string that is a substring of both a and b, with its positions.
// Usage:
//   auto [i, j, len] = longestCommonSubstring(a, b);   // a.substr(i, len) == b.substr(j, len)
//   longestCommonSubstring<10, '0'>(a, b)               // alphabet as in strings/suffix-automaton
//   len == 0 (and i == j == 0) when there is no common character
// Complexity: O(|a| * A + |b|).
// Requires: strings/suffix-automaton
template <int A = 26, int Base = 'a', class S> array<int, 3> longestCommonSubstring(const S &a, const S &b) {
    SuffixAutomaton<A, Base> sam(a);
    auto &t = sam.t;
    int v = 0, len = 0;
    array<int, 3> best{0, 0, 0};
    // walk b on the automaton of a; (v, len): longest suffix of b[0..j] that occurs in a
    for (int j = 0; j < (int)b.size(); j++) {
        int c = b[j] - Base;
        while (v && t[v].next[c] == -1) v = t[v].link, len = t[v].len;
        if (t[v].next[c] != -1) v = t[v].next[c], len++;
        if (len > best[2]) best = {t[v].first - len + 1, j - len + 1, len};
    }
    return best;
}
