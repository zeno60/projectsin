#include "D3DDevice.h"
#include "Win32Device.h"
#include "D3DHelper.h"

D3DDevice* D3DDevice::mInstance = NULL;

D3DDevice::D3DDevice(HWND hWnd)
{
	mInstance = this;

	mHWnd = hWnd;
	mDevice = NULL;
	mBackBuffer = NULL;
	mRenderTargetView = NULL;
	mSwapChain = NULL;

	mWorldMtx.makeIdentity();
	mViewMtx.makeIdentity();
	mProjMtx.makeIdentity();

	m_iNumLights = 0;
}

D3DDevice::~D3DDevice()
{
	DestroyDevice();
}

D3DDevice* D3DDevice::getInstance()
{
	return mInstance;
}

void D3DDevice::DestroyDevice()
{
	if(mDevice) mDevice->ClearState();

	Release(mDefaultInputLayout);
	Release(mDefaultEffect);
	Release(mFont);
	Release(mFontSprite);
	Release(mDepthStencilView);
	Release(mDepthStencilState);
	Release(mRenderTargetView);
    Release(mBackBuffer);	
	Release(mDepthStencilBuffer);
	Release(mSwapChain);
	Release(mDevice);

	mInstance = NULL;
}

void D3DDevice::Create(HWND hWnd, int width, int height)
{
	new D3DDevice(hWnd);
	getInstance()->InitD3D(width, height, 32);
}

bool D3DDevice::CreateDeviceAndSwapChain()
{
	HRESULT hr = 0;

	DXGI_SWAP_CHAIN_DESC descSwap;
	ZeroMemory(&descSwap, sizeof(descSwap));

	descSwap.BufferCount = 1;

    descSwap.BufferDesc.Width = mScreenDimensions.right;
    descSwap.BufferDesc.Height = mScreenDimensions.bottom;

    descSwap.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// 60hz refresh rate
    descSwap.BufferDesc.RefreshRate.Numerator = 60;
    descSwap.BufferDesc.RefreshRate.Denominator = 1;
    descSwap.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Connect it to our main window
    descSwap.OutputWindow = MainWindow()->GetHWnd();

	// No multisampling
    descSwap.SampleDesc.Count = 1;
    descSwap.SampleDesc.Quality = 0;

    descSwap.Windowed = TRUE;
	
	// Create the device using hardware acceleration 
	hr = D3D10CreateDeviceAndSwapChain( 
			NULL,
			D3D10_DRIVER_TYPE_HARDWARE,
			NULL,
			D3D10_CREATE_DEVICE_DEBUG,
			D3D10_SDK_VERSION,
			&descSwap,		
			&mSwapChain,
			&mDevice);

	if(FAILED(hr)) return false;

	// Get a copy of the pointer to the back buffer
    hr = mSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&mBackBuffer);

    if(FAILED(hr)) return false;

	// Create a render target view
    hr = mDevice->CreateRenderTargetView(mBackBuffer, NULL, &mRenderTargetView);

    if(FAILED(hr)) return false;
	
	return true;
}

bool D3DDevice::CreateViewport()
{
	// Create a viewport the same size as the backbuffer
	D3D10_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(vp));
    vp.Width = mScreenDimensions.right;
    vp.Height = mScreenDimensions.bottom;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    mDevice->RSSetViewports( 1, &vp );
	return true;
}

bool D3DDevice::CreateDebugText()
{
	// Create the font for rendering text
	D3DX10CreateFont(mDevice, 
		15, 0, 
		FW_BOLD, 
		1, 
		FALSE, 
		DEFAULT_CHARSET, 
        OUT_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH | FF_DONTCARE, 
		L"Arial", 
		&mFont);

	// Create the sprite to use to render letters
	D3DX10CreateSprite(mDevice, D3DDevice::MAX_CHARS_PER_FRAME, &mFontSprite);

	return true;
}

