//Main длль-ки, внешние функции которые експортируютс€ дл€ екзешника и хука.
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
			//при подключении выставл€ем себе самый большой приоритет, чтобы не тормозило
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

////////////////////////////////////////////////////////////////////////// √лобальные переменные
VOID CALLBACK TimerProc( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime );
HHOOK hNextGlobalHook;//хук, который нужно передавать следующему по цепочке очереди хуков

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
	if( nCode != HC_ACTION ) {//это сообщение обрабатывать нельз€
		return CallNextHookEx( hNextGlobalHook, nCode, wParam, lParam );
	}
	PKBDLLHOOKSTRUCT hookStruct = (PKBDLLHOOKSTRUCT)lParam;

	if( hookStruct->dwExtraInfo == MY_KEY_MESSAGE ) {//свои собственные мне обрабатывать не надо
		hookStruct->dwExtraInfo = 0;//подмен€ем, чтобы никто не заметил, проверенно, что всегда шлЄтс€ 0
		return CallNextHookEx( hNextGlobalHook, nCode, wParam, lParam );
	}
	if( hookStruct->vkCode < 0 || 254 <= hookStruct->vkCode ) {//это противоречит MSDN
		return CallNextHookEx( hNextGlobalHook, nCode, wParam, lParam );
	}
	if( hookStruct->scanCode < 0 || 254 < hookStruct->scanCode ) {
		//иногда в европейских €зыках приходит VK_LCONTROL, с безумным scanCode.
		//это используетс€ дл€ написани€ в немецком €зыке символа [, нажатием AltGr + 8 в числах вверху
		return CallNextHookEx( hNextGlobalHook, nCode, wParam, lParam );
	}
	IsInHookProcBlocker = true;
	if( GetCDllHookManager().OnProcHook( hookStruct, wParam ) ) {
		IsInHookProcBlocker = false;
		return -1;//надо вернуть не ноль, если обработали и плоглотили
	}
	IsInHookProcBlocker = false;
	return CallNextHookEx( hNextGlobalHook, nCode, wParam, lParam);
}

LRESULT CALLBACK LowLevelMouseProcFunc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0) {//это сообщение обрабатывать нельз€
		return CallNextHookEx(hNextGlobalHook, nCode, wParam, lParam);
	}
	PMSLLHOOKSTRUCT hookStruct = (PMSLLHOOKSTRUCT)lParam;

	if (hookStruct->flags & LLMHF_INJECTED) {//нам не нужны эмулированные нажати€
		return CallNextHookEx(hNextGlobalHook, nCode, wParam, lParam);
	}
	if (GetCDllHookManager().OnProcMouseHook(hookStruct, wParam)) {
		return -1;//надо вернуть не ноль, если обработали и плоглотили
	}
	return CallNextHookEx(hNextGlobalHook, nCode, wParam, lParam);
}

VOID CALLBACK TimerProc( HWND,UINT uMsg, UINT_PTR, DWORD )
{
	if( !IsInHookProcBlocker && uMsg == WM_TIMER ) {
		GetCDllHookManager().OnTimer();
	}
}



