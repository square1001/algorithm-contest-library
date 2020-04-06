#ifndef CLASS_MATRIX
#define CLASS_MATRIX

#include <vector>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <utility>

template<class type>
class matrix {
private:
	std::size_t R, C;
	std::vector<type> val;
public:
	matrix() : R(0), C(0), val(std::vector<type>()) {};
	matrix(std::size_t R_, std::size_t C_) : R(R_), C(C_), val(std::vector<type>(R* C)) {}
	matrix(std::size_t N_) : R(N_), C(N_), val(std::vector<type>(N_* N_)) {}
	type& entry(std::size_t r, std::size_t c) { return val[r * C + c]; }
	type entry(std::size_t r, std::size_t c) const { return val[r * C + c]; }
	static const matrix unit(std::size_t N) {
		matrix ret(N);
		for (std::size_t i = 0; i < N; ++i) {
			ret.entry(i, i) = type(1);
		}
		return ret;
	}
	matrix transpose() const {
		matrix ret(C, R);
		for (std::size_t i = 0; i < R; ++i) {
			for (std::size_t j = 0; j < C; ++j) {
				ret.val[j * R + i] = val[i * C + j];
			}
		}
		return ret;
	}
	bool operator==(const matrix& mat) const {
		if (R != mat.R || C != mat.C) return false;
		for (std::size_t i = 0; i < R * C; ++i) {
			if (val[i] != mat.val[i]) return false;
		}
		return true;
	}
	bool operator!=(const matrix& mat) const {
		if (R != mat.R || C != mat.C) return true;
		for (std::size_t i = 0; i < R * C; ++i) {
			if (val[i] != mat.val[i]) return true;
		}
		return false;
	}
	matrix& operator+=(const matrix& mat) {
		assert(R == mat.R && C == mat.C);
		for (std::size_t i = 0; i < R * C; ++i) val[i] += mat.val[i];
		return *this;
	}
	matrix& operator-=(const matrix& mat) {
		assert(R == mat.R && C == mat.C);
		for (std::size_t i = 0; i < R * C; ++i) val[i] -= mat.val[i];
		return *this;
	}
	matrix& operator*=(const matrix& mat) {
		assert(C == mat.R);
		matrix tmat = mat.transpose();
		std::vector<type> tmp(R * tmat.R);
		for (std::size_t i = 0; i < R; ++i) {
			for (std::size_t j = 0; j < tmat.R; ++j) {
				for (std::size_t k = 0; k < C; ++k) {
					tmp[i * tmat.R + j] += val[i * C + k] * tmat.val[j * tmat.C + k];
				}
			}
		}
		C = tmat.R;
		val = tmp;
		return *this;
	}
	matrix operator+=(const type& x) {
		for (std::size_t i = 0; i < R * C; ++i) val[i] += x;
		return *this;
	}
	matrix operator-=(const type& x) {
		for (std::size_t i = 0; i < R * C; ++i) val[i] -= x;
		return *this;
	}
	matrix operator*=(const type& x) {
		for (std::size_t i = 0; i < R * C; ++i) val[i] *= x;
		return *this;
	}
	matrix operator+(const matrix& mat) const { return matrix(*this) += mat; }
	matrix operator-(const matrix& mat) const { return matrix(*this) -= mat; }
	matrix operator*(const matrix& mat) const { return matrix(*this) *= mat; }
	matrix operator+(const type& x) const { return matrix(*this) += x; }
	matrix operator-(const type& x) const { return matrix(*this) -= x; }
	matrix operator*(const type& x) const { return matrix(*this) *= x; }
	matrix pow(std::uint64_t b) const {
		assert(R == C);
		matrix ans = unit(R), cur(*this);
		while (b != 0) {
			if (b & 1) ans *= cur;
			cur *= cur;
			b >>= 1;
		}
		return ans;
	}
	std::pair<matrix, matrix> gaussian_elimination(const matrix& mat) const {
		assert(R == mat.R);
		matrix lmat(*this), rmat(mat);
		std::size_t curpos = 0;
		for (std::size_t i = 0; i < R; ++i) {
			while (curpos < C) {
				std::size_t pos = std::size_t(-1);
				for (std::size_t j = i; j < R; ++j) {
					if (lmat.val[j * C + curpos] != type(0)) {
						pos = j;
						if (j != i) {
							for (std::size_t k = 0; k < C; ++k) std::swap(lmat.val[i * C + k], lmat.val[j * C + k]);
							for (std::size_t k = 0; k < rmat.C; ++k) std::swap(rmat.val[i * rmat.C + k], rmat.val[j * rmat.C + k]);
						}
						break;
					}
				}
				if (pos != std::size_t(-1)) {
					type mult = type(1) / lmat.val[i * C + curpos];
					for (std::size_t j = 0; j < C; ++j) lmat.val[i * C + j] *= mult;
					for (std::size_t j = 0; j < rmat.C; ++j) rmat.val[i * rmat.C + j] *= mult;
					lmat.val[i * C + curpos] = type(1);
					for (std::size_t j = 0; j < R; ++j) {
						if (j == i || lmat.val[j * C + curpos] == type(0)) continue;
						type submult = lmat.val[j * C + curpos];
						for (std::size_t k = 0; k < C; ++k) lmat.val[j * C + k] -= lmat.val[i * C + k] * submult;
						for (std::size_t k = 0; k < rmat.C; ++k) rmat.val[j * rmat.C + k] -= rmat.val[i * rmat.C + k] * submult;
						lmat.val[j * C + curpos] = type(0);
					}
					break;
				}
				++curpos;
			}
			if (curpos == C) break;
		}
		return std::make_pair(lmat, rmat);
	}
	matrix inverse() const {
		assert(R == C);
		std::pair<matrix, matrix> res = gaussian_elimination(unit(R));
		if (res.first != unit(R)) return matrix();
		return res.second;
	}
	type determinant() const {
		assert(R == C);
		matrix mat(*this);
		type ans = type(1);
		for(std::size_t i = 0; i < R; ++i) {
			std::size_t pos = std::size_t(-1);
			for(std::size_t j = i; j < R; ++j) {
				if(mat.val[j * C + i] != type(0)) {
					pos = j;
					break;
				}
			}
			if(pos == std::size_t(-1)) return type(0);
			if(pos != i) {
				ans *= type(-1);
				for(std::size_t j = i; j < C; ++j) {
					std::swap(mat.val[i * C + j], mat.val[pos * C + j]);
				}
			}
			ans *= mat.val[i * C + i];
			for(std::size_t j = i + 1; j < R; ++j) {
				type mul = mat.val[j * C + i] / mat.val[i * C + i];
				for(std::size_t k = i + 1; k < C; ++k) {
					mat.val[j * C + k] -= mat.val[i * C + k] * mul;
				}
			}
		}
		return ans;
	}
};

#endif // CLASS_MATRIX