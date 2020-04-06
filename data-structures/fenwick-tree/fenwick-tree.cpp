#include <chrono>
#include <vector>
#include <iostream>
#include "fenwick-tree.h"
using namespace std;
unsigned x = 123456789;
unsigned xorshift32() {
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	return x;
}
void test(int n, int q) {
	fenwick_tree<long long> fen(n);
	chrono::system_clock::time_point start = chrono::system_clock::now();
	for(int i = 0; i < q; ++i) {
		int pos = xorshift32() % n, val = (long long)(xorshift32()) % 33 - 16;
		fen.add(pos, val);
	}
	chrono::system_clock::time_point mid = chrono::system_clock::now();
	long long sum = 0;
	for(int i = 0; i < q; ++i) {
		int l = xorshift32() % n, r = xorshift32() % n;
		if(l > r) std::swap(l, r);
		sum += fen.getsum(l, r);
	}
	chrono::system_clock::time_point finish = chrono::system_clock::now();
	cout.precision(12);
	std::chrono::duration<double> adding_duration = mid - start;
	std::chrono::duration<double> getsum_duration = finish - mid;
	cout << "---------- TEST RESUTLTS (# of Elements = " << n << ", # of Queries = " << q << ") ----------" << endl;
	cout << "Answer: " << sum << endl;
	cout << fixed << q << " Addings: " << adding_duration.count() << " seconds (" << adding_duration.count() / q << " seconds per query)" << endl;
	cout << fixed << q << " Sum Getting: " << getsum_duration.count() << " seconds (" << getsum_duration.count() / q << " seconds per element)" << endl;
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