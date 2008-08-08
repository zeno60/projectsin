#ifndef D3DDEVICE_H
#define D3DDEVICE_H

#include <d3d10.h>
#include <d3dx10.h>
#include "vector3d.h"
#include "matrix4.h"

class D3DDevice
{
protected:
	HWND mHWnd;
	ID3D10Device*			mDevice;
	ID3D10Texture2D*		mBackBuffer;
	ID3D10RenderTargetView* mRenderTargetView;
	IDXGISwapChain*			mSwapChain;
	RECT					mScreenDimensions;

	ID3D10Texture2D* mDepthStencilBuffer;
	ID3D10DepthStencilState* mDepthStencilState;
	ID3D10DepthStencilView* mDepthStencilView;

	ID3D10Effect* mDefaultEffect;
	ID3D10EffectTechnique* mDefaultTechnique;
	ID3D10EffectTechnique* mDefaultTechniqueLight;
	ID3D10InputLayout* mDefaultInputLayout;
public:
	ID3D10EffectMatrixVariable* mWorldMtxVar;
	ID3D10EffectMatrixVariable* mViewMtxVar;
	ID3D10EffectMatrixVariable* mProjMtxVar;
private:
	ID3D10EffectVectorVariable* mLightDirVec;
	ID3D10EffectVectorVariable* mLightColorVec;
	ID3D10EffectVectorVariable* mNumLightsVec;
	ID3D10EffectVectorVariable* mOutputColorVec;
	
	matrixf mWorldMtx;
	matrixf mViewMtx;
	matrixf mProjMtx;

	ID3DX10Font* mFont;
	ID3DX10Sprite* mFontSprite;

	class cLight
	{
	public:
		cLight()
		{
		}

		D3DXCOLOR mColor;
		vector3df mDirection;
	};
public:
	cLight m_aLights[10];
	int m_iNumLights;
private:

	D3DDevice(HWND hWnd);

	static D3DDevice* mInstance;

	static const UINT MAX_CHARS_PER_FRAME = 512;
	

public:
	~D3DDevice();
	
	// Accessors
	ID3D10Device* GetDevice() { return mDevice; }
	ID3D10EffectTechnique* GetDefaultTechnique() const {return mDefaultTechnique;}
	ID3D10EffectTechnique* GetDefaultTechniqueLight() const { return mDefaultTechniqueLight; }
	
	/*void SetWorldMtx(const D3DXMATRIX& mtx) { mWorldMtx = mtx; }
	void SetViewMtx(const D3DXMATRIX& mtx) { mViewMtx = mtx; }
	void SetProjMtx(const D3DXMATRIX& mtx) { mProjMtx = mtx;  }*/

	void SetWorldMtx(const matrixf& mtx) { mWorldMtx = mtx; }
	void SetViewMtx(const matrixf& mtx) { mViewMtx = mtx; }
	void SetProjMtx(const matrixf& mtx) { mProjMtx = mtx;  }
	
	const matrixf& GetWorldMtx() const { return mWorldMtx; }
	const matrixf& GetViewMtx() const { return mViewMtx; }
	const matrixf& GetProjdMtx() const { return mProjMtx; }

	ID3D10EffectMatrixVariable* GetWorldMatrixVariable() const { return mWorldMtxVar; }
	ID3D10EffectMatrixVariable* GetViewMatrixVariable() const { return mViewMtxVar; }
	ID3D10EffectMatrixVariable* GetProjectionMatrixVariable() const { return mProjMtxVar; }

	ID3D10EffectVectorVariable* GetOutputColorVector() const { return mOutputColorVec; }

	void DrawTextString(int x, int y, D3DXCOLOR color, const TCHAR* strOutput);

	void UpdateShaderMatrices();
	void UpdateLights();

	void ClearDepthStencil(const float fDepth, const UINT8 uiStencil);
	
	void AddLight(const D3DXCOLOR& vColor, const D3DXVECTOR3 vDir);

	static D3DDevice* getInstance();
	static void Create(HWND hWnd, int width, int height);

	bool InitD3D(int width, int height, int bpp);
	bool CreateDeviceAndSwapChain();
	bool CreateViewport();
	bool CreateDebugText();
	bool CreateDepthStencilBuffer();
	bool CreateDefaultShader();

	void Present();
	void Clear(const float (&colClear)[4]);

	void DestroyDevice();

};

inline D3DDevice* Graphics()
{
	return D3DDevice::getInstance();
}

#endif