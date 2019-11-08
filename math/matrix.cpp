#include "matrix.h"
#include <iostream>
using namespace std;
int main() {
	matrix<double> d(2), I = matrix<double>::unit(2);
	d.entry(0, 0) = 2.0; d.entry(0, 1) = -1.0;
	d.entry(1, 0) = -1.0; d.entry(1, 1) = 2.0;
	for(int i = -10; i <= 10; ++i) {
		cout << i << ' ' << (d - I * 4 - I * i).determinant() << endl;
	}
	return 0;
}