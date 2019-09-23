#ifndef CLASS_MODINT
#define CLASS_MODINT

#include <cstdint>

template <std::uint32_t mod>
class modint {
private:
	std::uint32_t n;
public:
	modint() : n(0) {};
	modint(std::int64_t n_) : n((n_ >= 0 ? n_ : mod - (-n_) % mod) % mod) {};
	static constexpr std::uint32_t get_mod() { return mod; }
	std::uint32_t get() const { return n; }
	bool operator==(const modint& m) const { return n == m.n; }
	bool operator!=(const modint& m) const { return n != m.n; }
	modint& operator+=(const modint& m) { n += m.n; n = (n < mod ? n : n - mod); return *this; }
	modint& operator-=(const modint& m) { n += mod - m.n; n = (n < mod ? n : n - mod); return *this; }
	modint& operator*=(const modint& m) { n = std::uint64_t(n) * m.n % mod; return *this; }
	modint operator+(const modint& m) const { return modint(*this) += m; }
	modint operator-(const modint& m) const { return modint(*this) -= m; }
	modint operator*(const modint& m) const { return modint(*this) *= m; }
	modint inv() const { return (*this).pow(mod - 2); }
	modint pow(std::uint64_t b) const {
		modint ans = 1, m = modint(*this);
		while (b) {
			if (b & 1) ans *= m;
			m *= m;
			b >>= 1;
		}
		return ans;
	}
};

#endif // CLASS_MODINT

#ifndef CLASS_FAST_MODINT
#define CLASS_FAST_MODINT

#include <cstdint>

static constexpr std::size_t digit_level = 8 * sizeof(std::uint32_t);
static constexpr std::uint32_t find_inv(std::uint32_t n, int d = 6, std::uint32_t x = 1) {
	return d == 0 ? x : find_inv(n, d - 1, x * (2 - x * n));
}
template <std::uint32_t mod> class fast_modint {
	// Fast Modulo Integer, Assertion: mod < 2^(bits of singlebit - 1) and mod is prime
private:
	std::uint32_t n;
	static constexpr std::uint32_t r2 = (((std::uint64_t(1) << digit_level) % mod) << digit_level) % mod;
	static constexpr std::uint32_t ninv = std::uint32_t(-1) * find_inv(mod);
	std::uint32_t reduce(std::uint64_t x) const {
		std::uint32_t res = (x + std::uint64_t(std::uint32_t(x) * ninv) * mod) >> digit_level;
		return res < mod ? res : res - mod;
	}
public:
	fast_modint() : n(0) {};
	fast_modint(std::uint32_t n_) { n = reduce(std::uint64_t(n_) * r2); };
	static constexpr std::uint32_t get_mod() { return mod; }
	std::uint32_t get() const { return reduce(n); }
	bool operator==(const fast_modint& x) const { return n == x.n; }
	bool operator!=(const fast_modint& x) const { return n != x.n; }
	fast_modint& operator+=(const fast_modint& x) { n += x.n; n -= (n < mod ? 0 : mod); return *this; }
	fast_modint& operator-=(const fast_modint& x) { n += mod - x.n; n -= (n < mod ? 0 : mod); return *this; }
	fast_modint& operator*=(const fast_modint& x) { n = reduce(std::uint64_t(n) * x.n); return *this; }
	fast_modint operator+(const fast_modint& x) const { return fast_modint(*this) += x; }
	fast_modint operator-(const fast_modint& x) const { return fast_modint(*this) -= x; }
	fast_modint operator*(const fast_modint& x) const { return fast_modint(*this) *= x; }
	fast_modint inv() const { return pow(mod - 2); }
	fast_modint pow(std::uint64_t b) const {
		fast_modint ans(1), cur(*this);
		while (b > 0) {
			if (b & 1) ans *= cur;
			cur *= cur;
			b >>= 1;
		}
		return ans;
	}
};

#endif // CLASS_FAST_MODINT

/*
	Assertion:
	- Basically the modulo value should be a prime number
		- Should be assured when using inv()

	Defining modulo integer modulo 1,000,000,007:
	- modint<100000007> m;
	- fast_modint<1000000007> m;

	Tips to use fixed modulo:
	- using modulo = modint<1000000007>;

	Constructors:
	- modint() : Constructor of modint set to zero
	- modint(int64_t n) : Constructor of modint set to (n % mod)

	Very Basic Function:
	- uint32_t get_mod() : Returns the modulo value
	- uint32_t get() : Returns the value converted to integer type

	Operators:
	- bool operator==(const modint& m) : Returns true if equal, otherwise false
	- bool operator!=(const modint& m) : Returns true if different, otherwise true
	- modint& operator+=(const modint& m) : Add m to the modint value
	- modint& operator-=(const modint& m) : Subtract m to the modint value
	- modint& operator*=(const modint& m) : Multiply m to the modint value
	- modint operator+(const modint& m) : Returns ((the modint value) + m)
	- modint operator-(const modint& m) : Returns ((the modint value) - m)
	- modint operator*(const modint& m) : Returns ((the modint value) * m)

	Advanced Functions:
	- modint pow(std::uint64_t b) : Returns b-th power of the modint value
	- modint inv() : Returns modular inverse of the modint value

	Verified:
	- CPSCO 2019 Session 3 Problem F: Flexible Permutations
		- https://atcoder.jp/contests/cpsco2019-s3/submissions/5276295
	- Educational DP Contest Problem H: Grid 1
		- https://atcoder.jp/contests/dp/submissions/3939858
*/