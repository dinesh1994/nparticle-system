#ifndef POLYGON_H
#define POLYGON_H
#include "Vertex.h"

class Polygon{
	public:
		vector<Vertex> vertices;
		//vector<vector<Vertex*> > edges;
		vector<vector<int> > edges;
		Vec3f center;
		bool use;
	Polygon();
	Polygon(float x, float y, float z);
	};

#endif