bool D3DDevice::CreateDepthStencilBuffer()
{
	HRESULT hr = 0;

	// Create the depth buffer
	D3D10_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = mScreenDimensions.right;
	descDepth.Height = mScreenDimensions.bottom;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D10_USAGE_DEFAULT;
	descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = mDevice->CreateTexture2D(&descDepth, NULL, &mDepthStencilBuffer);

	if(FAILED(hr)) return false;

	D3D10_DEPTH_STENCIL_DESC descDS;
	ZeroMemory(&descDS, sizeof(descDS));
	descDS.DepthEnable = true;
	descDS.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
	descDS.DepthFunc = D3D10_COMPARISON_LESS;

	// Stencil test values
	descDS.StencilEnable = true;
	descDS.StencilReadMask =  (UINT8)0xFFFFFFFF;
	descDS.StencilWriteMask = (UINT8)0xFFFFFFFF;

	// Stencil op if pixel is front
	descDS.FrontFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
	descDS.FrontFace.StencilDepthFailOp = D3D10_STENCIL_OP_INCR;
	descDS.FrontFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
	descDS.FrontFace.StencilFunc = D3D10_COMPARISON_ALWAYS;

	// Stencil op if pixel is back
	descDS.BackFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
	descDS.BackFace.StencilDepthFailOp = D3D10_STENCIL_OP_DECR;
	descDS.BackFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
	descDS.BackFace.StencilFunc = D3D10_COMPARISON_ALWAYS;

	hr = mDevice->CreateDepthStencilState(&descDS, &mDepthStencilState);
	
	if(FAILED(hr)) return false;

	mDevice->OMSetDepthStencilState(mDepthStencilState, 1);

	D3D10_DEPTH_STENCIL_VIEW_DESC descDSView;
	ZeroMemory(&descDSView, sizeof(descDSView));
	descDSView.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSView.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	descDSView.Texture2D.MipSlice = 0;

	hr = mDevice->CreateDepthStencilView(mDepthStencilBuffer, &descDSView, &mDepthStencilView);
	if(FAILED(hr)) return false;

	return true;
}

