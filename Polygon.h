#ifndef POLYGON_H
#define POLYGON_H
#include "Vertex.h"

class Polygon{
	public:
		vector<Vertex> vertices;
		Vec3f center;
		bool use;
	Polygon();
	Polygon(float x, float y, float z);
	};

#endif
