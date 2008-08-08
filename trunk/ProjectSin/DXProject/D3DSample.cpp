#include "Application.h"
#include <string>
#include <vector>

#include "matrix4.h"
#include "GCTManager.h"
#include "GOTManager.h"
#include "GCTTest.h"
#include "gctVisualSphere.h"
#include "camera.h"
#include "gocCamera.h"

class D3DSample : public Application
{
public:
	virtual void DoFrame(float timeDelta);
	virtual void SceneInit();

	D3DSample():Application()
	{
		mTitle = std::wstring(L"D3DSample App");
	}

private:
	std::vector<GameObject*> objects;
	Camera camera;
};

Application* CreateApplication()
{
	return new D3DSample();
}

void D3DSample::SceneInit()
{	
	//GCTTest::RegisterTemplate();
	//gctVisualSphere::RegisterTemplate();

	GOTemplate* got = new GOTemplate("Test");
	got->addGCTemplate(new gctVisualSphere);
	got->addGCTemplate(new GCTTest);

	GOTManager::getInstance()->registerTemplate(got);
	
	GOComponent* goc = new gocCamera();

	GameObject* go = GOTManager::getInstance()->createGO("Test");
	go->setGOComponent(goc);
	go->setPosition(vector3df(1.0f, 0.0f, 1.0f));
	objects.push_back(go);

	//go = GOTManager::getInstance()->createGO("Test");
	//objects.push_back(go);

	Graphics()->AddLight(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), vector3df(0.0f, 10.0f, 0.0f).toD3DXVECTOR3());
	//Graphics()->AddLight(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), vector3df(1.0f, 0.0f, 0.0f).toD3DXVECTOR3());
}

void D3DSample::DoFrame(float timeDelta)
{
	if(!Graphics()) 
		return;
	
	for(int i = 0; i < objects.size(); i++)
	{
		GameObject* go = objects[i];
		if(go)
		{
			GOComponent* gocomp = go->getGOComponent(GOComponent::goc_id_type("gocCamera"));
			gocCamera* gocCam = static_cast<gocCamera*>(gocomp);
			if(gocCam)
			{
				matrixf v;
				gocCam->getViewMatrix(&v);
				v = gocCam->getMatrix() * v;
				Graphics()->SetViewMtx(v);
				gocCam->update(timeDelta);
			}
		}
	}
	/*matrixf v;
	camera.getViewMatrix(&v);
	
	v = camera.getMatrix() * v;
	//matrixf view = camera.getMatrix() * v;

	Graphics()->SetViewMtx(v);
	//Graphics()->GetViewMatrixVariable()->SetMatrix((float*)&view);

	camera.update(timeDelta);*/

	// Clear the previous contents of the backbuffer
	float colClear[4] = {0.2f, 0.2f, 0.5f , 1.0f};
	Graphics()->Clear(colClear);
	Graphics()->ClearDepthStencil(1.0f, 0);

	Graphics()->UpdateShaderMatrices();
	Graphics()->UpdateLights();

	for(int i = 0; i < objects.size(); i++)
	{
		GameObject* go = objects[i];
		if(go)
		{
			GOComponent* gocomp = go->getGOComponent(GOComponent::goc_id_type("GOCTest"));
			//GOCTest* gocTest = static_cast<GOCTest*>(gocomp);
			//if(gocTest) gocTest->update();
			gocomp->update(timeDelta);
		}
	}

	D3D10_TECHNIQUE_DESC descTechnique;
    Graphics()->GetDefaultTechnique()->GetDesc(&descTechnique);

	for(int i = 0; i < objects.size(); i++)
	{
		GameObject* go = objects[i];
		if(go)
		{
			matrixf objTrans = go->getGOTransformation();
			Graphics()->GetWorldMatrixVariable()->SetMatrix((float*)&objTrans);
			for(UINT uiCurPass = 0; uiCurPass < descTechnique.Passes; uiCurPass++)
			{
				Graphics()->GetDefaultTechnique()->GetPassByIndex(uiCurPass)->Apply(0);
				GOComponent* gocomp = go->getGOComponent(GOComponent::goc_id_type("gocVisual"));
				gocVisual* gocVis = static_cast<gocVisual*>(gocomp);
				if(gocVis) gocVis->render();
			}
		}
	}
	
	/*wchar_t* str = new wchar_t[128];
	wchar_t format[] = L"x: %f, y: %f, z: %f";

	swprintf(str,128,format,mat(0,3),mat(1,3),mat(2,3));

	Graphics()->DrawTextString(0, 0, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), str);*/
	
	matrixf view = Graphics()->GetViewMtx();

	wchar_t* str = new wchar_t[128];
	wchar_t format[] = L"x: %f, y: %f, z: %f";

	swprintf(str,128,format,view[12],view[13],view[14]);

	Graphics()->DrawTextString(0, 0, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), str);

	Graphics()->Present();
}