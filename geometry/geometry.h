#ifndef ___GEOMETRY_LIBRARY
#define ___GEOMETRY_LIBRARY

#include <vector>
#include <algorithm>

template<class type>
class point2d {
public:
	type x, y;
	point2d() : x(type(0)), y(type(0)) {};
	point2d(type x_, type y_) : x(x_), y(y_) {};
	bool operator==(const point2d& p) { return x == p.x && y == p.y; }
	bool operator!=(const point2d& p) { return x != p.x || y != p.y; }
	point2d& operator+=(const point2d& p) { x += p.x; y += p.y; return *this; }
	point2d& operator-=(const point2d& p) { x -= p.x; y -= p.y; return *this; }
	point2d& operator*=(type val) { x *= val; y *= val; return *this; }
	point2d& operator/=(type val) { x /= val; y /= val; return *this; }
	point2d operator+(const point2d& p) const { return point2d(*this) += p; }
	point2d operator-(const point2d& p) const { return point2d(*this) -= p; }
	point2d operator*(type val) const { return point2d(*this) *= val; }
	point2d operator/(type val) const { return point2d(*this) /= val; }
	type norm() const { return x * x + y * y; }
	type abs() const { return type(std::sqrt(x * x + y * y)); }
	long double fabs() const { return std::sqrt((long double)(x * x + y * y)); }
	type dot(const point2d& p) const { return x * p.x + y * p.y; }
	type cross(const point2d& p) const { return x * p.y - y * p.x; }
	int ccw(const point2d& p1, const point2d& p2) const {
		// Centering at point p, p1 -> p2 is COUNTER-CLOCKWISE : (1)
		// Centering at point p, p1 -> p2 is CLOCKWISE : (-1)
		// Collinear with order p1 -> p -> p2 : (2)
		// Collinear with order p -> p1 -> p2 : (-2)
		// Collinear with order p -> p2 -> p1 : (0)
		point2d pr1 = p1 - point2d(*this);
		point2d pr2 = p2 - point2d(*this);
		if (pr1.cross(pr2) != 0) return pr1.cross(pr2) > 0 ? 1 : -1;
		if (pr1.dot(pr2) < 0) return 2;
		return pr1.norm() < pr2.norm() ? -2 : 0;
	}
};

template<class type>
class segment2d {
public:
	point2d<type> p1, p2;
	segment2d() : p1(point2d<type>()), p2(point2d<type>()) {};
	segment2d(const point2d<type>& p1_, const point2d<type>& p2_) : p1(p1_), p2(p2_) {};
	type norm() const { return (p1 - p2).norm(); }
	type abs() const { return (p1 - p2).abs(); }
	long double fabs() const { return (p1 - p2).fabs(); }
};

// DETERMINE IF "SEGMENT" S1 AND S2 INTERSECTS
template<class type>
bool intersect(const segment2d<type>& s1, const segment2d<type>& s2) {
	int f1 = s1.p1.ccw(s1.p2, s2.p1) * s1.p1.ccw(s1.p2, s2.p2);
	int f2 = s2.p1.ccw(s2.p2, s1.p1) * s2.p1.ccw(s2.p2, s1.p2);
	return (f1 <= 0 && f2 <= 0);
}

// FIND THE INTERSECTION POINT OF "LINE" S1 AND S2
template<class type>
point2d<type> intersect_point(const segment2d<type>& s1, const segment2d<type>& s2) {
	point2d<type> vec = s1.p2 - s1.p1;
	type d1 = vec.cross(s2.p1 - s1.p1);
	type d2 = vec.cross(s2.p2 - s1.p1);
	return s2.p1 + (s2.p2 - s2.p1) * d1 / (d1 - d2);
}

// FIND THE DISTANCE OF POINT P AND "LINE/SEGMENT" S
template<class type>
type distance(const point2d<type>& p, const segment2d<type>& s, bool isline) {
	type nx = (s.p2 - s.p1).norm();
	type d = std::abs((s.p2 - s.p1).cross(p - s.p1)) / std::sqrt(nx);
	if (isline) return d;
	type ny = (p - s.p1).norm();
	type nz = (p - s.p2).norm();
	if (nx + ny < nz) return std::sqrt(ny);
	if (nx + nz < ny) return std::sqrt(nz);
	return d;
}

// FIND CONVEX HULL OF A SET OF POINTS
template<class type>
std::vector<int> convex_hull(const std::vector<point2d<type> >& v) {
	int N = v.size();
	std::vector<int> perm(N);
	for (int i = 0; i < N; ++i) perm[i] = i;
	std::sort(perm.begin(), perm.end(), [&](int i, int j) { return v[i].y != v[j].y ? v[i].y < v[j].y : v[i].x < v[j].x; });
	std::vector<int> st; st.push_back(perm[0]);
	for (int i = 1; i < 2 * N - 1; ++i) {
		int pos = perm[i < N ? i : 2 * N - i - 2];
		while (st.size() >= 2) {
			point2d<type> v1 = v[pos] - v[st.back()];
			point2d<type> v2 = v[st[st.size() - 2]] - v[st.back()];
			if (v1.cross(v2) >= type(0)) break;
			st.pop_back();
		}
		st.push_back(pos);
	}
	st.pop_back();
	return st;
}

#endif // ___GEOMETRY_LIBRARY