#include <stdio.h>
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include "ExeHookManager.h"

#include "Presume.h"
#include "Str.h"
#include "CrazyKeysConst.h"

C���HookManager::C���HookManager( HWND hWndMain ) 
: hWnd( hWndMain ), hModule( 0 ), hHook( 0 ), isDllLoaded( false ), isWndHookSet( false ), hookState( HS_Off )
{
	char msgStr[100];//����� ��������� ��� ���-��
	_snprintf( msgStr, 100, "CrazyKeysMsg %s", __TIMESTAMP__ );
	crazyKeysMsg = RegisterWindowMessage( GetStrW( msgStr ) );
	CheckZero( crazyKeysMsg );
}

C���HookManager::~C���HookManager()
{
	if( isWndHookSet ) {//���� ������� ����� �����
		removeWndHook();//����������� �
	}
	if( hookState != HS_Off ) {//���� ��� �� ��������
		fHookSetState( HS_Off, hHook, hWnd, crazyKeysMsg );//��������� ��� � ����.���
	}
	if( isDllLoaded ) {//���� �� ��������
		unloadDll();//���������
	}
}

bool C���HookManager::loadDll()
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

void C���HookManager::unloadDll()
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

bool C���HookManager::setWndHook()
{
	if( !isDllLoaded && !loadDll() ) {//���� �� ��������� � �� ����� ���� ���������
		return false;
	}
	//WH_KEYBOARD_LL �� ��� ��� ������ �������� ������, � ��� ����������� � ������, � �������� ������
	//WH_KEYBOARD ����������� �� WM_KEYUP/DOWN � ��������� � ������ ��� ��� �� ����������
	//������, ��-�� WH_KEYBOARD_LL ��������� � 98-� �� �������� :(
	hHook = SetWindowsHookEx( WH_KEYBOARD_LL, fHookProc, hModule, 0 );
	CheckZero( hHook );
	isWndHookSet = ( hHook != 0 ); 
	return isWndHookSet;
}

void C���HookManager::removeWndHook()
{
	if( isWndHookSet ) {
		CheckZero( UnhookWindowsHookEx( hHook ) );
		hHook = NULL;
		isWndHookSet = false;
	}
}

bool C���HookManager::SetHookState( THookState newState ) 
{
	if( newState == hookState ) {//���� ��������� ����� ��
		return true;
	}
	if( newState == HS_Off  ) {//���� ���� ��������� => ���� ��������
		presume( isDllLoaded );
		if( isDllLoaded ) {
			removeWndHook();//��������� ��� �������
			fHookSetState( HS_Off, hHook, hWnd, crazyKeysMsg );//��������� ��� � ����.���
		}
	} else if( hookState == HS_Off ) {//���� ���� ��������� => ���� �������� 
		if( !isDllLoaded && !loadDll() ) {//��������� ���, ���� ��� �� ���������
			return false;//�� ����������� ���
		}
		if( !fHookSetState( newState, hHook, hWnd, crazyKeysMsg ) ) {//������ ��� � ���������
			return false;//�� ������ ������� � off � ���-�� (�������� ����� �������)
		}
		if( !setWndHook() ) {//���� �� ������ ������� ��� �������
			fHookSetState( HS_Off, hHook, hWnd, crazyKeysMsg );//������ ��������� ��� �������
			return false;
		}
	} else {//����� ������������ ��������� ����� ����� � ��, ��� �� ������� ������ �� ������� 
		if( !fHookSetState( newState, hHook, hWnd, crazyKeysMsg ) ) {//���� ���-�� �� ������ ������������
			return false;
		}
	}
	hookState = newState;//���� ����� ���������
	return true;
}



