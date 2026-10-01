// Title: Subset sum (bitset)
// Description: All reachable subset sums of non-negative weights, with recovery of one subset per sum.
// Usage:
//   SubsetSum ss(w);           // w: vector<int>, w[i] >= 0
//   ss.can(s)                  // is s a subset sum?
//   ss.recover(s)              // indices of one subset with sum s (requires can(s))
//   ss.maxAtMost(s)            // largest subset sum <= s (-1 if s < 0)
//   int diff = ss.total - 2 * ss.maxAtMost(ss.total / 2);   // most balanced split into two parts
// Complexity: O(n S / 64) time, O(S) memory, S = sum of weights; maxAtMost O(S).
struct SubsetSum {
    vector<int> w, par;  // par[s]: item that first reached s (-1: unreachable or s = 0)
    int total;
    SubsetSum(const vector<int> &w_) : w(w_), total(0) {
        for (int x : w) total += x;
        par.assign(total + 1, -1);
        vector<unsigned long long> bs(total / 64 + 1);
        bs[0] = 1;
        for (int i = 0; i < (int)w.size(); i++) {
            int q = w[i] / 64, r = w[i] % 64;
            for (int j = (int)bs.size() - 1; j >= 0; j--) {
                unsigned long long pre = bs[j], add = 0;
                if (j >= q) add |= (q ? bs[j - q] : pre) << r;
                if (r && j > q) add |= bs[j - q - 1] >> (64 - r);
                bs[j] |= add;
                for (unsigned long long k = bs[j] ^ pre; k; k &= k - 1) par[64 * j + __builtin_ctzll(k)] = i;
            }
        }
    }
    bool can(int s) const { return s == 0 || (0 < s && s <= total && par[s] != -1); }
    int maxAtMost(int s) const {
        for (s = min(s, total); s > 0 && par[s] == -1; s--) {}
        return max(s, -1);
    }
    vector<int> recover(int s) const {
        vector<int> res;
        while (s > 0) res.push_back(par[s]), s -= w[par[s]];
        return res;
    }
};
