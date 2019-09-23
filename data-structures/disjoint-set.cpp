#include <chrono>
#include <vector>
#include <iostream>
#include "disjoint-set.h"
using namespace std;
unsigned x = 123456789;
unsigned xorshift32() {
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	return x;
}
void test(int n, int q) {
	disjoint_set uf(n);
	chrono::system_clock::time_point start = chrono::system_clock::now();
	for(int i = 0; i < q; ++i) {
		int a = xorshift32() % n, b = xorshift32() % n;
		uf.link(a, b);
	}
	chrono::system_clock::time_point mid = chrono::system_clock::now();
	long long sum = 0;
	for(int i = 0; i < n; ++i) {
		sum += uf.root(i);
	}
	chrono::system_clock::time_point finish = chrono::system_clock::now();
	cout.precision(12);
	std::chrono::duration<double> linking_duration = mid - start;
	std::chrono::duration<double> rootfind_duration = finish - mid;
	cout << "---------- TEST RESUTLTS (# of Elements = " << n << ", # of Queries = " << q << ") ----------" << endl;
	cout << "Answer: " << sum << endl;
	cout << fixed << q << " Linkings: " << linking_duration.count() << " seconds (" << linking_duration.count() / q << " seconds per query)" << endl;
	cout << fixed << n << " Root Findings: " << rootfind_duration.count() << " seconds (" << rootfind_duration.count() / n << " seconds per element)" << endl;
}
int main() {
	test(1 << 18, 1 << 17);
	test(1 << 19, 1 << 18);
	test(1 << 20, 1 << 19);
	test(1 << 21, 1 << 20);
	test(1 << 22, 1 << 21);
	test(1 << 23, 1 << 22);
	test(1 << 24, 1 << 23);
	test(1 << 25, 1 << 24);
	test(1 << 26, 1 << 25);
	test(1 << 27, 1 << 26);
	return 0;
}