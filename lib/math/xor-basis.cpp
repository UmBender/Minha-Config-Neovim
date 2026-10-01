// Title: XOR basis
// Description: Linear basis of integers under XOR: insert, membership, min/max XOR, k-th smallest of the span.
// Usage:
//   XorBasis<> xb;          // values < 2^64; XorBasis<B> for values < 2^B (faster)
//   xb.add(x)               // true if x was independent (the span grew)
//   xb.contains(x)          // x is a XOR of inserted values
//   xb.maxXor(x = 0)        // max of x ^ v over v in the span; xb.minXor(x): the min
//   xb.size()               // rank: the span has 2^size() values
//   xb.kth(k)               // k-th smallest value of the span (0-indexed, k < 2^size()), kth(0) = 0
// Complexity: O(B) per operation, kth O(B^2).
template <int B = 64> struct XorBasis {
    unsigned long long b[B] = {};  // b[i]: basis vector with highest bit i, or 0
    int dim = 0;
    bool add(unsigned long long x) {
        for (int i = B - 1; i >= 0; i--)
            if (x >> i & 1) {
                if (!b[i]) {
                    b[i] = x, dim++;
                    return true;
                }
                x ^= b[i];
            }
        return false;
    }
    unsigned long long minXor(unsigned long long x) const {
        for (int i = B - 1; i >= 0; i--) x = min(x, x ^ b[i]);
        return x;
    }
    unsigned long long maxXor(unsigned long long x = 0) const {
        for (int i = B - 1; i >= 0; i--) x = max(x, x ^ b[i]);
        return x;
    }
    bool contains(unsigned long long x) const { return minXor(x) == 0; }
    int size() const { return dim; }
    unsigned long long kth(unsigned long long k) const {
        unsigned long long red[B], res = 0;  // reduced basis: no vector has another's leading bit
        for (int i = 0, t = 0; i < B; i++) {
            red[i] = b[i];
            for (int j = i - 1; j >= 0; j--)
                if (red[i] >> j & 1) red[i] ^= red[j];
            if (red[i] && (k >> t++ & 1)) res ^= red[i];  // bit t of k picks the t-th smallest vector
        }
        return res;
    }
};
