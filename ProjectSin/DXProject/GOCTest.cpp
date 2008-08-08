#include "GOCTest.h"

#include "GameObject.h"
#include "D3DDevice.h"

GOComponent::goc_id_type GOCTest::mComponentId = "GOCTest";
GOComponent::goc_id_type GOCTest::mFamilyId = "GOCTest";

GOCTest::GOCTest(GCTTest *gct)
{
}


void GOCTest::update(float timeDelta)
{
	GameObject* go = this->getParentGO();
	if(go)
	{		
		//go->setRotation(vector3df(t,0.0f,0.0f));
		//go->setPosition(vector3df(0.0f, -t, 0.0f));

		//D3DXMATRIX origMat = go->getGOTransformation();
		//D3DXMATRIX rotMat;

		//D3DXMatrixRotationX(&rotMat, t);
		//D3DXMatrixMultiply(&mat, &mat, &mat2);
		//D3DXMatrixTranslation(&mat, t, t, t);
		
		//D3DXMATRIX m = rotMat * origMat;
		//go->setGOTransformation(m);
	}
}