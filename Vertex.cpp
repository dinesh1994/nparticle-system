#include "Vertex.h"

int verticeCount=0;

int Vertex::getId(){ return id; }

Vertex::Vertex(){
  id = verticeCount;
  verticeCount++;
  netForce = Vec3f(0.0, 0.0, 0.0);
  velocity = Vec3f(0.0, 0.0, 0.0);
}

Vertex::Vertex(float x, float y, float z)
{
    pos = Vec3f(x, y, z);
    id = verticeCount;
    verticeCount++;
    netForce = Vec3f(0.0, 0.0, 0.0);
    velocity = Vec3f(0.0, 0.0, 0.0);
}