bool D3DDevice::CreateDefaultShader()
{
	HRESULT hr = 0;

	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;

    #if defined( DEBUG ) || defined( _DEBUG )
		// Turn on extra debug info when in debug config
		shaderFlags |= D3D10_SHADER_DEBUG;
    #endif

	ID3D10Blob* pErrors = 0;

	// Create the default rendering effect
	hr = D3DX10CreateEffectFromFile(L"DefaultShader.fx", NULL, NULL, "fx_4_0", shaderFlags, 0, 
                                     mDevice, NULL, NULL, &mDefaultEffect, &pErrors, NULL);
	if(pErrors)
	{
		/*char* pCompileErrors = static_cast<char*>(pErrors->GetBufferPointer());
		TCHAR wcsErrors[MAX_PATH];
        mbstowcs(wcsErrors, pCompileErrors, MAX_PATH);
		OutputDebugString(wcsErrors);*/
	}

	if(FAILED(hr)) return false;

	mDefaultTechnique = mDefaultEffect->GetTechniqueByName("DefaultTechnique");
	mDefaultTechniqueLight = mDefaultEffect->GetTechniqueByName("DefaultTechniqueLight");

	if(!mDefaultTechnique || !mDefaultTechniqueLight)
	{
		::MessageBox(0, L"Could not find default techniques in DefaultShader.fx", 0, 0);
		return false;
	}

	// Setup the input layout
    D3D10_INPUT_ELEMENT_DESC defaultLayout[] =
    {
        {"POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,								D3D10_INPUT_PER_VERTEX_DATA, 0},  
		{"NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D10_APPEND_ALIGNED_ELEMENT,	D3D10_INPUT_PER_VERTEX_DATA, 0}, 
		{"COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D10_APPEND_ALIGNED_ELEMENT,	D3D10_INPUT_PER_VERTEX_DATA, 0}, 
		{"TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D10_APPEND_ALIGNED_ELEMENT,	D3D10_INPUT_PER_VERTEX_DATA, 0}, 
	};

	UINT uiNumElements = sizeof(defaultLayout)/sizeof(defaultLayout[0]);
	D3D10_PASS_DESC descPass;
    mDefaultTechnique->GetPassByIndex(0)->GetDesc(&descPass);
    hr = mDevice->CreateInputLayout(defaultLayout, uiNumElements, 
		descPass.pIAInputSignature, descPass.IAInputSignatureSize, &mDefaultInputLayout);

    if(FAILED(hr)) return false;

	mDevice->IASetInputLayout(mDefaultInputLayout);
	
	mWorldMtxVar = mDefaultEffect->GetVariableByName("World")->AsMatrix();
    mViewMtxVar = mDefaultEffect->GetVariableByName("View")->AsMatrix();
    mProjMtxVar = mDefaultEffect->GetVariableByName("Projection")->AsMatrix();
	
	/*
	matrixf mtxView;
	vector3df vCamPos(0.0f, 1.0f, -5.0f);
    vector3df vCamAt(0.0f, 1.0f, 0.0f);
    vector3df vCamUp(0.0f, 1.0f, 0.0f);
    D3DXMatrixLookAtLH(&mtxView, &vCamPos, &vCamAt, &vCamUp);

	SetViewMtx(mtxView);

	D3DXMATRIX mtxProj;
    D3DXMatrixPerspectiveFovLH(&mtxProj, 45.0f, 
		mScreenDimensions.right/(float)mScreenDimensions.bottom, 0.1f, 100.0f);

	SetProjMtx(mtxProj);*/
	
	matrixf proj;
	
	proj.buildPerspectiveFovLH(math::pi * 0.5f, mScreenDimensions.right/mScreenDimensions.bottom, 0.0f, 1000.0f);

	SetProjMtx(proj);

	UpdateShaderMatrices();

	mLightDirVec = mDefaultEffect->GetVariableByName( "g_vLightDirections" )->AsVector();
    mLightColorVec = mDefaultEffect->GetVariableByName( "g_vLightColors" )->AsVector();
	mNumLightsVec = mDefaultEffect->GetVariableByName( "g_viLightStatus" )->AsVector();
	mOutputColorVec = mDefaultEffect->GetVariableByName( "g_vOutputColor" )->AsVector();
}

bool D3DDevice::InitD3D(int width, int height, int bpp)
{
	HRESULT hr = 0;
	
	// Keep a copy of the screen dimensions
	mScreenDimensions.left = 0;
	mScreenDimensions.top = 0;
	mScreenDimensions.right = width;
	mScreenDimensions.bottom = height;	

	CreateDeviceAndSwapChain();
	CreateViewport();
	CreateDepthStencilBuffer();
	CreateDebugText();

	// Attach the render target view to the output merger state
    mDevice->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	CreateDefaultShader();

	return true;
}

void D3DDevice::Present()
{
	HRESULT hr = S_OK;
    
	hr = mSwapChain->Present(0, 0);

	if(FAILED(hr))
	{
		::MessageBox(0, L"Present Failed!\n", 0, 0);
	}
}

void D3DDevice::Clear(const float (&colClear)[4])
{
	HRESULT r = S_OK;
	mDevice->ClearRenderTargetView(mRenderTargetView, colClear);
}

void D3DDevice::UpdateShaderMatrices()
{
	mWorldMtxVar->SetMatrix((float*)&mWorldMtx);
    mViewMtxVar->SetMatrix((float*)&mViewMtx);
    mProjMtxVar->SetMatrix((float*)&mProjMtx);
}

void D3DDevice::ClearDepthStencil(const float fDepth, const UINT8 uiStencil)
{
	mDevice->ClearDepthStencilView(mDepthStencilView, D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, fDepth, uiStencil);
}

void D3DDevice::UpdateLights()
{	
	int iLightData[4] = {m_iNumLights, 0, 0, 0};
	mNumLightsVec->SetIntVector(iLightData);

	for(int iCurLight = 0 ; iCurLight < m_iNumLights ; iCurLight++)
	{
		vector3df viewLightDir = m_aLights[iCurLight].mDirection;
		viewLightDir.normalize();
		mWorldMtx.transform(viewLightDir);

		viewLightDir.normalize();

		//g_pViewSpaceLightDir->SetFloatVector( (float*)&viewLightDir );
		mLightDirVec->SetFloatVector((float*)&viewLightDir);
		//mLightDirVec->SetFloatVectorArray((float*)m_aLights[iCurLight].mDirection, iCurLight, 1);
		mLightColorVec->SetFloatVectorArray((float*)m_aLights[iCurLight].mColor, iCurLight, 1);
	}
}

void D3DDevice::AddLight(const D3DXCOLOR& vColor, const D3DXVECTOR3 vDir)
{
	if(m_iNumLights < 10)
	{		
		m_aLights[m_iNumLights].mColor = vColor;
		m_aLights[m_iNumLights].mDirection = vDir;
		m_iNumLights++;
	}
}

void D3DDevice::DrawTextString(int x, int y, D3DXCOLOR color, const TCHAR* strOutput)
{
	mFontSprite->Begin(D3DX10_SPRITE_SAVE_STATE);
	RECT rect = {x, y, mScreenDimensions.right, mScreenDimensions.bottom};
	mFont->DrawText(mFontSprite, strOutput, -1, &rect, DT_LEFT, color);
	mFontSprite->End();
}
