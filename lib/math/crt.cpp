// Title: Chinese remainder theorem
// Description: Combine congruences x = r (mod m) into one x = r (mod lcm), or report that none exists.
// Usage:
//   auto [r, m] = crt(r1, m1, r2, m2);   // x = r (mod m), 0 <= r < m = lcm(m1, m2); {0, -1} if incompatible
//   auto [r, m] = crt(rs, ms);           // a whole system (vectors); {0, 1} for an empty one
//   // moduli >= 1 (need not be coprime), residues of any sign, the lcm must fit in a long long
// Complexity: O(log min(m1, m2)) per pair.
// Requires: math/egcd
pair<long long, long long> crt(long long r1, long long m1, long long r2, long long m2) {
    r1 = (r1 % m1 + m1) % m1, r2 = (r2 % m2 + m2) % m2;
    long long x, y, g = egcd(m1, m2, x, y);
    if ((r2 - r1) % g) return {0, -1};
    long long k = m2 / g, q = ((r2 - r1) / g % k + k) % k;  // x = r1 + m1 * t, t = q / (m1 / g) (mod k)
    long long t = (long long)((__int128)q * ((x % k + k) % k) % k);
    return {r1 + m1 * t, m1 * k};
}

pair<long long, long long> crt(const vector<long long> &rs, const vector<long long> &ms) {
    pair<long long, long long> cur{0, 1};
    for (size_t i = 0; i < rs.size() && cur.second != -1; i++) cur = crt(cur.first, cur.second, rs[i], ms[i]);
    return cur;
}
