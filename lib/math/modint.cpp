// Title: Modular integer
// Description: Integer modulo m with + - * / pow inv; static modulus, or dynamic (set at runtime).
// Usage:
//   using Mint = ModInt<998244353>;           // static modulus (or ModInt<1000000007>), 1 <= M < 2^31
//   Mint a = -5, b = 1LL << 40;               // any integer type and sign, reduced into [0, mod)
//   a + b; a - b; a * b; a / b; -a; a += 3; 2 * a; a == b; a.pow(e); a.inv(); a.val(); cin >> a; cout << a;
//   using DMint = ModInt<0>; DMint::setMod(m);   // dynamic modulus 1 <= m < 2^31 (998244353 until set);
//                                                // ModInt<-1>, ModInt<-2>, ... are independent ones
//   // inv() and / work for any modulus when gcd(val, mod) = 1, else inv() is 0; pow(-e) = inv().pow(e)
// Complexity: O(1) per + - * (one 64-bit %), O(log mod) per inv and /, O(log e) per pow.
template <int M> struct ModInt {
    static inline unsigned dyn = 998244353;  // modulus of a dynamic ModInt (M <= 0)
    unsigned v = 0;

    static constexpr int mod() {
        if constexpr (M > 0) return M;
        else return (int)dyn;
    }
    static void setMod(int m)
        requires(M <= 0)
    {
        dyn = m;
    }
    static ModInt raw(int x) {  // x already in [0, mod)
        ModInt r;
        r.v = x;
        return r;
    }

    ModInt() = default;
    template <class T>
        requires is_integral_v<T>
    ModInt(T x) {
        if constexpr (is_signed_v<T>) {
            long long r = (long long)x % mod();
            v = unsigned(r < 0 ? r + mod() : r);
        } else {
            v = unsigned((unsigned long long)x % mod());
        }
    }

    int val() const { return (int)v; }

    ModInt &operator+=(ModInt o) {
        v += o.v;
        if (v >= (unsigned)mod()) v -= mod();
        return *this;
    }
    ModInt &operator-=(ModInt o) {
        v += mod() - o.v;
        if (v >= (unsigned)mod()) v -= mod();
        return *this;
    }
    ModInt &operator*=(ModInt o) {
        v = unsigned((unsigned long long)v * o.v % mod());
        return *this;
    }
    ModInt &operator/=(ModInt o) { return *this *= o.inv(); }
    ModInt &operator++() { return *this += 1; }
    ModInt &operator--() { return *this -= 1; }
    ModInt operator++(int) {
        ModInt r = *this;
        ++*this;
        return r;
    }
    ModInt operator--(int) {
        ModInt r = *this;
        --*this;
        return r;
    }
    ModInt operator+() const { return *this; }
    ModInt operator-() const { return ModInt() - *this; }

    ModInt pow(long long e) const {
        if (e < 0) return inv().pow(-e);
        ModInt r = 1, b = *this;
        for (; e; e >>= 1, b *= b)
            if (e & 1) r *= b;
        return r;
    }
    ModInt inv() const {  // extended Euclid: x * v = a (mod m) and y * v = b (mod m) throughout
        long long a = v, b = mod(), x = 1, y = 0;
        while (b) {
            long long q = a / b;
            swap(a -= q * b, b);
            swap(x -= q * y, y);
        }
        return a == 1 ? ModInt(x) : ModInt();
    }

    friend ModInt operator+(ModInt a, ModInt b) { return a += b; }
    friend ModInt operator-(ModInt a, ModInt b) { return a -= b; }
    friend ModInt operator*(ModInt a, ModInt b) { return a *= b; }
    friend ModInt operator/(ModInt a, ModInt b) { return a /= b; }
    friend bool operator==(ModInt a, ModInt b) { return a.v == b.v; }
    friend ostream &operator<<(ostream &os, ModInt a) { return os << a.v; }
    friend istream &operator>>(istream &is, ModInt &a) {
        long long x;
        is >> x;
        a = x;
        return is;
    }
};
