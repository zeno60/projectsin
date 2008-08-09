#ifndef IVIDEODEVICE_H
#define IVIDEODEVICE_H

#include "MeshBuffer.h"
#include "matrix.h"

class MeshBuffer;

class IVideoDevice
{
public:
	virtual ~IVideoDevice() {}

	virtual void setWorldTransform(const matrixf& mtx) = 0;
	virtual void setViewTransform(const matrixf& mtx) = 0;
	virtual void setProjTransform(const matrixf& mtx) = 0;

	virtual const matrixf& getWorldTransform() = 0;
	virtual const matrixf& getViewTransform() = 0;
	virtual const matrixf& getProjTransform() = 0;

	virtual void drawVertexPrimitiveList(const void* vertices, int vertexCount, const short int* indices, int triangleCount) = 0;
	virtual void drawIndexedTriangleList(const vertex* vertices, int vertexCount, const short int* indices, int triangleCount) = 0;
	
	virtual void drawMeshBuffer(const MeshBuffer* mb) = 0;

	virtual const wchar_t* getName() = 0;
	/*
		Needs:
		-draw3dline
		-draw3dtriangle
		-draw2dimage
		-addlight
	*/
};

#endif