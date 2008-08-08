#ifndef GOCVISUALSPHERE_HEADER
#define GOCVISUALSPHERE_HEADER

#include "gocVisual.h"
#include "vertex.h"
#include "triangle.h"
#include <vector>

class gocVisualSphere : public gocVisual
{
public:
	virtual const goc_id_type& componentId() const { return mFamilyId; }
	virtual void render() const;

	gocVisualSphere(int r);

	const float getSize() const { return mSize; }
	void setSize(const float r) { mSize = r; }

private:
	int mSize;
	static goc_id_type mFamilyId;

	ID3D10Buffer* vertexBuffer;
	ID3D10Buffer* indexBuffer;
	
	typedef triangle<WORD> sTri;

	std::vector<vertex> m_verts;
	std::vector<sTri> m_tris;
	std::vector<WORD> m_indices;

	void GenerateBuffer();
};

#endif