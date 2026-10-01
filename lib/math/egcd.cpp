// Title: Extended Euclid
// Description: gcd(a, b) with x, y such that ax + by = gcd, and the modular inverse for any modulus.
// Usage:
//   long long x, y, g = egcd(a, b, x, y);   // a, b >= 0: a*x + b*y = g, |x| <= b/g, |y| <= a/g
//   invMod(a, m)                            // a^-1 mod m in [0, m), -1 if gcd(a, m) != 1 (any a, m >= 1)
// Complexity: O(log min(a, b)).
long long egcd(long long a, long long b, long long &x, long long &y) {
    if (b == 0) {
        x = 1, y = 0;
        return a;
    }
    long long g = egcd(b, a % b, y, x);
    y -= a / b * x;
    return g;
}

long long invMod(long long a, long long m) {
    long long x, y;
    if (egcd((a % m + m) % m, m, x, y) != 1) return -1;
    return (x % m + m) % m;
}
