#ifndef CLASS_DISJOINT_SET
#define CLASS_DISJOINT_SET

#include <vector>
#include <cstdint>
#include <cstring>
#include <algorithm>

class disjoint_set {
private:
	typedef std::int32_t value_type;
	std::vector<value_type> val;
public:
	explicit disjoint_set() : val() {};
	explicit disjoint_set(std::size_t n) : val(n, -1) {};
	std::size_t size() const { return val.size(); }
	std::size_t size(std::size_t elem) { return std::size_t(-val[root(elem)]); }
	std::size_t root(std::size_t elem) {
		// path halving
		while (val[elem] >= 0 && val[val[elem]] >= 0) {
			val[elem] = val[val[elem]];
			elem = val[elem];
		}
		return std::size_t(val[elem] >= 0 ? val[elem] : elem);
	}
	void link(std::size_t elemx, std::size_t elemy) {
		elemx = root(elemx);
		elemy = root(elemy);
		if (elemx == elemy) return;
		if (val[elemx] > val[elemy]) {
			std::swap(elemx, elemy);
		}
		val[elemx] += val[elemy];
		val[elemy] = elemx;
	}
	bool connected(std::size_t elemx, std::size_t elemy) {
		return root(elemx) == root(elemy);
	}
};

#endif // CLASS_DISJOINT_SET