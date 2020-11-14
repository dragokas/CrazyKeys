#pragma once
#include <windows.h>

class C≈хеHookManager;
class CTaskbar;
enum THookState;

class CCrazyKeysMain {
public:
	CCrazyKeysMain( HINSTANCE hInstance );
	~CCrazyKeysMain();

	bool Init();

private:
	void createMainWindow();
	
	void onMenuWM_Command( WPARAM wParam );
	void showOptions();
	
	bool setState( THookState state );

	bool createFilesFromResources( HINSTANCE hInst );//вынуть из exe все нужные файлы
	bool createFileFromResources( HINSTANCE hInst, const wchar_t* resourceName, const wchar_t* fileName );
	
	HINSTANCE hInst;
	HWND hWnd;

	C≈хеHookManager *hookManager;
	CTaskbar* taskbar;

	HANDLE hMutex;

	bool isInOptionsDialog;

	LRESULT mainWndProc( HWND hWnd, UINT imsg, WPARAM wParam, LPARAM lParam );
	friend LRESULT CALLBACK MainWndProc( HWND hWnd, UINT imsg, WPARAM wParam, LPARAM lParam );
};
