#include "gocVisualSphere.h"

GOComponent::goc_id_type gocVisualSphere::mFamilyId = "gocVisualSphere";

gocVisualSphere::gocVisualSphere(int s)
{
	mSize = s;
	
	vertexBuffer = NULL;
	indexBuffer = NULL;

	GenerateBuffer();	
}

void gocVisualSphere::render() const
{	
	UINT uiStride = sizeof(vertex);
	UINT uiOffset = 0;
	
    Graphics()->GetDevice()->IASetVertexBuffers(0, 1, &vertexBuffer, &uiStride, &uiOffset);
    Graphics()->GetDevice()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);
    Graphics()->GetDevice()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Graphics()->GetDevice()->DrawIndexed(m_indices.size(), 0, 0);
}

void gocVisualSphere::GenerateBuffer()
{	
	vertex curr;

	int vertices = 2+mSize*mSize*2;
	int indices = (mSize*4 + mSize*4*(mSize-1))*3;

	m_verts = std::vector<vertex>(vertices);
	m_indices = std::vector<WORD>(indices);
	
	curr.mPosition = vector3df(0.0f, 1.0f, 0.0f);
	curr.mNormal = curr.mPosition.normalize();
	curr.mColor = color(1.0f, 1.0f, 1.0f, 1.0f);
	curr.mTexCoords = vector2df(0.0f, 0.0f);
	m_verts.insert(m_verts.begin(), curr);

	curr.mPosition = vector3df(0.0f, -1.0f, 0.0f);
	curr.mNormal = curr.mPosition.normalize();
	curr.mTexCoords = vector2df(1.0f, 1.0f);
	m_verts.insert(m_verts.begin()+1,curr);

	float  dj = 3.14159265/(mSize+1.0f);
	float  di = 3.14159265/mSize;
	
	for (int j=0; j<mSize; j++) {
		for (int i=0; i<mSize*2; i++) {
		vector3df    p;
		float        u, v;
		// Generate the x,y,&z coordinates from the equation
		// mentioned in the text
		p.y = (float) cos((j+1) * dj);
		p.x = (float) sin(i * di) * (float) sin((j+1) * dj);
		p.z = (float) cos(i * di) * (float) sin((j+1) * dj);

		// now assign the texture coordinates
		u = (float)i/mSize;
		if (u>1.0f) 
		u -= 1.0f;
		u = 1.0f - u;    // flip so texture is not mirrored
		v = (float)j/mSize;

		m_verts[2+i+j*mSize*2] = vertex(p, p, color(1.0f, 1.0f, 1.0f, 1.0f), vector2df(u,v));
		}
	}

	// now generate the traingle indices strip around north pole
	for (int i=0; i<mSize*2; i++) {
		m_indices[3*i] = 0; // always start tgl with the north pole
		m_indices[3*i+1] = i+2;
		m_indices[3*i+2] = i+3;
		if (i==mSize*2-1)
		m_indices[3*i+2] = 2;
	}

	// now all the middle strips
	int v;     // vertex offset
	int ind;  // indices offset
	for (int j=0; j<mSize-1; j++) {
		v = 2+j*mSize*2;
		ind = 3*mSize*2 + j*6*mSize*2;
		for (int i=0; i<mSize*2; i++) {
			m_indices[6*i+ind] = v+i;
			m_indices[6*i+2+ind] = v+i+1;
			m_indices[6*i+1+ind] = v+i+mSize*2;
			m_indices[6*i+ind+3] = v+i+mSize*2;
			m_indices[6*i+2+ind+3] = v+i+1;
			m_indices[6*i+1+ind+3] = v+i+mSize*2+1;
			if (i==mSize*2-1) {
				m_indices[6*i+2+ind] = v+i+1-2*mSize;
				m_indices[6*i+2+ind+3] = v+i+1-2*mSize;
				m_indices[6*i+1+ind+3] = v+i+mSize*2+1-2*mSize;
			}
		}
	}

	// finally strip around south pole
	v = vertices-mSize*2;
	ind = indices-3*mSize*2;
	for (int i=0; i<mSize*2; i++) {
		m_indices[3*i+ind] = 1; // the south pole is reused
		m_indices[3*i+1+ind] = v+i+1;
		m_indices[3*i+2+ind] = v+i;
		if (i==mSize*2-1)
			m_indices[3*i+1+ind] = v;
	}

	D3D10_BUFFER_DESC descBuffer;
	memset(&descBuffer, 0, sizeof(descBuffer));
    descBuffer.Usage = D3D10_USAGE_DEFAULT;
	descBuffer.ByteWidth = sizeof(vertex) * m_verts.size();
    descBuffer.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    descBuffer.CPUAccessFlags = 0;
    descBuffer.MiscFlags = 0;
   
	D3D10_SUBRESOURCE_DATA resData;
	memset(&resData, 0, sizeof(resData));
    resData.pSysMem = &m_verts[0];
	Graphics()->GetDevice()->CreateBuffer(&descBuffer, &resData, &vertexBuffer);

	descBuffer.Usage = D3D10_USAGE_DEFAULT;
    descBuffer.ByteWidth = sizeof(WORD) * m_indices.size();        
    descBuffer.BindFlags = D3D10_BIND_INDEX_BUFFER;
    descBuffer.CPUAccessFlags = 0;
    descBuffer.MiscFlags = 0;
    resData.pSysMem = &m_indices[0];
    Graphics()->GetDevice()->CreateBuffer(&descBuffer, &resData, &indexBuffer);
}