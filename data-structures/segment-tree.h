#ifndef CLASS_SEGMENT_TREE
#define CLASS_SEGMENT_TREE

#include <limits>
#include <algorithm>
#include <functional>

template<class type>
class monoid {
private:
	std::function<type(type, type)> func_;
	type identity_;
public:
	explicit monoid() : func_(std::function<type(type, type)>()), identity_(0) {};
	explicit monoid(std::function<type(type, type)> func__, type identity__) : func_(func__), identity_(identity__) {};
	type identity() const { return identity_; }
	type operator()(type input1, type input2) const { return func_(input1, input2); }
};

template <class type>
class segment_tree {
private:
	std::size_t n, sz;
	type *val;
	monoid<type> M;
public:
	explicit segment_tree() : M(monoid<type>()), n(0), sz(0) {};
	explicit segment_tree(monoid<type> M_, std::size_t n_) : M(M_), n(n_) {
		sz = 1; while(sz < n) sz *= 2;
		val = new type[2 * sz];
		std::fill(val, val + 2 * sz, M.identity());
	}
	template <class InputIterator>
	explicit segment_tree(monoid<type> M_, InputIterator first, InputIterator last) : M(M_), n(last - first) {
		sz = 1; while(sz < n) sz *= 2;
		val = new type[2 * sz];
		std::fill(val, val + 2 * sz, M.identity());
		std::size_t cur = sz;
		for (InputIterator it = first; it != last; ++it) val[cur++] = *it;
		for (std::size_t i = sz - 1; i >= 1; --i) val[i] = M(val[2 * i], val[2 * i + 1]);
	}
	~segment_tree() { delete[] val; }
	void update(std::size_t pos, type nxtval) {
		pos += sz;
		val[pos] = nxtval;
		while(pos > 1) {
			pos >>= 1;
			val[pos] = M(val[pos * 2], val[pos * 2 + 1]);
		}
	}
	type range_query(type l, type r) const {
		type ansl = M.identity(), ansr = M.identity();
		l += sz; r += sz;
		while(l != r) {
			if(l & 1) ansl = M(ansl, val[l]), ++l;
			if(r & 1) --r, ansr = M(val[r], ansr);
			l >>= 1; r >>= 1;
		}
		return M(ansl, ansr);
	}
};

template <class type> const monoid<type> monoid_min([&](type input1, type input2) { return std::min(input1, input2); }, std::numeric_limits<type>::max());
template <class type> const monoid<type> monoid_max([&](type input1, type input2) { return std::max(input1, input2); }, std::numeric_limits<type>::min());
template <class type> const monoid<type> monoid_sum([&](type input1, type input2) { return input1 + input2; }, type(0));

#endif // CLASS_SEGMENT_TREE

/*
	<<< Monoid >>>
	Assertion
	- Needs to be a "monoid"
		- Associativity func(func(a, b), c) = func(a, func(b, c)) must hold
		- For identity element I, func(I, a) = I = func(a, I) must hold

	Constructor
	- monoid<type>(func: (type, type) -> type, type identity)

	Getting Identity
	- type identity() : const

	Getting Function Return Value
	- type operator()(type input1, type input2) : const
		- We can use like M(a, b) for monoid M
	
	
	<<< Segment Tree >>>
	Constructors
	- segment_tree() : Define an empty segment tree
	- segment_tree()
*/