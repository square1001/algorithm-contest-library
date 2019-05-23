#ifndef CLASS_POLYNOMIAL_NTT
#define CLASS_POLYNOMIAL_NTT

#include "modint.h"
#include <vector>
#include <algorithm>

template<singlebit mod, singlebit depth, singlebit primroot>
class polynomial_ntt {
public:
	using modulo = fast_modint<mod>;
	static void fourier_transform(std::vector<modulo> &v, bool inverse) {
		std::size_t s = v.size();
		for (std::size_t i = 0, j = 1; j < s - 1; ++j) {
			for (std::size_t k = s >> 1; k > (i ^= k); k >>= 1);
			if (i < j) std::swap(v[i], v[j]);
		}
		std::size_t sc = 0, sz = 1;
		while (sz < s) sz *= 2, ++sc;
		modulo root = modulo(primroot).binpow((mod - 1) >> sc);
		std::vector<modulo> pw(s + 1); pw[0] = 1;
		for (std::size_t i = 1; i <= s; i++) pw[i] = pw[i - 1] * root;
		std::size_t qs = s;
		for (std::size_t b = 1; b < s; b <<= 1) {
			qs >>= 1;
			for (std::size_t i = 0; i < s; i += b * 2) {
				for (std::size_t j = i; j < i + b; ++j) {
					modulo delta = pw[(inverse ? b * 2 - j + i : j - i) * qs] * v[j + b];
					v[j + b] = v[j] - delta;
					v[j] += delta;
				}
			}
		}
		if (!inverse) return;
		modulo powinv = modulo((mod + 1) / 2).binpow(sc);
		for (std::size_t i = 0; i < s; ++i) {
			v[i] = v[i] * powinv;
		}
	}
	static std::vector<modulo> convolve(std::vector<modulo> v1, std::vector<modulo> v2) {
		std::size_t s1 = v1.size(), s2 = v2.size(), s = 1;
		while (s < s1 || s < s2) s *= 2;
		v1.resize(s * 2); fourier_transform(v1, false);
		v2.resize(s * 2); fourier_transform(v2, false);
		for (singlebit i = 0; i < s * 2; ++i) v1[i] *= v2[i];
		fourier_transform(v1, true);
		v1.resize(s1 + s2 - 1);
		return v1;
	}
};

#endif // CLASS_POLYNOMIAL_NTT

/*
	Assertion:
	- "mod" should be a prime number, less than 2^(digit_level - 2)
	- "depth" should be the largest d which can be expressed as mod = k * 2^d + 1
	- "primroot" should be a primitive root of mod

	Basic (modulo, depth, primroot) Combinations:
	- Case when digit_level = 32
		+ using ntt1 = polynomial_ntt<469762049, 26, 3>
		+ using ntt2 = polynomial_ntt<167772161, 25, 3>
		+ using ntt3 = polynomial_ntt<754974721, 24, 11>
		+ usint ntt4 = polynomial_ntt<998244353, 23, 3>
	- Case when digit_level = 64
		+ using ntt1 = polynomial_ntt<180143985094819841, 55, 6>
	
	Main Function:
	- std::vector<modulo> convolve(std::vector<modulo> v1, std::vector<modulo> v2)
		+ Returns convolution of v1 and v2
	
	Verified:
	- AtCoder Typical Contest 001 Problem C: Fast Fourier Transform
		+ https://atcoder.jp/contests/atc001/submissions/5544644
*/