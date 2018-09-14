#include <vector>
#include <iostream>
using namespace std;
template<int mod, int depth, int primroot>
class ntt {
	// Number Theoretic Transform
private:
	std::vector<int> roots;
	std::vector<int> powinv;
public:
	ntt() {
		roots = std::vector<int>(depth + 1, 0);
		powinv = std::vector<int>(depth + 1, 0);
		powinv[1] = (mod + 1) / 2;
		for (int i = 2; i <= depth; ++i) {
			powinv[i] = 1LL * powinv[i - 1] * powinv[1] % mod;
		}
		roots[depth] = 1;
		for (int i = 0; i < mod - 1; i += 1 << depth) {
			roots[depth] = 1LL * roots[depth] * primroot % mod;
		}
		for (int i = depth - 1; i >= 1; --i) {
			roots[i] = 1LL * roots[i + 1] * roots[i + 1] % mod;
		}
	}
	void fourier_transform(std::vector<int> &v, bool inverse) {
		int s = v.size();
		for (int i = 0, j = 1; j < s - 1; ++j) {
			for (int k = s >> 1; k >(i ^= k); k >>= 1);
			if (i < j) std::swap(v[i], v[j]);
		}
		int sc = 0, sz = 1;
		while (sz < s) sz *= 2, ++sc;
		std::vector<int> pw(s + 1); pw[0] = 1;
		for (int i = 1; i <= s; i++) pw[i] = 1LL * pw[i - 1] * roots[sc] % mod;
		int qs = s;
		for (int b = 1; b < s; b <<= 1) {
			qs >>= 1;
			for (int i = 0; i < s; i += b * 2) {
				for (int j = i; j < i + b; ++j) {
					int f0 = v[j], f1 = 1LL * pw[(inverse ? b * 2 - j + i : j - i) * qs] * v[j + b] % mod;
					v[j] = (f0 + f1 < mod ? f0 + f1 : f0 + f1 - mod);
					v[j + b] = (f0 - f1 >= 0 ? f0 - f1 : f0 - f1 + mod);
				}
			}
		}
		if (inverse) {
			for (int i = 0; i < s; ++i) v[i] = 1LL * v[i] * powinv[sc] % mod;
		}
	}
	std::vector<int> convolve(std::vector<int> v1, std::vector<int> v2) {
		int s1 = v1.size(), s2 = v2.size(), s = 1;
		while (s < s1 && s < s2) s *= 2;
		v1.resize(s * 2); fourier_transform(v1, false);
		v2.resize(s * 2); fourier_transform(v2, false);
		for (int i = 0; i < s * 2; ++i) v1[i] = 1LL * v1[i] * v2[i] % mod;
		fourier_transform(v1, true);
		v1.resize(s1 + s2 - 1);
		return v1;
	}
};
// Examples
ntt<469762049, 26, 3> ntt1;
ntt<167772161, 25, 3> ntt2;
ntt<998244353, 23, 3> ntt3;