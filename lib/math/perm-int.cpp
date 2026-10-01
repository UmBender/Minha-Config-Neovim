// Title: Permutation rank
// Description: Lexicographic rank of a permutation of 0..n-1 among all n! permutations, and back.
// Usage:
//   perm2int({2, 1, 0})    // 5
//   int2perm(3, 5)         // {2, 1, 0}
//   // n <= 20 (20! - 1 fits in a long long)
// Complexity: O(n^2).
long long perm2int(const vector<int> &p) {
    int n = (int)p.size();
    long long r = 0;
    for (int i = 0; i < n; i++) {  // mixed radix: digit i = how many later values are smaller, base n - i
        int smaller = 0;
        for (int j = i + 1; j < n; j++) smaller += p[j] < p[i];
        r = r * (n - i) + smaller;
    }
    return r;
}

vector<int> int2perm(int n, long long k) {
    vector<int> rest(n), p;
    iota(rest.begin(), rest.end(), 0);
    vector<long long> fact(n + 1, 1);
    for (int i = 1; i < n; i++) fact[i] = fact[i - 1] * i;
    for (int i = n - 1; i >= 0; i--) {
        int j = (int)(k / fact[i]);
        k %= fact[i];
        p.push_back(rest[j]);
        rest.erase(rest.begin() + j);
    }
    return p;
}
