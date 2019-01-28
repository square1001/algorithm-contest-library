#ifndef ___CLASS_MODINT_ANYMOD
#define ___CLASS_MODINT_ANYMOD

#include <cstdint>

class modint {
private:
	std::uint32_t n;
	static std::uint32_t mod;
public:
	modint() : n(0) {};
	modint(std::uint64_t n_) : n(n_ % mod) {};
	static void set_mod(std::uint64_t mod_) { mod = mod_; }
	bool operator==(const modint& m) const { return n == m.n; }
	bool operator!=(const modint& m) const { return n != m.n; }
	std::uint32_t get() const { return n; }
	modint& operator+=(const modint& m) { n += m.n; n = (n < mod ? n : n - mod); return *this; }
	modint& operator-=(const modint& m) { n += mod - m.n; n = (n < mod ? n : n - mod); return *this; }
	modint& operator*=(const modint& m) { n = std::uint64_t(n) * m.n % mod; return *this; }
	modint operator+(const modint& m) const { return modint(*this) += m; }
	modint operator-(const modint& m) const { return modint(*this) -= m; }
	modint operator*(const modint& m) const { return modint(*this) *= m; }
	modint binpow(std::uint64_t b) const {
		modint ans = 1, m = modint(*this);
		while (b) {
			if (b & 1) ans *= m;
			m *= m;
			b >>= 1;
		}
		return ans;
	}
	modint inv() { return (*this).binpow(mod - 2); }
};

std::uint32_t modint::mod = 0;

#endif // ___CLASS_MODINT_ANYMOD