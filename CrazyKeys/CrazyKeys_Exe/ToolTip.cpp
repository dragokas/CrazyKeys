#include "ToolTip.h"
#include "presume.h"
/*
CToolTip::CToolTip() : hTipWnd( 0 ) 
{
	INITCOMMONCONTROLSEX iccex;
	iccex.dwSize = sizeof( INITCOMMONCONTROLSEX );
	iccex.dwICC = ICC_WIN95_CLASSES;
	BOOL res = InitCommonControlsEx( &iccex );
	presume( res == TRUE );
}*/

void CToolTip::ShowToolTip( HWND hWnd, wchar_t* tipText, DWORD style /*= 0*/ ) const
{
	enableMouseEvents( hWnd );
	hTipWnd = CreateWindowEx( WS_EX_TOPMOST, TOOLTIPS_CLASS, 0, WS_POPUP | style, 
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hWnd, 0, 0, 0 );
	fillTipInfo( hWnd, tipText );
	SendMessage( hTipWnd, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &toolInfo );
	SendMessage( hTipWnd, TTM_ACTIVATE, (WPARAM) TRUE, 0 );
}

void CToolTip::RemoveToolTip() const
{
	if( hTipWnd != 0 ) {
		SendMessage( hTipWnd, TTM_DELTOOL, 0, (LPARAM)(LPTOOLINFO) &toolInfo );
		DestroyWindow( hTipWnd );
	}
	hTipWnd = 0;
}

void CToolTip::fillTipInfo( HWND hWnd, wchar_t* tipText ) const
{
	ZeroMemory( &toolInfo, sizeof( TOOLINFO ) );
	toolInfo.cbSize = sizeof( TOOLINFO );
	toolInfo.uFlags = TTF_CENTERTIP | TTF_IDISHWND | TTF_SUBCLASS;
	toolInfo.hwnd = hWnd;
	toolInfo.uId = (UINT) hWnd;
	toolInfo.lpszText = tipText;
}

void CToolTip::enableMouseEvents( HWND hWnd ) const
{
	TRACKMOUSEEVENT tme = {0};
	ZeroMemory( &tme, sizeof( TRACKMOUSEEVENT ) );
	tme.cbSize = sizeof( TRACKMOUSEEVENT );
	tme.dwFlags = TME_LEAVE;
	tme.hwndTrack = hWnd;
	_TrackMouseEvent( &tme );
}