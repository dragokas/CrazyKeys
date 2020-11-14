#include <stdio.h>
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include "ExeHookManager.h"

#include "Presume.h"
#include "Str.h"
#include "CrazyKeysConst.h"

CЕхеHookManager::CЕхеHookManager( HWND hWndMain ) 
: hWnd( hWndMain ), hModule( 0 ), hHook( 0 ), isDllLoaded( false ), isWndHookSet( false ), hookState( HS_Off )
{
	char msgStr[100];//регим сообщение для длл-ки
	_snprintf( msgStr, 100, "CrazyKeysMsg %s", __TIMESTAMP__ );
	crazyKeysMsg = RegisterWindowMessage( GetStrW( msgStr ) );
	CheckZero( crazyKeysMsg );
}

CЕхеHookManager::~CЕхеHookManager()
{
	if( isWndHookSet ) {//если функция ловит буквы
		removeWndHook();//отсоединяем её
	}
	if( hookState != HS_Off ) {//если длл не выключен
		fHookSetState( HS_Off, hHook, hWnd, crazyKeysMsg );//переводим длл в сост.офф
	}
	if( isDllLoaded ) {//если не выгружен
		unloadDll();//выгружаем
	}
}

bool CЕхеHookManager::loadDll()
{
	presume( !isDllLoaded );
	if( isDllLoaded ) {
		return true;
	}
	hModule = LoadLibrary( HookDllFileName );
	CheckZero( hModule );
	if( hModule == 0 ) {
		return false;
	}
	fHookSetState = ( TDF_HookSetState )GetProcAddress( hModule, "SetHookState" );
	CheckZero( fHookSetState );
	fHookProc = ( TDF_HookProcFunc )GetProcAddress( hModule, "LowLevelKeyboardProcFunc" );
	CheckZero( fHookProc );
	if( fHookSetState == 0 || fHookProc == 0 ) {
		CheckZero( FreeLibrary( hModule ) );
		return false;
	}
	isDllLoaded = true;
	return true;
}

void CЕхеHookManager::unloadDll()
{
	presume( isDllLoaded );
	if( !isDllLoaded ) {
		return;
	}
	CheckZero( FreeLibrary( hModule ) );
	hModule = 0;
	fHookSetState = 0;
	fHookProc = 0;
	isDllLoaded = false;
}

bool CЕхеHookManager::setWndHook()
{
	if( !isDllLoaded && !loadDll() ) {//если не загружена и не может быть загружена
		return false;
	}
	//WH_KEYBOARD_LL он шлёт как только нажалась кнопка, и даёт возможность её съесть, и отсылать другое
	//WH_KEYBOARD запускается на WM_KEYUP/DOWN и переслать и понять что шлём мы невозможно
	//однако, из-за WH_KEYBOARD_LL программа в 98-м не работает :(
	hHook = SetWindowsHookEx( WH_KEYBOARD_LL, fHookProc, hModule, 0 );
	CheckZero( hHook );
	isWndHookSet = ( hHook != 0 ); 
	return isWndHookSet;
}

void CЕхеHookManager::removeWndHook()
{
	if( isWndHookSet ) {
		CheckZero( UnhookWindowsHookEx( hHook ) );
		hHook = NULL;
		isWndHookSet = false;
	}
}

bool CЕхеHookManager::SetHookState( THookState newState ) 
{
	if( newState == hookState ) {//если состояние такое же
		return true;
	}
	if( newState == HS_Off  ) {//если надо выключать => были включены
		presume( isDllLoaded );
		if( isDllLoaded ) {
			removeWndHook();//отпускаем хук функцию
			fHookSetState( HS_Off, hHook, hWnd, crazyKeysMsg );//переводим длл в сост.офф
		}
	} else if( hookState == HS_Off ) {//если были выключены => надо включить 
		if( !isDllLoaded && !loadDll() ) {//проверяем длл, если нет то загружаем
			return false;//не загрузилась длл
		}
		if( !fHookSetState( newState, hHook, hWnd, crazyKeysMsg ) ) {//ставим длл в состояние
			return false;//не смогли перейти с off в длл-ки (например опции подвели)
		}
		if( !setWndHook() ) {//если не смогли завести хук функцию
			fHookSetState( HS_Off, hHook, hWnd, crazyKeysMsg );//меняет состояние длл обратно
			return false;
		}
	} else {//иначе переключение состояния между пауза и он, что от данного класса не зависит 
		if( !fHookSetState( newState, hHook, hWnd, crazyKeysMsg ) ) {//если длл-ка не смогла переключится
			return false;
		}
	}
	hookState = newState;//наше новое состояние
	return true;
}



