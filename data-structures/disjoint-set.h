#ifndef CLASS_DISJOINT_SET
#define CLASS_DISJOINT_SET

#include <cstdint>

class disjoint_set {
private:
	std::size_t n, *par, *sz;
public:
	disjoint_set() : n(0) {};
	disjoint_set(std::size_t n_) : n(n_) { par = new std::size_t[n]; sz = new std::size_t[n]; for(std::size_t i = 0; i < n; ++i) par[i] = i, sz[i] = 1; }
	~disjoint_set() { delete par, sz; }
	std::size_t size() const { return n; }
	std::size_t root(std::size_t vert) {
		if(vert == par[vert]) return vert;
		return par[vert] = root(par[vert]);
	}
	void link(std::size_t vertx, std::size_t verty) {
		vertx = root(vertx);
		verty = root(verty);
		if(vertx == verty) return;
		if(sz[vertx] > sz[verty]) sz[vertx] += sz[verty], sz[verty] = 0, par[verty] = vertx;
		else sz[verty] += sz[vertx], sz[vertx] = 0, par[vertx] = verty;
	}
	bool connected(std::size_t vertx, std::size_t verty) {
		return root(vertx) == root(verty);
	}
};

#endif // CLASS_DISJOINT_SET