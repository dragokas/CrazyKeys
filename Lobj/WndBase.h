#pragma once
#include <windows.h>
#include "Str.h"
#include "Queue.h"

class CWndBase {
protected:

	CWndBase( HINSTANCE hInstance ) : hWnd( 0 ) { hInst = ( hInstance != 0 ) ? hInstance : GetModuleHandle( 0 ); }

	virtual LPARAM DefProc( UINT uMsg, WPARAM wParam, LPARAM lParam ) = 0;
	virtual LPARAM OnMsg( UINT uMsg, WPARAM wParam, LPARAM lParam ) = 0;

	struct CPair {
		CPair( CWndBase* wnd, HWND hwnd ) : window( wnd ), handle( hwnd ) {} 
		CWndBase* window;
		HWND handle;
		bool operator==( HWND other ) const { return ( other == handle ); }
	};
	
	void BindWindowHandle( CList<CPair>& mapTable, HWND _hwnd ) {
		hWnd = hDlg = _hwnd;
		mapTable.PushToHead( CWndBase::CPair( this, hWnd ) );
	}

	HINSTANCE hInst;
	union {
		HWND hWnd;
		HWND hDlg;
	};
};