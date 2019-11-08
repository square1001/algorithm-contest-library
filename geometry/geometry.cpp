#include "geometry.h"
#include <iostream>
#include <algorithm>
using namespace std;
int main() {
	cin.tie(0);
	ios_base::sync_with_stdio(false);
	int N;
	cin >> N;
	vector<point2d<double> > p(N);
	for (int i = 0; i < N; ++i) {
		cin >> p[i].x >> p[i].y;
	}
	vector<int> ans = convex_hull(p);
	cout << ans.size() << endl;
	for (int i = 0; i < ans.size(); ++i) {
		cout << p[ans[i]].x << ' ' << p[ans[i]].y << "\n";
	}
	return 0;
}