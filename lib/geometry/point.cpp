// Title: Point
// Description: 2D point/vector Point<T> (integer or floating T) with dot, cross, lengths, rotations; sgn with EPS.
// Usage:
//   using P = Point<long long>;     // exact: orientation tests, hulls, areas (coordinates up to ~1e9)
//   using P = Point<long double>;   // when new points are built (intersections, circles, cuts)
//   P a{1, 2}, b{3, 4};  a + b, a - b, a * k, a / k, -a, a == b, a < b (by x, then y)
//   a.dot(b), a.cross(b)            // cross > 0: b is counterclockwise from a
//   a.cross(b, c)                   // (b - a) x (c - a): > 0 left turn a -> b -> c, 0 collinear
//   a.dist2() (squared length, exact), a.dist(), a.angle() in (-pi, pi], a.perp() (rotated +90)
//   a.unit(), a.rotate(rad)         // floating T only
//   Point<long double>(a)           // convert coordinate types;  cin >> a, cout << a ("x y")
//   sgn(x): -1 / 0 / 1, exact for integers, |x| <= EPS is 0 for floating types (tune EPS per problem)
// Complexity: O(1) per operation.
const long double EPS = 1e-9;

template <class T> int sgn(T x) {
    if constexpr (is_floating_point_v<T>) return (x > EPS) - (x < -EPS);
    else return (x > 0) - (x < 0);
}

template <class T> struct Point {
    T x = 0, y = 0;
    Point operator+(Point o) const { return {x + o.x, y + o.y}; }
    Point operator-(Point o) const { return {x - o.x, y - o.y}; }
    Point operator-() const { return {-x, -y}; }
    Point operator*(T k) const { return {x * k, y * k}; }
    Point operator/(T k) const { return {x / k, y / k}; }
    friend Point operator*(T k, Point p) { return p * k; }
    Point &operator+=(Point o) { return *this = *this + o; }
    Point &operator-=(Point o) { return *this = *this - o; }
    auto operator<=>(const Point &) const = default;
    T dot(Point o) const { return x * o.x + y * o.y; }
    T cross(Point o) const { return x * o.y - y * o.x; }
    T cross(Point a, Point b) const { return (a - *this).cross(b - *this); }
    T dist2() const { return x * x + y * y; }
    long double dist() const { return sqrtl((long double)dist2()); }
    long double angle() const { return atan2l((long double)y, (long double)x); }
    Point perp() const { return {-y, x}; }
    Point unit() const { return *this / dist(); }
    Point rotate(long double a) const {
        long double c = cosl(a), s = sinl(a);
        return {T(x * c - y * s), T(x * s + y * c)};
    }
    template <class U> explicit operator Point<U>() const { return {U(x), U(y)}; }
    friend istream &operator>>(istream &in, Point &p) { return in >> p.x >> p.y; }
    friend ostream &operator<<(ostream &out, Point p) { return out << p.x << ' ' << p.y; }
};
