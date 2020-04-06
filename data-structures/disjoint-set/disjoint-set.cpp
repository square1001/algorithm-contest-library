#include <cmath>
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
	const int samples = 50;
	double tsum1 = 0.0, tsum2 = 0.0;
	long long gsum = 0;
	for(int t = 0; t < samples; ++t) {
		chrono::system_clock::time_point start = chrono::system_clock::now();
		disjoint_set uf(n);
		chrono::system_clock::time_point mid1 = chrono::system_clock::now();
		for(int i = 0; i < q; ++i) {
			int a = xorshift32() % n, b = xorshift32() % n;
			uf.link(a, b);
		}
		chrono::system_clock::time_point mid2 = chrono::system_clock::now();
		long long sum = 0;
		for(int i = 0; i < n; ++i) {
			sum += uf.root(i);
		}
		chrono::system_clock::time_point finish = chrono::system_clock::now();
		cout.precision(12);
		std::chrono::duration<double> init_duration = mid1 - start;
		std::chrono::duration<double> linking_duration = mid2 - mid1;
		double tinit = init_duration.count();
		double tlink = linking_duration.count();
		tsum1 += tinit;
		tsum2 += tlink;
		gsum += sum;
	}
	cout << "---------- TEST RESUTLTS (# of Elements = " << n << ", # of Queries = " << q << ") ----------" << endl;
	cout << "Debug Answer: " << gsum << endl;
	cout << fixed << "Initialization: " << (tsum1 / samples) / n << " seconds per element" << endl;
	cout << fixed << "Linkings: " << (tsum2 / samples) / q << " seconds per query" << endl;
}
int main() {
	for(int i = 12 * 3; i <= 26 * 3; ++i) {
		double t = std::pow(2.0, i / 3.0);
		int n = int(t);
		test(n, n);
	}
	return 0;
}