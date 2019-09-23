#ifndef CLASS_ROLLING_HASH
#define CLASS_ROLLING_HASH

#include <string>
#include <cstddef>
#include <cstdint>

class rolling_hash {
public:
	static constexpr std::uint32_t level = 61; // <= 62
	static constexpr std::uint64_t mod = (1ull << level) - 1;
	static constexpr std::uint64_t base = 10000000000;
private:
	int sz;
	std::string str;
	std::uint64_t *hash;
	std::uint64_t *pw;
	std::uint64_t modmul(std::uint64_t va, std::uint64_t vb) const {
		std::uint64_t pa = va >> 31, qa = va & 2147483647;
		std::uint64_t pb = vb >> 31, qb = vb & 2147483647;
		std::uint64_t rem = qa * qb + (((pa * qb + pb * qa) & (mod >> 31)) << 31);
		std::uint64_t quo = ((pa * qb + pb * qa) >> (level - 31)) + ((pa * pb) << (62 - level)) + (rem >> level);
		return quo + (rem & mod) - (quo + (rem & mod) < mod ? 0 : mod);
	}
	std::uint32_t xorshift32(std::uint32_t x) {
		x ^= x << 13;
		x ^= x >> 17;
		x ^= x << 5;
		return x;
	}
public:
	rolling_hash() : sz(0), str(std::string()) {};
	rolling_hash(std::string str_) : sz(str_.length()), str(str_) {
		hash = new std::uint64_t[sz + 1]();
		pw = new std::uint64_t[sz + 1]();
		pw[0] = 1;
		for(std::size_t i = 0; i < sz; ++i) {
			hash[i + 1] = modmul(hash[i], base) + xorshift32(str[i] + 1);
			pw[i + 1] = modmul(pw[i], base);
			if(hash[i + 1] >= mod) hash[i + 1] -= mod;
		}
	}
	~rolling_hash() {
		delete[] hash;
		delete[] pw;
	}
	std::uint64_t gethash(std::size_t l, std::size_t r) const {
		std::uint64_t ans = mod + hash[r] - modmul(hash[l], pw[r - l]);
		return ans < mod ? ans : ans - mod;
	}
};

#endif // CLASS_ROLLING_HASH