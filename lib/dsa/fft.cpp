// Title: FFT convolution
// Description: Convolution of real or integer sequences with complex FFT (doubles).
// Usage:
//   vector<double> c = fftConv(a, b);            // c[k] = sum a[i] b[k - i]
//   vector<long long> c = fftConvInt(a, b);      // rounded; exact while
//       (sum a_i^2 + sum b_i^2) * log2(n) < ~9e14  (e.g. values <= 1e4 and n <= 1e5 are fine)
//   For results modulo a prime use dsa/ntt (nttConv / convMod).
// Complexity: O(n log n).
// Verify: https://judge.yosupo.jp/problem/convolution_mod
inline void fft(vector<complex<double>> &a) {
    int n = (int)a.size(), L = 31 - __builtin_clz(n);
    static vector<complex<long double>> R(2, 1);
    static vector<complex<double>> rt(2, 1);
    for (static int k = 2; k < n; k *= 2) {
        R.resize(n), rt.resize(n);
        auto x = polar(1.0L, acosl(-1) / k);
        for (int i = k; i < 2 * k; i++) rt[i] = R[i] = i & 1 ? R[i / 2] * x : R[i / 2];
    }
    vector<int> rev(n);
    for (int i = 0; i < n; i++) rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
    for (int i = 0; i < n; i++)
        if (i < rev[i]) swap(a[i], a[rev[i]]);
    for (int k = 1; k < n; k *= 2)
        for (int i = 0; i < n; i += 2 * k)
            for (int j = 0; j < k; j++) {
                auto z = rt[j + k] * a[i + j + k];
                a[i + j + k] = a[i + j] - z;
                a[i + j] += z;
            }
}

inline vector<double> fftConv(const vector<double> &a, const vector<double> &b) {
    if (a.empty() || b.empty()) return {};
    vector<double> res(a.size() + b.size() - 1);
    int L = 32 - __builtin_clz((int)res.size()), n = 1 << L;
    vector<complex<double>> in(n), out(n);
    copy(a.begin(), a.end(), in.begin());
    for (size_t i = 0; i < b.size(); i++) in[i].imag(b[i]);
    fft(in);
    for (auto &x : in) x *= x;
    for (int i = 0; i < n; i++) out[i] = in[-i & (n - 1)] - conj(in[i]);
    fft(out);
    for (size_t i = 0; i < res.size(); i++) res[i] = imag(out[i]) / (4 * n);
    return res;
}

inline vector<long long> fftConvInt(const vector<long long> &a, const vector<long long> &b) {
    auto c = fftConv(vector<double>(a.begin(), a.end()), vector<double>(b.begin(), b.end()));
    vector<long long> res(c.size());
    for (size_t i = 0; i < c.size(); i++) res[i] = llround(c[i]);
    return res;
}
