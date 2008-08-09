#ifndef MESHBUFFER_H
#define MESHBUFFER_H

#include "vertex.h"
#include <vector>

template<class T>
class MeshBuffer
{
public:
	MeshBuffer() {}
	~MeshBuffer() {}

	void* getVertexData() { return vertices.pointer(); }
	int getVertexCount() { return vertices.size(); }

	const short int* getIndexData() { return indices.pointer(); }
	int getIndexCount() { return indices.size(); }

	vector<T> vertices;
	vector<short int> indices;
};

typedef MeshBuffer<vertex> SMeshBuffer;

#endif