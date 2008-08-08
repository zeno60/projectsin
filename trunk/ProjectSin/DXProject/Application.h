#ifndef APPLICATION_H
#define APPLICATION_H

#include <windows.h>
#include <string>
#include "D3DDevice.h"

//Initiation of Win32Device class (Application can use the class file)
class Win32Device; 

class Application
{
public:
	//Window Title
	std::wstring mTitle;
	//Window Size
	int mWidth, mHeight, mBpp; 
	//Tells if window is active
	bool mActive;
	//D3d Flags (Debug/Release)
	UINT mD3DFlags;

	//Holds the instance of the application
	static Application* mInstance;

	//Initilize Primary window
	virtual void InitPrimaryWindow();
	//Initilize Graphics
	virtual void InitGraphics();
	//Initlize Extra Subsystems
	virtual void InitExtraSubsystems();

public:
	Application();
	virtual ~Application();

	//Initilize Init()
	virtual void Init();
	//Initilize Run()
	virtual void Run();

	//The method that is called every loop
	virtual void DoFrame(float timeDelta);
	//The method tells when active it's false
	virtual void DoIdleFrame(float timeDelta);

	//Scene Initilize
	virtual void SceneInit();
	//Scene End Intilize
	virtual void SceneEnd();

	void Pause() { mActive = false; }
	void UnPause() { mActive = true; }

	static Application* GetInstance() { return mInstance; }

	static void EndApplication()
	{
		delete mInstance;
		mInstance = NULL;
	}
};

inline Application* Instance()
{
	return Application::GetInstance();
}

HINSTANCE ApplicationInstance();

Application* CreateApplication();

#endif