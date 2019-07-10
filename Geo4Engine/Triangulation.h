#pragma once

#include<vector>
#include "Math/HMath.h"
#include "Math/HVector2.h"

class Triangulation
{
public:
	Triangulation();
	~Triangulation();

	enum PointType {UNKNOWN=0, CONVEX=1, CONCAVE=2};

	class PointIndex {
	public:
		PointIndex() {}
		PointIndex(unsigned int i, Vector2 p) : index(i), point(p), type(PointType::UNKNOWN) {	}
		PointIndex(unsigned int i, PointType t ) : index(i),type(t){	}
		unsigned int	index;
		Vector2			point;
		PointType		type;
	};
	class Triangle {
	public:
		Triangle(PointIndex a, PointIndex b, PointIndex c) : a(a), b(b), c(c){}
		PointIndex a;
		PointIndex b;
		PointIndex c;
	};

	std::vector<PointIndex>	pointIndices;
	std::vector<Triangle>	triangles;

	void EarClipTrinagulate(const std::vector<Vector2>& pointsIn,
		std::vector<unsigned int>& triangleIndexOut);
	
private:
	bool			areVerticesCW(const std::vector<Vector2>& pointsIn);
	PointType		classifyPoint(unsigned int i);
	PointType		computeSpannedAreaSign(const Vector2& p1, const Vector2& p2, const Vector2& p3);
	unsigned int	findEar();
	void			sliceEar(unsigned int);
	unsigned int	nextPoint(unsigned int);
	unsigned int	previousPoint(unsigned int);
};

