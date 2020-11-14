#include <stdio.h>
#include "Taskbar.h"
#include "resource.h"
#include "Presume.h"
#include "Str.h"
#include "MUI.h"
#include "CrazyKeysMUI.h"
#include "CrazyKeysConst.h"

static const UINT Taskbar_uID = 13;

CTaskbar::~CTaskbar()
{
	SetIconColor( HS_NoIcon );
}

bool CTaskbar::Init()
{
	char msgStr[100];
	_snprintf( msgStr, 100, "TransTrayMsg %s", __TIMESTAMP__ );
	taskbarMsg = RegisterWindowMessage( GetStrW( msgStr ) );
	CheckZero( taskbarMsg );

	currentIconColor = HS_NoIcon;
	hIcons[0] = LoadIcon( hInst, MAKEINTRESOURCE( IDI_ICON_GREEN ) );
	CheckZero( hIcons[0] ); 
	hIcons[1] = LoadIcon( hInst, MAKEINTRESOURCE( IDI_ICON_YELLOW ) );
	CheckZero( hIcons[1] ); 
	hIcons[2] = LoadIcon( hInst, MAKEINTRESOURCE( IDI_ICON_RED ) );
	CheckZero( hIcons[2] ); 

	return ( hIcons[0] != 0 && hIcons[1] != 0 && hIcons[2] != 0 && taskbarMsg != 0 );
}

void CTaskbar::fillNotifyIconData( NOTIFYICONDATA& tnid, THookState color )
{
	ZeroMemory( &tnid, sizeof( tnid ) );
	tnid.cbSize = sizeof( tnid );
	tnid.hWnd = hWnd;
	tnid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	tnid.uID = Taskbar_uID;
 	tnid.uCallbackMessage = taskbarMsg;
	if( color == HS_Green ) {
		lstrcpyn( tnid.szTip, L"Trans Hook is ON", 63 );
	} else if( color == HS_Yellow ) {
		lstrcpyn( tnid.szTip, L"Trans Hook is PAUSE (press switch key (default - ScrollLock) to begin)", 63 );
	} else {
		lstrcpyn( tnid.szTip, L"Trans Hook is OFF", 63 );
	}
	tnid.hIcon = hIcons[min( HS_Red, color )];
}

void CTaskbar::SetIconColor( THookState color )
{
	if( currentIconColor == color ) {
		return;//не надо ничего менять
	}
	NOTIFYICONDATA tnid; 
	fillNotifyIconData( tnid, color );
	if( currentIconColor == HS_NoIcon ) {
		BOOL resultShell_NotifyIconAdd = Shell_NotifyIcon( NIM_ADD, &tnid );
		presume( resultShell_NotifyIconAdd == TRUE );
	} else if( color != HS_NoIcon ) {
		if( Shell_NotifyIcon( NIM_MODIFY, &tnid ) == FALSE ) {
			BOOL resultShell_NotifyIconAddInModify = Shell_NotifyIcon( NIM_ADD, &tnid );
			presume( resultShell_NotifyIconAddInModify == TRUE );
		}
	} else {
		CheckError( Shell_NotifyIcon( NIM_DELETE, &tnid ), FALSE );
	}
	currentIconColor = color;
}

void CTaskbar::ShowMenu( THookState hookState )
{
	POINT pt;
	CheckZero( GetCursorPos( &pt ) );
	HMENU hMenu = LoadMenu( hInst, (LPCWSTR)IDR_MENU );
	CheckZero( hMenu );
	HMENU hSubMenu = GetSubMenu( hMenu, 0 );
	CheckZero( hSubMenu );
	CheckZero( SetForegroundWindow( hWnd ) );
	//серим тут не нужный пункт меню который сейчас выбран
	int menuLineID = ( hookState == HS_Off ) ? ID_TRANS_STOP : ( hookState == HS_Pause ) ? ID_TRANS_PAUSE : ID_TRANS_RUN;
	CheckError( EnableMenuItem( hSubMenu, menuLineID, MF_DISABLED | MF_GRAYED ), -1 );
	if( hookState == HS_Off ) {//из Off сразу запустить нельзя, т.к. длл не загружена
		CheckError( EnableMenuItem( hSubMenu, ID_TRANS_RUN, MF_DISABLED | MF_GRAYED ), -1 );
	}
	TranslateMenu( hSubMenu );
	CheckZero( TrackPopupMenu( hSubMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON,  pt.x, pt.y, 0, hWnd, 0 ) );
	CheckZero( PostMessage( hWnd, WM_NULL, 0, 0 ) );
}






