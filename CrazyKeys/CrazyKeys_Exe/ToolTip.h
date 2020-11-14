#pragma once
#include <windows.h>
#include <commctrl.h>

class CToolTip {
public:
	CToolTip() : hTipWnd( 0 ) {};
	
	void ShowToolTip( HWND  hWnd, wchar_t* tipText, DWORD style = 0 ) const;
	void RemoveToolTip() const;
	bool IsToolTip() const { return ( hTipWnd != 0 ); }

private:
	void fillTipInfo( HWND hWnd, wchar_t* tipText ) const;
	void enableMouseEvents( HWND hWnd ) const;

	mutable HWND hTipWnd;
	mutable TOOLINFO toolInfo;	
};
