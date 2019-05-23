#ifndef CLASS_FAST_MODINT
#define CLASS_FAST_MODINT

#include <cstdint>

using singlebit = uint32_t;
using doublebit = uint64_t;
static constexpr int digit_level = 32;

static constexpr singlebit find_inv(singlebit n, int d = 6, singlebit x = 1) {
	return d == 0 ? x : find_inv(n, d - 1, x * (2 - x * n));
}
template <singlebit mod> class fast_modint {
	// Fast Modulo Integer, Assertion: mod < 2^(bits of singlebit - 1) and mod is prime
private:
	singlebit n;
	static constexpr singlebit r2 = (((doublebit(1) << digit_level) % mod) << digit_level) % mod;
	static constexpr singlebit ninv = singlebit(-1) * find_inv(mod);
	singlebit reduce(doublebit x) const {
		singlebit res = (x + doublebit(singlebit(x) * ninv) * mod) >> digit_level;
		return res < mod ? res : res - mod;
	}
public:
	fast_modint() : n(0) {};
	fast_modint(singlebit n_) { n = reduce(doublebit(n_) * r2); };
	static constexpr singlebit get_mod() { return mod; }
	singlebit get() const { return reduce(n); }
	bool operator==(const fast_modint& x) const { return n == x.n; }
	bool operator!=(const fast_modint& x) const { return n != x.n; }
	fast_modint& operator+=(const fast_modint& x) { n += x.n; n -= (n < mod ? 0 : mod); return *this; }
	fast_modint& operator-=(const fast_modint& x) { n += mod - x.n; n -= (n < mod ? 0 : mod); return *this; }
	fast_modint& operator*=(const fast_modint& x) { n = reduce(doublebit(n) * x.n); return *this; }
	fast_modint operator+(const fast_modint& x) const { return fast_modint(*this) += x; }
	fast_modint operator-(const fast_modint& x) const { return fast_modint(*this) -= x; }
	fast_modint operator*(const fast_modint& x) const { return fast_modint(*this) *= x; }
	fast_modint inv() const { return binpow(mod - 2); }
	fast_modint binpow(singlebit b) const {
		fast_modint ans(1), cur(*this);
		while (b > 0) {
			if (b & 1) ans *= cur;
			cur *= cur;
			b >>= 1;
		}
		return ans;
	}
};

template<typename modulo>
std::vector<modulo> get_modvector(std::vector<int> v) {
	std::vector<modulo> ans(v.size());
	for (int i = 0; i < v.size(); ++i) {
		ans[i] = modulo(v[i]);
	}
	return ans;
}

#endif // CLASS_FAST_MODINT

/*
	Assertion:
	- "mod" should be a prime number, less than 2^(digit_level - 1)
	- If you do not use "inv", "mod" only have to be an odd number 

	Defining modulo integer modulo 1,000,000,007:
	- fast_modint<100000007> m;

	Tips to use fixed modulo:
	- using modulo = fast_modint<1000000007>;

	Constructors:
	- fast_modint() : Constructor of modint set to zero
	- fast_modint(singlebit n) : Constructor of modint set to n
		+ Assertion: 0 <= n < mod

	Very Basic Function:
	- singlebit get() : Returns the value converted to integer type

	Operators:
	- bool operator==(const modint& m) : Returns true if equal, otherwise false
	- bool operator!=(const modint& m) : Returns true if different, otherwise true
	- fast_modint& operator+=(const modint& m) : Add m to the modint value
	- fast_modint& operator-=(const modint& m) : Subtract m to the modint value
	- fast_modint& operator*=(const modint& m) : Multiply m to the modint value
	- fast_modint operator+(const modint& m) : Returns ((the modint value) + m)
	- fast_modint operator-(const modint& m) : Returns ((the modint value) - m)
	- fast_modint operator*(const modint& m) : Returns ((the modint value) * m)

	Advanced Functions:
	- fast_modint pow(std::uint64_t b) : Returns b-th power of the modint value
	- fast_modint inv() : Returns modular inverse of the modint value

	Verified:
	- AtCoder Typical Contest 001 Problem C: Fast Fourier Transform
		+ https://atcoder.jp/contests/atc001/submissions/5544644
*/