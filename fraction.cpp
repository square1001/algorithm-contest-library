long long gcd(long long x, long long y) {
	if (y == 0) return x;
	return gcd(y, x % y);
}
struct fraction {
public:
	int a, b;
	explicit fraction() : a(0), b(1) {};
	explicit fraction(int a_) : a(a_), b(1) {};
	explicit fraction(int a_, int b_) : a(a_), b(b_) {};
	bool operator==(const fraction& f) { return a == f.a && b == f.b; }
	bool operator!=(const fraction& f) { return a != f.a || b != f.b; }
	fraction& operator+=(const fraction& f) {
		long long ca = 1LL * a * f.b + 1LL * b * f.a;
		long long cb = 1LL * b * f.b;
		long long g = gcd((ca > 0 ? ca : -ca), cb);
		a = ca / g;
		b = cb / g;
		return *this;
	}
	fraction& operator-=(const fraction& f) {
		long long ca = 1LL * a * f.b - 1LL * b * f.a;
		long long cb = 1LL * b * f.b;
		long long g = gcd((ca > 0 ? ca : -ca), cb);
		a = ca / g;
		b = cb / g;
		return *this;
	}
	fraction& operator*=(const fraction& f) {
		long long ca = 1LL * a * f.a, cb = 1LL * b * f.b;
		long long g = gcd((ca > 0 ? ca : -ca), cb);
		a = ca / g;
		b = cb / g;
		return *this;
	}
	fraction& operator/=(const fraction& f) {
		long long ca = 1LL * a * f.b, cb = 1LL * b * f.a;
		if (cb < 0) ca *= -1, cb *= -1;
		long long g = gcd((ca > 0 ? ca : -ca), cb);
		a = ca / g;
		b = cb / g;
		return *this;
	}
	fraction operator+(const fraction& f) const { return fraction(*this) += f; }
	fraction operator-(const fraction& f) const { return fraction(*this) -= f; }
	fraction operator*(const fraction& f) const { return fraction(*this) *= f; }
	fraction operator/(const fraction& f) const { return fraction(*this) /= f; }
	bool operator<(const fraction& f) const { return (fraction(*this) - f).a < 0; }
	bool operator>(const fraction& f) const { return (fraction(*this) - f).a > 0; }
	bool operator<=(const fraction& f) const { return (fraction(*this) - f).a <= 0; }
	bool operator>=(const fraction& f) const { return (fraction(*this) - f).a >= 0; }
};