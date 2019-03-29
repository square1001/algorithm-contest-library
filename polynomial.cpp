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
			for (std::size_t k = n >> 1; k > (i ^= k); k >>= 1);
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
	explicit polynomial(int sz_) : sz(sz_), a(std::vector<type>(sz_, type())) {};
	explicit polynomial(std::vector<type> a_) : sz(a_.size()), a(a_) {};
	std::size_t size() { return sz; }
	std::size_t degree() { return sz - 1; }
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
		while (n < sz * 2 && n < p.sz * 2) n <<= 1;
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
};