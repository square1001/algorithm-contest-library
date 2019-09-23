#ifndef CLASS_MATRIX
#define CLASS_MATRIX

#include <cassert>
#include <cstddef>

template<class type>
class matrix {
private:
	std::size_t R, C;
	type *val;
public:
	matrix() : R(0), C(0) {};
	matrix(std::size_t R_, std::size_t C_) : R(R_), C(C_) { val = new type[R * C]; }
	matrix(std::size_t N_) : R(N_), C(N_) { val = new type[N_ * N_]; }
	matrix(const matrix& mat) { R = mat.R; C = mat.C; val = new type[R * C]; for(std::size_t i = 0; i < R * C; ++i) val[i] = mat.val[i]; }
	~matrix() { delete[] val; }
	matrix& operator=(const matrix& mat) { R = mat.R; C = mat.C; val = new type[R * C]; for(std::size_t i = 0; i < R * C; ++i) val[i] = mat.val[i]; return *this; }
	type& entry(std::size_t r, std::size_t c) { return val[r * C + c]; }
	type entry(std::size_t r, std::size_t c) const { return val[r * C + c]; }
	matrix operator==(const matrix& mat) const {
		if(R != mat.R || C != mat.C) return false;
		for(std::size_t i = 0; i < R * C; ++i) {
			if(val[i] != mat.val[i]) return false;
		}
		return true;
	}
	matrix operator!=(const matrix& mat) const {
		if(R != mat.R || C != mat.C) return true;
		for(std::size_t i = 0; i < R * C; ++i) {
			if(val[i] != mat.val[i]) return true;
		}
		return false;
	}
	matrix& operator+=(const matrix& mat) {
		assert(R == mat.R && C == mat.C);
		for(std::size_t i = 0; i < R * C; ++i) val[i] += mat.val[i];
		return *this;
	}
	matrix& operator-=(const matrix& mat) {
		assert(R == mat.R && C == mat.C);
		for(std::size_t i = 0; i < R * C; ++i) val[i] -= mat.val[i];
		return *this;
	}
	matrix& operator*=(const matrix& mat) {
		assert(C == mat.R);
		type *tmp = new type[R * mat.C];
		for(std::size_t i = 0; i < R; ++i) {
			for(std::size_t j = 0; j < C; ++j) {
				for(std::size_t k = 0; k < mat.C; ++k) {
					tmp[i * mat.C + k] += val[i * C + j] * mat.val[j * mat.C + k];
				}
			}
		}
		C = mat.C;
		for(std::size_t i = 0; i < R * C; ++i) val[i] = tmp[i];
		return *this;
	}
	matrix operator+(const matrix& mat) const { return matrix(*this) += mat; }
	matrix operator-(const matrix& mat) const { return matrix(*this) -= mat; }
	matrix operator*(const matrix& mat) const { return matrix(*this) *= mat; }
};

template<class type>
matrix<type> unit(int N) {
	matrix<type> ret(N);
	for (std::size_t i = 0; i < N; ++i) {
		ret.entry(i, i) = type(1);
	}
	return ret;
}

#endif // CLASS_MATRIX