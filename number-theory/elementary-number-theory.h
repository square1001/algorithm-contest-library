#ifndef ELEMENTARY_NUMBER_THEORY_LIBRARY
#define ELEMENTARY_NUMBER_THEORY_LIBRARY

#include <map>
#include <vector>
#include <cstdint>

std::uint64_t gcd(std::uint64_t x, std::uint64_t y) {
	if(y == 0) return x;
	return gcd(y, x % y);
}

std::uint64_t lcm(std::uint64_t x, std::uint64_t y) {
	return x / gcd(x, y) * y;
}

std::map<std::uint64_t, std::uint32_t> prime_factorize(std::uint64_t n) {
	std::map<std::uint64_t, std::uint32_t> res;
	for(std::uint32_t i = 2; std::uint64_t(i) * i <= n; ++i) {
		while(n % i == 0) {
			++res[i];
			n /= i;
		}
	}
	if(n > 1) ++res[n];
	return res;
}

std::vector<std::uint64_t> get_divisors(std::uint64_t n) {
	std::map<std::uint64_t, std::uint32_t> pf = prime_factorize(n);
	std::uint64_t elems = 1;
	for(std::pair<std::uint64_t, std::uint32_t> i : pf) {
		elems *= i.second + 1;
	}
	std::vector<std::uint64_t> res(elems);
	res[0] = 1;
	elems = 1;
	for(std::pair<std::uint64_t, std::uint32_t> i : pf) {
		for(std::uint32_t j = 1; j <= i.second; ++j) {
			for(std::size_t k = j * elems; k < (j + 1) * elems; ++k) {
				res[k] = res[k - elems] * i.first;
			}
		}
		elems *= i.second + 1;
	}
	return res;
}

std::uint64_t totient(std::uint64_t n) {
	std::map<std::uint64_t, std::uint32_t> pf = prime_factorize(n);
	std::uint64_t res = n;
	for(std::pair<std::uint64_t, std::uint32_t> i : pf) {
		res = res / i.first * (i.first - 1);
	}
	return res;
}

#endif // ELEMENTARY_NUMBER_THEORY_LIBRARY