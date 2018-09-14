#include <cmath>
#include <vector>
#include <algorithm>
class point2d {
public:
	double x, y;
	point2d() : x(0), y(0) {};
	point2d(double x_, double y_) : x(x_), y(y_) {};
	bool operator==(const point2d& p) { return x == p.x && y == p.y; }
	bool operator!=(const point2d& p) { return x != p.x || y != p.y; }
	point2d& operator+=(const point2d& p) { x += p.x; y += p.y; return *this; }
	point2d& operator-=(const point2d& p) { x -= p.x; y -= p.y; return *this; }
	point2d& operator*=(const double v) { x *= v; y *= v; return *this; }
	point2d operator+(const point2d& p) const { return point2d(*this) += p; }
	point2d operator-(const point2d& p) const { return point2d(*this) -= p; }
	point2d operator*(const double v) const { return point2d(*this) *= v; }
	double norm() { return x * x + y * y; }
	double abs() { return std::sqrt(x * x + y * y); }
	double dot(const point2d& p) { return x * p.x + y * p.y; }
	double cross(const point2d& p) { return x * p.y - y * p.x; }
	int ccw(const point2d& p1, const point2d& p2) {
		// RETURNS: P0 -> P1 -> P2 IS COLLINEAR --> ((P0, P1, P2) IS CENTER --> (2, -2, 0)), COUNTER-CLOCKWISE --> 1, CLOCKWISE --> -1
		point2d pr1 = p1 - point2d(*this);
		point2d pr2 = p2 - point2d(*this);
		if (pr1.cross(pr2) != 0) return pr1.cross(pr2) > 0 ? 1 : -1;
		if (pr1.dot(pr2) < 0) return 2;
		return pr1.norm() < pr2.norm() ? -2 : 0;
	}
};
class segment2d {
public:
	point2d p1, p2;
	segment2d() : p1(point2d()), p2(point2d()) {};
	segment2d(const point2d& p1_, const point2d& p2_) : p1(p1_), p2(p2_) {};
	double norm() { return (p1 - p2).norm(); }
	double abs() { return (p1 - p2).abs(); }
};
bool intersect(segment2d& s1, segment2d& s2) {
	return s1.p1.ccw(s1.p2, s2.p1) * s1.p1.ccw(s1.p2, s2.p2) <= 0 && s2.p1.ccw(s2.p2, s1.p1) * s2.p1.ccw(s2.p2, s1.p2) <= 0;
}
point2d intersect_point(segment2d &s1, segment2d &s2) {
	point2d base = s2.p2 - s2.p1;
	double d1 = std::abs(base.cross(s1.p1 - s2.p1));
	double d2 = std::abs(base.cross(s1.p2 - s2.p1));
	return s1.p1 + (s1.p2 - s1.p1) * (d1 / (d1 + d2));
}
double distance(point2d& p, segment2d& s) {
	double d1 = (p - s.p1).norm();
	double d2 = (p - s.p2).norm();
	double d0 = s.norm();
	if (d0 + d1 < d2 || d0 + d2 < d1) return std::sqrt(std::min(d1, d2));
	return std::abs((p - s.p1).cross(s.p2 - s.p1)) / std::sqrt(d0);
}
double distance(segment2d& s1, segment2d& s2) {
	if (intersect(s1, s2)) return 0.0;
	return min({ distance(s1.p1, s2), distance(s1.p2, s2), distance(s2.p1, s1), distance(s2.p2, s1) });
}
double area(vector<point2d> v) {
	double sum = 0.0;
	for (int i = 0; i < v.size(); ++i) {
		sum += v[i].cross(v[(i + 1) % v.size()]);
	}
	return std::abs(sum) * 0.5;
}
int contain(vector<point2d> v, point2d p) {
	// (IN POLYGON, ON THE EDGE, OUT OF POLYGON) = (1, 0, -1)
	int ans = -1;
	for (int i = 0; i < v.size(); ++i) {
		point2d p1 = v[i] - p;
		point2d p2 = v[(i + 1) % v.size()] - p;
		if (p1.x > p2.x) swap(p1, p2);
		if (p1.x <= 0 && 0 < p2.x && p1.cross(p2) < 0) ans *= -1;
		if (p1.cross(p2) == 0 && p1.dot(p2) <= 0) return 0;
	}
	return ans;
}
std::vector<int> convex_hull(std::vector<point2d> v) {
	int N = v.size();
	std::vector<int> perm(N);
	for (int i = 0; i < N; ++i) perm[i] = i;
	std::sort(perm.begin(), perm.end(), [&](int i, int j) { return v[i].y != v[j].y ? v[i].y < v[j].y : v[i].x < v[j].x; });
	std::vector<int> st; st.push_back(perm[0]);
	for (int i = 1; i < 2 * N - 1; ++i) {
		int pos = perm[i < N ? i : 2 * N - i - 2];
		while (st.size() >= 2) {
			point2d v1 = v[pos] - v[st.back()];
			point2d v2 = v[st[st.size() - 2]] - v[st.back()];
			if (v1.cross(v2) >= 0.0) break;
			st.pop_back();
		}
		st.push_back(pos);
	}
	st.pop_back();
	return st;
}