#include "Application.h"
#include "Win32Device.h"

Application* Application::mInstance = NULL;

HINSTANCE globalInstance;

HINSTANCE ApplicationInstance()
{
	return globalInstance;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	Application* pApp;

	globalInstance = hInstance;

	pApp = CreateApplication();

	pApp->Init();
	pApp->SceneInit();
	pApp->Run();

	delete pApp;
	return 0;
}

Application::Application()
{
	mInstance = this;

	mTitle = std::wstring(L"Default window name");
	mWidth = 800;
	mHeight = 600;
	mBpp = 32;
	mActive = true;
}

Application::~Application()
{
	delete Graphics();
	delete MainWindow();
}

void Application::Init()
{
	InitPrimaryWindow();
	InitGraphics();
	InitExtraSubsystems();
}

void Application::Run()
{
	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));
	
	static float lastTime = static_cast<float>(timeGetTime()); 

	while(msg.message != WM_QUIT)
	{
		if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
        {	
			float currTime  = static_cast<float>(timeGetTime());
			float timeDelta = (currTime - lastTime)*0.001f;

			if(mActive) DoFrame(timeDelta);
			else DoIdleFrame(timeDelta);

			lastTime = currTime;
        }
    }
}

void Application::DoFrame(float timeDelta)
{
}

void Application::DoIdleFrame(float timeDelta)
{
}

void Application::SceneInit()
{
}

void Application::InitPrimaryWindow()
{
	new Win32Device(mWidth, mHeight, mTitle.c_str());

	MainWindow()->RegisterClass();
	MainWindow()->InitInstance();
}

void Application::InitGraphics()
{
	D3DDevice::Create(MainWindow()->GetHWnd(), mWidth, mHeight);
}

void Application::InitExtraSubsystems()
{
}

void Application::SceneEnd()
{
}