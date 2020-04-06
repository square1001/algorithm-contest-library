#ifndef CLASS_FENWICKTREE
#define CLASS_FENWICKTREE

#include <vector>
#include <cassert>
#include <cstddef>

template <class type>
class fenwick_tree {
private:
	std::size_t n, sz;
	std::vector<type> val;
public:
	fenwick_tree() : n(0), sz(0) {};
	fenwick_tree(std::size_t n_) : n(n_) {
		sz = 1; while (sz < n) sz *= 2;
		val = std::vector<type>(sz + 1);
	}
	template <class InputIterator>
	fenwick_tree(InputIterator first, InputIterator last) : n(last - first) {
		sz = 1; while (sz < n) sz *= 2;
		val = std::vector<type>(sz + 1);
		std::size_t cur = 0;
		for (InputIterator it = first; it != last; ++it) val[++cur] += *it;
		for (std::size_t i = 1; i < sz; ++i) val[i + (i & ~(i - 1))] += val[i];
	}
	void add(std::size_t pos, type delta) {
		for (std::size_t i = pos + 1; i <= sz; i += i & ~(i - 1)) {
			val[i] += delta;
		}
	}
	type getsum(std::size_t r) const {
		assert(0 <= r && r <= n);
		type ans = 0;
		for (std::size_t i = r; i >= 1; i -= i & ~(i - 1)) {
			ans += val[i];
		}
		return ans;
	}
	type getsum(std::size_t l, std::size_t r) const {
		assert(0 <= l && l <= r && r <= n);
		return getsum(r) - getsum(l);
	}
	std::size_t binary_search(type threshold) const {
		std::size_t ans = 0;
		for(std::size_t i = (sz >> 1); i >= 1; i >>= 1) {
			if(threshold >= val[ans + i]) {
				threshold -= val[ans + i];
				ans += i;
			}
		}
		return ans;
	}
};

#endif // CLASS_FENWICKTREE