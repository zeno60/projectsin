#ifndef VERTEX_H
#define VERTEX_H

#include "vector3d.h"
#include "vector2d.h"
#include "color.h"

struct vertex
{
	vector3df mPosition;  
	vector3df mNormal;
	color mColor;
	vector2df mTexCoords;

	vertex(){}
	vertex(vector3df p, vector3df n, color c, vector2df t):mPosition(p),mNormal(n),mColor(c),mTexCoords(t){ }
};


#endif