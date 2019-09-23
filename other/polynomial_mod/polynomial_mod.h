#ifndef CLASS_POLYNOMIAL_MOD
#define CLASS_POLYNOMIAL_MOD

#include "polynomial_ntt.h"

template<const singlebit mod, const singlebit depth, const singlebit primroot>
class polynomial_mod {
public:
	using modulo = fast_modint<mod>;
	using ntt = polynomial_ntt<mod, depth, primroot>;
protected:
	std::size_t sz;
	std::vector<modulo> a;
public:
	explicit polynomial_mod() : sz(1), a(std::vector<modulo>({ modulo() })) {};
	explicit polynomial_mod(std::size_t sz_) : sz(sz_), a(std::vector<modulo>(sz_, modulo())) {};
	explicit polynomial_mod(std::vector<modulo> a_) : sz(a_.size()), a(a_) {};
	polynomial_mod& operator=(const polynomial_mod& p) {
		sz = p.sz;
		a = p.a;
		return (*this);
	}
	std::size_t size() const { return sz; }
	std::size_t degree() const { return sz - 1; }
	modulo operator[](std::size_t idx) const {
		return a[idx];
	}
	modulo& operator[](std::size_t idx) {
		return a[idx];
	}
	bool operator==(const polynomial_mod& p) const {
		for (std::size_t i = 0; i < sz || i < p.sz; ++i) {
			if ((i < sz ? a[i] : modulo(0)) != (i < p.sz ? p.a[i] : modulo(0))) {
				return false;
			}
		}
		return true;
	}
	bool operator!=(const polynomial_mod& p) const {
		return !(operator==(p));
	}
	polynomial_mod resize_transform(std::size_t d) const {
		// Resize polynomial to d: in other words, f(x) := f(x) mod x^d
		polynomial_mod ans(*this);
		ans.sz = d;
		ans.a.resize(d, modulo(0));
		return ans;
	}
	polynomial_mod star_transform() const {
		// f*(x) = x^degree * f(1/x)
		polynomial_mod ans(*this);
		std::reverse(ans.a.begin(), ans.a.end());
		return ans;
	}
	polynomial_mod inverse(std::size_t d) const {
		// Find g(x) where g(x) * f(x) = 1 (mod x^d)
		polynomial_mod ans(std::vector<modulo>({ a[0].inv() }));
		while (ans.size() < d) {
			polynomial_mod nxt({ modulo(2) });
			nxt -= ans * resize_transform(ans.size() * 2);
			nxt *= ans;
			ans = nxt.resize_transform(ans.size() * 2);
		}
		ans = ans.resize_transform(d);
		return ans;
	}
	polynomial_mod& operator+=(const polynomial_mod& p) {
		sz = std::max(sz, p.sz);
		a.resize(sz);
		for (std::size_t i = 0; i < sz; ++i) a[i] += p.a[i];
		return (*this);
	}
	polynomial_mod& operator-=(const polynomial_mod& p) {
		sz = std::max(sz, p.sz);
		a.resize(sz);
		for (std::size_t i = 0; i < sz; ++i) a[i] -= p.a[i];
		return (*this);
	}
	polynomial_mod& operator*=(const polynomial_mod& p) {
		a = ntt::convolve(a, p.a);
		sz += p.sz - 1;
		return (*this);
	}
	polynomial_mod& operator/=(const polynomial_mod& p) {
		// Expected Time: 4 * multiply(p)
		std::size_t dn = degree(), dm = p.degree();
		if (dn < dm) (*this) = polynomial_mod();
		else {
			polynomial_mod gstar = p.star_transform().inverse(dn - dm + 1);
			polynomial_mod qstar = (gstar * (*this).star_transform()).resize_transform(dn - dm + 1);
			(*this) = qstar.star_transform();
		}
		return (*this);
	}
	polynomial_mod& operator%=(const polynomial_mod& p) {
		// Expected Time: 5 * multiply(p)
		(*this) -= polynomial_mod(*this) / p * p;
		(*this) = (*this).resize_transform(p.size() - 1);
		return (*this);
	}
	polynomial_mod operator+() const {
		return polynomial_mod(*this);
	}
	polynomial_mod operator-() const {
		return polynomial_mod() - polynomial_mod(*this);
	}
	polynomial_mod operator+(const polynomial_mod& p) const {
		return polynomial_mod(*this) += p;
	}
	polynomial_mod operator-(const polynomial_mod& p) const {
		return polynomial_mod(*this) -= p;
	}
	polynomial_mod operator*(const polynomial_mod& p) const {
		return polynomial_mod(*this) *= p;
	}
	polynomial_mod operator/(const polynomial_mod& p) const {
		return polynomial_mod(*this) /= p;
	}
	polynomial_mod operator%(const polynomial_mod& p) const {
		return polynomial_mod(*this) %= p;
	}
};

#endif // CLASS_POLYNOMIAL_MOD

/*
	Assertion:
	- "mod" should be a prime number, less than 2^(digit_level - 2)
	- "depth" should be the largest d which can be expressed as mod = k * 2^d + 1
	- "primroot" should be a primitive root of mod

	Basic (modulo, depth, primroot) Combinations:
	- Case when digit_level = 32
		+ using poly1 = polynomial_mod<469762049, 26, 3>
		+ using poly2 = polynomial_mod<167772161, 25, 3>
		+ using poly3 = polynomial_mod<754974721, 24, 11>
		+ usint poly4 = polynomial_mod<998244353, 23, 3>
	- Case when digit_level = 64
		+ using ntt1 = polynomial_mod<180143985094819841, 55, 6>
	
	Constructors:
	- polynomial_mod() : Constructor of zero polynomial
	- polynomial_mod(std::size_t sz) : Constructor of zero polynomial with capacity sz
	- polynomial_mod(std::vector<modulo> v) : Constructor of polynomial P(x) = sum{v[i] * x^i}

	Operators:
	- bool operator==(const polynomial_mod &p) : Return true if equal, otherwise false
	- bool operator!=(const polynomial_mod &p) : Return true if different, otherwise true
	- modulo operator[](std::size_t idx) : Returns coefficient of x^idx
	- modulo& operator[](std::size_t idx) : Assignation of coefficient of x^idx
	- modulo& operator+=(const polynomial_mod &p) : Add p to the polynomial
	- modulo& operator-=(const polynomial_mod &p) : Subtract p to the polynomial
	- modulo& operator*=(const polynomial_mod &p) : Multiply p to the polynomial
	- modulo& operator/=(const polynomial_mod &p) : Divide p to the polynomial
	- modulo& operator%=(const polynomial_mod &p) : Changed to (the polynomial) mod p

	Verified:
	- AtCoder Typical Contest 001 Problem C: Fast Fourier Transform
		+ https://atcoder.jp/contests/atc001/submissions/5544644
*/