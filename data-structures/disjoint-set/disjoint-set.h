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

/*
	What is Disjoint Set?
	- The disjoint set of N elements is the set of sets which appears 0, 1, 2, ..., N - 1 as elements exactly once.
	- First, it is initialized for { 0 }, { 1 }, { 2 }, ..., { N - 1 }, which means all N elements are in different set.
	- We can process queries of "merging two sets" in time complexity O(log* N).
	- We can process queries of "deterimining if given two elements are in the same set" in time complexity O(log* N).
	- Here, O(log* N) is the iterated logarithm of N.

	Author: square1001

	Last Update: January 6th, 2020

	Constructors:
	- disjoint_set() : Initialize to the empty disjoint set
	- disjoint_set(N) : Initialize to the non-linked disjoint set with "N" elements

	Basic Functions:
	- std::size_t size() : Get the size of disjoint set itself

	Functions:
	- std::size_t size(elem) : Get the size of set with "elem"
	- std::size_t link(elemx, elemy) : Merge set with "elemx" and set with "elemy"
	- std::size_t connected(elemx, elemy) : Check if two elements "elemx" and "elemy" are in the same set

	Verified:
	- PAKEN Programming Camp 2019 Day 3 "PAKEN CUP 2019" Problem E : "Big Christmas Presents"
		- https://atcoder.jp/contests/pakencamp-2019-day3/submissions/9336106
		- Verified for "size(elem)" and "link(elem)"
		- Verified in January 6th, 2020
	- AOJ DSL 1-A : Disjoint Set: Union Find Tree
		- https://onlinejudge.u-aizu.ac.jp/recent_judges/DSL_1_A/judge/4093054/square1001/C++11
		- Verified for "link(elemx, elemy)" and "connected(elemx, elemy)"
		- Verified in January 6th, 2020
*/