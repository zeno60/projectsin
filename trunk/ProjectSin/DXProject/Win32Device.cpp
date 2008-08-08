#include "Win32Device.h"
#include "D3DDevice.h"
#include "Application.h"
#include "IEventHandler.h"

using namespace std;

Win32Device* Win32Device::mInstance = NULL;

LRESULT CALLBACK GlobalWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return MainWindow()->WndProc( hWnd, uMsg, wParam, lParam );
}

Win32Device::Win32Device(int w, int h, const TCHAR *name):mWidth(w),mHeight(h),mHWnd(NULL)
{
	mInstance = this;
	mActive = false;

	mName = wstring(name);
}

Win32Device::~Win32Device()
{
	::DestroyWindow(mHWnd);
	mInstance = NULL;
}

LRESULT Win32Device::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Event event;
	
	switch( msg )
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if(wParam == VK_ESCAPE) ::DestroyWindow(hWnd);
		else
		{
			event.Type = E_EVENT_TYPE::E_KEYPRESS;
			event.KeyInput.KeyCode = (KEY_CODE)wParam;
			event.KeyInput.PressedDown = true;
			EventDispatcher::Get()->SendEvent(event);
		}
		break;
	case WM_KEYUP:
		event.Type = E_EVENT_TYPE::E_KEYPRESS;
		event.KeyInput.KeyCode = (KEY_CODE)wParam;
		event.KeyInput.PressedDown = false;
		EventDispatcher::Get()->SendEvent(event);
		break;
	case WM_LBUTTONDOWN:
		event.Type = E_EVENT_TYPE::E_MOUSEPRESS;
		event.MouseInput.X = LOWORD(lParam);
		event.MouseInput.Y = HIWORD(lParam);
		event.MouseInput.Type = MOUSE_INPUT_TYPE::LMOUSE_PRESS;
		EventDispatcher::Get()->SendEvent(event);
		break;
	case WM_LBUTTONUP:
		event.Type = E_EVENT_TYPE::E_MOUSEPRESS;
		event.MouseInput.X = LOWORD(lParam);
		event.MouseInput.Y = HIWORD(lParam);
		event.MouseInput.Type = MOUSE_INPUT_TYPE::LMOUSE_RELEASE;
		EventDispatcher::Get()->SendEvent(event);
		break;
	case WM_MOUSEMOVE:
		event.Type = E_EVENT_TYPE::E_MOUSEMOVE;
		event.MouseInput.X = LOWORD(lParam);
		event.MouseInput.Y = HIWORD(lParam);
		EventDispatcher::Get()->SendEvent(event);
		break;
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

void Win32Device::RegisterClass(WNDCLASSEX* pWc)
{
	WNDCLASSEX wcex;
	if(pWc) wcex = *pWc;
	else
	{
		wcex.cbSize = sizeof(WNDCLASSEX); 

		wcex.style			= CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc	= GlobalWndProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= ApplicationInstance();
		wcex.hIcon			= 0;
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszMenuName	= 0;
		wcex.lpszClassName	= mName.c_str();
		wcex.hIconSm		= 0;
	}

	if(!RegisterClassEx(&wcex))
	{
		::MessageBox(0, L"RegisterClassEx() - Failed", 0, 0);
	}
}

void Win32Device::InitInstance()
{
	HINSTANCE hi = ApplicationInstance();

	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = mWidth;
	rc.bottom = mHeight;
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );

	mHWnd = CreateWindow( 
	   mName.c_str(), 
	   mName.c_str(), 
	   WS_OVERLAPPEDWINDOW | WS_VISIBLE,
	   0, 
	   0, 
	   rc.right - rc.left, 
	   rc.bottom - rc.top, 
	   NULL, 
	   NULL, 
	   ApplicationInstance(), 
	   NULL);

	if(!mHWnd)
	{
		::MessageBox(0, L"CreateWindow() - Failed", 0, 0);
		return;
	}

	// Draw
	ShowWindow(mHWnd, SW_SHOW);
	UpdateWindow(mHWnd);
}