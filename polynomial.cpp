#ifndef CLASS_POLYNOMIAL
#define CLASS_POLYNOMIAL

#include <vector>
#include <complex>
#include <cstdint>
#include <algorithm>
class polynomial {
private:
	using type = double;
	const type epsilon = 1.0e-9;
	std::size_t sz;
	std::vector<type> a;
	inline bool equivalent(type ra, type rb) const {
		return (epsilon <= ra - rb && ra - rb <= epsilon);
	}
	void discrete_fourier_transform(std::vector<std::complex<type> >& v, bool rev) {
		std::size_t n = v.size();
		const type pi = acos(type(-1));
		for (std::size_t i = 0, j = 1; j < n - 1; ++j) {
			for (std::size_t k = n >> 1; k >(i ^= k); k >>= 1);
			if (i > j) std::swap(v[i], v[j]);
		}
		for (std::size_t b = 1; b < n; b <<= 1) {
			std::complex<type> wr = std::polar(type(1), (rev ? type(-1) : type(1)) * pi / b);
			for (std::size_t i = 0; i < n; i += 2 * b) {
				std::complex<type> w = type(1);
				for (std::size_t j = 0; j < b; ++j) {
					std::complex<type> v0 = v[i + j];
					std::complex<type> v1 = w * v[i + j + b];
					v[i + j] = v0 + v1;
					v[i + j + b] = v0 - v1;
					w *= wr;
				}
			}
		}
		if (!rev) return;
		for (std::size_t i = 0; i < n; i++) v[i] /= n;
	}
public:
	explicit polynomial() : sz(1), a(std::vector<type>({ type() })) {};
	explicit polynomial(std::size_t sz_) : sz(sz_), a(std::vector<type>(sz_, type())) {};
	explicit polynomial(std::vector<type> a_) : sz(a_.size()), a(a_) {};
	polynomial& operator=(const polynomial& p) {
		sz = p.sz;
		a = p.a;
		return (*this);
	}
	std::size_t size() const { return sz; }
	std::size_t degree() const { return sz - 1; }
	type operator[](std::size_t idx) const {
		return a[idx];
	}
	type& operator[](std::size_t idx) {
		return a[idx];
	}
	bool operator==(const polynomial& p) const {
		for (std::size_t i = 0; i < sz && i < p.sz; ++i) {
			if (!equivalent(i < sz ? a[i] : type(0), i < p.sz ? p.a[i] : type(0))) {
				return false;
			}
		}
		return true;
	}
	bool operator!=(const polynomial& p) const {
		return !(operator==(p));
	}
	polynomial resize_transform(std::size_t d) const {
		// Resize polynomial to d: in other words, f(x) := f(x) mod x^d
		polynomial ans(*this);
		ans.sz = d;
		ans.a.resize(d, type(0));
		return ans;
	}
	polynomial star_transform() const {
		// f*(x) = x^degree * f(1/x)
		polynomial ans(*this);
		reverse(ans.a.begin(), ans.a.end());
		return ans;
	}
	polynomial inverse(std::size_t d) const {
		// Find g(x) where g(x) * f(x) = 1 (mod x^d)
		polynomial ans(std::vector<type>({ type(1) / a[0] }));
		while (ans.size() < d) {
			polynomial nxt;
			nxt = -ans * resize_transform(ans.size() * 2);
			nxt.a[0] += type(2);
			nxt *= ans;
			ans = nxt.resize_transform(ans.size() * 2);
		}
		ans = ans.resize_transform(d);
		return ans;
	}
	polynomial& operator+=(const polynomial& p) {
		sz = std::max(sz, p.sz);
		a.resize(sz);
		for (std::size_t i = 0; i < sz; ++i) a[i] += p.a[i];
		return (*this);
	}
	polynomial& operator-=(const polynomial& p) {
		sz = std::max(sz, p.sz);
		a.resize(sz);
		for (std::size_t i = 0; i < sz; ++i) a[i] -= p.a[i];
		return (*this);
	}
	polynomial& operator*=(const polynomial& p) {
		std::size_t n = 2;
		while (n < sz * 2 || n < p.sz * 2) n <<= 1;
		std::vector<std::complex<type> > v(n), pv(n);
		for (std::size_t i = 0; i < sz; ++i) v[i] = a[i];
		for (std::size_t i = 0; i < p.sz; ++i) pv[i] = p.a[i];
		discrete_fourier_transform(v, false);
		discrete_fourier_transform(pv, false);
		for (std::size_t i = 0; i < n; ++i) v[i] *= pv[i];
		discrete_fourier_transform(v, true);
		sz += p.sz - 1;
		a.resize(sz, type(0));
		for (std::size_t i = 0; i < sz; ++i) a[i] = v[i].real();
		return (*this);
	}
	polynomial& operator/=(const polynomial& p) {
		std::size_t dn = degree(), dm = p.degree();
		if (dn < dm) return polynomial();
		polynomial gstar = p.star_transform().inverse(dn - dm + 1);
		polynomial qstar = (gstar * (*this).star_transform()).resize_transform(dn - dm + 1);
		(*this) = qstar.star_transform();
		return (*this);
	}
	polynomial& operator%=(const polynomial& p) {
		(*this) -= (*this) / p * p;
		return (*this);
	}
	polynomial operator+() const {
		return polynomial(*this);
	}
	polynomial operator-() const {
		return polynomial() - polynomial(*this);
	}
	polynomial operator+(const polynomial& p) const {
		return polynomial(*this) += p;
	}
	polynomial operator-(const polynomial& p) const {
		return polynomial(*this) -= p;
	}
	polynomial operator*(const polynomial& p) const {
		return polynomial(*this) *= p;
	}
	polynomial operator/(const polynomial& p) const {
		return polynomial(*this) /= p;
	}
	polynomial operator%(const polynomial& p) const {
		return polynomial(*this) %= p;
	}
};

#endif