// Based on code by cobalthex on compilr.com 
#include "QuadTree.h"

QuadTree::QuadTree(Rect Bounds)
{
	bounds = Bounds;
	for (int i = 0; i < QT_NODE_CAPACITY; i++)
		points[i] = XY();
	sz = 0;
	ne = 0;
	se = 0;
	nw = 0;
	sw = 0;
}

bool QuadTree::Insert(XY p)
{
	//Ignore objects which are outside
	if (!bounds.Contains(p))
		return false;

	//If there is space in this quad tree, add the object here
	if (sz < QT_NODE_CAPACITY)
	{
		points[sz++] = p;
		return true;
	}

	//Otherwise, we need to subdivide then add the point to whichever node will accept it
	if (nw == 0)
		Subdivide();

	if (nw->Insert(p) || ne->Insert(p) || sw->Insert(p) || se->Insert(p))
		return true;

	//Otherwise, the point cannot be inserted for some unknown reason (which should never happen)
	return false;
}

bool QuadTree::Subdivide()
{
	if (ne != 0 || nw != 0 || sw != 0 || se != 0)
		return false;

	ne = new QuadTree(Rect(bounds.x + (bounds.w >> 1), bounds.y, bounds.w >> 1, bounds.h >> 1));
	se = new QuadTree(Rect(bounds.x + (bounds.w >> 1), bounds.y + (bounds.h >> 1), bounds.w >> 1, bounds.h >> 1));
	nw = new QuadTree(Rect(bounds.x, bounds.y, bounds.w >> 1, bounds.h >> 1));
	sw = new QuadTree(Rect(bounds.x, bounds.y + (bounds.h >> 1), bounds.w >> 1, bounds.h >> 1));
	return true;
}

void QuadTree::DelChildren()
{
	if (nw != 0)
	{
		nw->DelChildren();
		delete nw;
		nw = 0;
	}
	if (ne != 0)
	{
		ne->DelChildren();
		delete ne;
		ne = 0;
	}
	if (sw != 0)
	{
		sw->DelChildren();
		delete sw;
		sw = 0;
	}
	if (se != 0)
	{
		se->DelChildren();
		delete se;
		se = 0;
	}
}

void QuadTree::Resize(Rect New)
{
	bounds = New;
	if (ne != 0) ne->Resize(Rect(bounds.x + (bounds.w >> 1), bounds.y, bounds.w >> 1, bounds.h >> 1));
	if (nw != 0) nw->Resize(Rect(bounds.x + (bounds.w >> 1), bounds.y + (bounds.h >> 1), bounds.w >> 1, bounds.h >> 1));
	if (se != 0) se->Resize(Rect(bounds.x, bounds.y, bounds.w >> 1, bounds.h >> 1));
	if (sw != 0) sw->Resize(Rect(bounds.x, bounds.y + (bounds.h >> 1), bounds.w >> 1, bounds.h >> 1));
}

void AddGroup(std::vector<XY>& vtr, std::vector<XY>& toAdd)
{
	for (unsigned int i = 0; i < toAdd.size(); i++)
		vtr.push_back(toAdd[i]);
}

std::vector<XY> QuadTree::QueryRange(Rect& range)
{
	// Prepare an array of results
	std::vector<XY> pointsInRange;

	// Automatically abort if the range does not collide with this quad
	if (!bounds.Intersects(range))
		return pointsInRange; // empty list

	// Check objects at this quad level
	for (int p = 0; p < sz; p++)
	{
		if (range.Contains(points[p]))
			pointsInRange.push_back(points[p]);
	}

	// Terminate here, if there are no children
	if (nw == 0)
		return pointsInRange;

	// Otherwise, add the points from the children
	AddGroup(pointsInRange, nw->QueryRange(range));
	AddGroup(pointsInRange, ne->QueryRange(range));
	AddGroup(pointsInRange, sw->QueryRange(range));
	AddGroup(pointsInRange, se->QueryRange(range));

	return pointsInRange;
}