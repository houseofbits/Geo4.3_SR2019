#include "Triangulation.h"
#include "StringUtils.h"


Triangulation::Triangulation() : pointIndices(0)
{

}


Triangulation::~Triangulation()
{
}


void Triangulation::EarClipTrinagulate(const std::vector<Vector2>& pointsIn,
	std::vector<unsigned int>& triangleIndexOut) {

	pointIndices.clear();
	triangles.clear();

	//Generate clockwise indices
	if (areVerticesCW(pointsIn)) {
		for (unsigned int i = 0; i < (unsigned int)pointsIn.size(); i++){
			pointIndices.push_back(PointIndex(i, pointsIn[i]));
		}
	}else {
		for (unsigned int i = (unsigned int)pointsIn.size() - 1; i >= 0; i--) {
			pointIndices.push_back(PointIndex(i, pointsIn[i]));
		}
	}
	//Classify points - inside angle or outside angle (concave or convex)
	//cout << "------ classify points ------------" << endl;
	for (unsigned int i = 0; i < pointIndices.size(); i++) {
		pointIndices[i].type = classifyPoint(i);
		//cout << pointIndices[i].index<<" / "<<Utils::VectorToString(pointIndices[i].point)<<" / "<<pointIndices[i].type << endl;
	}

	//Do some triangulation
	while (pointIndices.size() > 3) {
		unsigned int eari = findEar();
		sliceEar(eari);
		
		for (unsigned int i = 0; i < pointIndices.size(); i++) {
			pointIndices[i].type = classifyPoint(i);
		}
	}
	//Process last 3 points
	triangles.push_back(Triangle(
		pointIndices[0], 
		pointIndices[1], 
		pointIndices[2]
	));
	//cout << "--------- triangles: " << triangles.size()<<endl;
	for (unsigned int i = 0; i < triangles.size(); i++){
		triangleIndexOut.push_back(triangles[i].a.index);
		triangleIndexOut.push_back(triangles[i].b.index);
		triangleIndexOut.push_back(triangles[i].c.index);
		//cout << triangles[i].a.index<<", "<< triangles[i].b.index <<", "<< triangles[i].c.index << endl;
	}
}

unsigned int Triangulation::findEar() {
	for (unsigned int i = 0; i < pointIndices.size(); i++) {
		//Consider only CONVEX points
		if (pointIndices[i].type == PointType::CONCAVE)continue;

		unsigned int previ = previousPoint(i);
		unsigned int nexti = nextPoint(i);
		//Check if point is in triangle
		for (unsigned int a = 0; a < pointIndices.size(); a++) {
			if (a != i && a != previ && a != nexti && pointIndices[a].type != PointType::CONVEX) {
				if (Math::PointInsideTriangle(pointIndices[a].point,
					pointIndices[previ].point,
					pointIndices[i].point,
					pointIndices[nexti].point)) {
					continue; //Considered ear has points inside of it, continue to next
				}
			}
		}
		return i;
	}
	//If nothing is found, get first which is convex or paralel
	for (unsigned int i = 0; i < pointIndices.size(); i++) {
		if (pointIndices[i].type != PointType::CONCAVE)return i;
	}
	return 0;
}

void Triangulation::sliceEar(unsigned int i) {
	triangles.push_back(Triangle(pointIndices[previousPoint(i)], pointIndices[i], pointIndices[nextPoint(i)]));
	pointIndices.erase(pointIndices.begin() + i);
}

bool Triangulation::areVerticesCW(const std::vector<Vector2>& pointsIn) {
	float area = 0, p1x, p1y, p2x, p2y;
	for (int i = 0, n = (unsigned int)pointsIn.size() - 1; i < n; i += 2) {
		p1x = pointsIn[i].x;
		p1y = pointsIn[i].y;
		p2x = pointsIn[i + 1].x;
		p2y = pointsIn[i + 1].y;
		area += p1x * p2y - p2x * p1y;
	}
	p1x = pointsIn[pointsIn.size() - 1].x;
	p1y = pointsIn[pointsIn.size() - 1].y;
	p2x = pointsIn[0].x;
	p2y = pointsIn[0].y;
	return area + p1x * p2y - p2x * p1y < 0;
}

Triangulation::PointType Triangulation::classifyPoint(unsigned int i) {
	return computeSpannedAreaSign(pointIndices[previousPoint(i)].point, pointIndices[i].point, pointIndices[nextPoint(i)].point);
}

Triangulation::PointType Triangulation::computeSpannedAreaSign(const Vector2& p1, const Vector2& p2, const Vector2& p3) {
	float area = p1.x * (p3.y - p2.y);
	area += p2.x * (p1.y - p3.y);
	area += p3.x * (p2.y - p1.y);
	if (area < 0)return PointType::CONCAVE;
	if (area > 0)return PointType::CONVEX;
	return PointType::UNKNOWN;
}
unsigned int Triangulation::nextPoint(unsigned int i) {
	return (i + 1) % pointIndices.size();
}
unsigned int  Triangulation::previousPoint(unsigned int i) {
	unsigned int previ = i;
	if (i > 0)previ = i - 1;
	else previ = (unsigned int)pointIndices.size() - 1;
	return previ;
}
