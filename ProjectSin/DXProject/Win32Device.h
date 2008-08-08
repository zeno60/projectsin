#ifndef WIN32DEVICE_HEADER
#define WIN32DEVICE_HEADER

#include <windows.h>
#include <string>

class Win32Device
{
	int mWidth, mHeight;
	HWND mHWnd;
	std::wstring mName;
	bool mActive;

	static Win32Device* mInstance;
public:
	Win32Device(int w, int h, const TCHAR* name = L"Default Window Name");
	~Win32Device();
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void RegisterClass(WNDCLASSEX* pWc = NULL);
	virtual void InitInstance();

	HWND GetHWnd() { return mHWnd; }
	bool IsActive() { return mActive; }
	static Win32Device* GetInstance() { return mInstance; }
};

inline Win32Device* MainWindow()
{
	return Win32Device::GetInstance();
}

#endif