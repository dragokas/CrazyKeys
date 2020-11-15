//Main ����-��, ������� ������� ������� �������������� ��� ��������� � ����.
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <crtdbg.h>
#include "ToExport_dll.h"
#include "DllHookManager.h"
#include "CrazyKeysConst.h"

inline CDllHookManager& GetCDllHookManager()
{
	static CDllHookManager dllHookManager;
	return dllHookManager;
}

BOOL APIENTRY DllMain( HINSTANCE, DWORD fdwReason, LPVOID )
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	switch( fdwReason ) {
		case DLL_PROCESS_ATTACH: 
		{
			GetCDllHookManager();//
			//��� ����������� ���������� ���� ����� ������� ���������, ����� �� ���������
			CheckZero( SetPriorityClass( GetCurrentProcess(), REALTIME_PRIORITY_CLASS ) );
			CheckZero( SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_HIGHEST ) );		
		}
		break;
		case DLL_THREAD_ATTACH: break;
		case DLL_THREAD_DETACH: break;
		case DLL_PROCESS_DETACH: break;
	}
	return TRUE;
}

////////////////////////////////////////////////////////////////////////// ���������� ����������
VOID CALLBACK TimerProc( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime );
HHOOK hNextGlobalHook;//���, ������� ����� ���������� ���������� �� ������� ������� �����

//////////////////////////////////////////////////////////////////////////

bool SetHookState( THookState state, HHOOK hNextHook, HWND hWnd, UINT crazyKeysMsg )
{
	if( state == HS_On || state == HS_Pause ) {
		hNextGlobalHook = hNextHook;
	}
	return GetCDllHookManager().SetHookState( state, hWnd, crazyKeysMsg );
}

volatile bool IsInHookProcBlocker = false;

#include "DebugLogTrace.h"

LRESULT CALLBACK LowLevelKeyboardProcFunc( int nCode, WPARAM wParam, LPARAM lParam )
{
	if( nCode != HC_ACTION ) {//��� ��������� ������������ ������
		return CallNextHookEx( hNextGlobalHook, nCode, wParam, lParam );
	}
	PKBDLLHOOKSTRUCT hookStruct = (PKBDLLHOOKSTRUCT)lParam;

	if( hookStruct->dwExtraInfo == MY_KEY_MESSAGE ) {//���� ����������� ��� ������������ �� ����
		hookStruct->dwExtraInfo = 0;//���������, ����� ����� �� �������, ����������, ��� ������ ����� 0
		return CallNextHookEx( hNextGlobalHook, nCode, wParam, lParam );
	}
	if( hookStruct->vkCode < 0 || 254 <= hookStruct->vkCode ) {//��� ������������ MSDN
		return CallNextHookEx( hNextGlobalHook, nCode, wParam, lParam );
	}
	if( hookStruct->scanCode < 0 || 254 < hookStruct->scanCode ) {
		//������ � ����������� ������ �������� VK_LCONTROL, � �������� scanCode.
		//��� ������������ ��� ��������� � �������� ����� ������� [, �������� AltGr + 8 � ������ ������
		return CallNextHookEx( hNextGlobalHook, nCode, wParam, lParam );
	}
	IsInHookProcBlocker = true;
	if( GetCDllHookManager().OnProcHook( hookStruct, wParam ) ) {
		IsInHookProcBlocker = false;
		return -1;//���� ������� �� ����, ���� ���������� � ����������
	}
	IsInHookProcBlocker = false;
	return CallNextHookEx( hNextGlobalHook, nCode, wParam, lParam);
}

LRESULT CALLBACK LowLevelMouseProcFunc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0) {//��� ��������� ������������ ������
		return CallNextHookEx(hNextGlobalHook, nCode, wParam, lParam);
	}
	PMSLLHOOKSTRUCT hookStruct = (PMSLLHOOKSTRUCT)lParam;

	if (hookStruct->flags & LLMHF_INJECTED) {//��� �� ����� ������������� �������
		return CallNextHookEx(hNextGlobalHook, nCode, wParam, lParam);
	}
	if (GetCDllHookManager().OnProcMouseHook(hookStruct, wParam)) {
		return -1;//���� ������� �� ����, ���� ���������� � ����������
	}
	return CallNextHookEx(hNextGlobalHook, nCode, wParam, lParam);
}

VOID CALLBACK TimerProc( HWND,UINT uMsg, UINT_PTR, DWORD )
{
	if( !IsInHookProcBlocker && uMsg == WM_TIMER ) {
		GetCDllHookManager().OnTimer();
	}
}



