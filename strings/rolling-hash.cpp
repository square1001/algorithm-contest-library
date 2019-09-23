#include "rolling-hash.h"
#include <chrono>
#include <limits>
#include <iostream>
using namespace std;
unsigned x = 123456789;
unsigned xorshift32() {
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	return x;
}
void test(int n, int q) {
	string str;
	for(int i = 0; i < n; ++i) {
		str += xorshift32() % 26 + 'a';
	}
	chrono::system_clock::time_point start = chrono::system_clock::now();
	rolling_hash rh(str);
	chrono::system_clock::time_point mid = chrono::system_clock::now();
	std::uint64_t sum = 0;
	for(int i = 0; i < q; ++i) {
		int l = xorshift32() % n, r = xorshift32() % n;
		if(l > r) std::swap(l, r);
		sum += rh.gethash(l, r + 1);
	}
	chrono::system_clock::time_point finish = chrono::system_clock::now();
	cout.precision(12);
	std::chrono::duration<double> constructing_duration = mid - start;
	std::chrono::duration<double> queries_duration = finish - mid;
	cout << "---------- TEST RESUTLTS (Length = " << n << ", # of Queries = " << q << ") ----------" << endl;
	cout << "Answer: " << sum << endl;
	cout << fixed << n << " Length Construction: " << constructing_duration.count() << " seconds (" << constructing_duration.count() / q << " seconds per length)" << endl;
	cout << fixed << q << " Queries: " << queries_duration.count() << " seconds (" << queries_duration.count() / q << " seconds per query)" << endl;
}
int main() {
	test(1 << 18, 1 << 18);
	test(1 << 19, 1 << 19);
	test(1 << 20, 1 << 20);
	test(1 << 21, 1 << 21);
	test(1 << 22, 1 << 22);
	test(1 << 23, 1 << 23);
	test(1 << 24, 1 << 24);
	test(1 << 25, 1 << 25);
	test(1 << 26, 1 << 26);
	test(1 << 27, 1 << 27);
	return 0;
}