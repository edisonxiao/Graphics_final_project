// Based on code by cobalthex on compilr.com 

#pragma once
#include <vector>
#include "Particle.h"


struct XY
{
	int x, y;
	Particle* p;
	XY(int X, int Y, Particle* P) : x(X), y(Y), p(P) { }
	XY() : x(0), y(0), p(NULL) { }
};

struct Rect
{
	int x, y, w, h;
	Rect() : x(0), y(0), w(0), h(0) { }
	Rect(int X, int Y, int W, int H) : x(X), y(Y), w(W), h(H) { }

	inline bool Contains(XY& p) { return (p.x >= x && p.y >= y && p.x < x + w && p.y < y + h); }
	inline bool Intersects(Rect& r) { return !(r.x > (x + w) || (r.x + r.w) < x || r.y > (y + h) || (r.y + r.h) < y); }
};

//A single layer of a quad tree
class QuadTree
{
public:
	// Arbitrary constant to indicate how many elements can be stored in this quad tree node
	static const int QT_NODE_CAPACITY = 4;

	// Axis-aligned bounding box stored as a center with half-dimensions
	// to represent the boundaries of this quad tree
	Rect bounds;

	//data inside
	XY points[4];
	int sz;

	QuadTree* nw;
	QuadTree* ne;
	QuadTree* sw;
	QuadTree* se;

	//Create a new quadtree
	QuadTree(Rect bounds);
	
	bool Insert(XY p);
	bool Subdivide();
	void DelChildren();
	void Resize(Rect NewBounds);

	std::vector<XY> QueryRange(Rect& range);
};