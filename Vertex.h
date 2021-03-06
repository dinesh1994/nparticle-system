#ifndef VERTEX_H
#define VERTEX_H
#include <algorithm>
#include <iostream>
#include <vector>
#include "vec3f.h"

using namespace std;

extern int verticeCount;

class Vertex{
  private:
    int id;
	public:
		Vec3f pos;
		Vec3f velocity;
		vector<Vec3f> forces;
		Vec3f netForce;

	Vertex();
	Vertex(float x, float y, float z);
  	int getId();
};

#endif
