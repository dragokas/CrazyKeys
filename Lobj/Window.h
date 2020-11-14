#pragma once
#include <windows.h>
#include "WndBase.h"
#include "Str.h"
#include "Queue.h"

class CWindow : public CWndBase {
public:
	CWindow( HINSTANCE hInstance = 0 ) : CWndBase( hInstance ) {};
	virtual ~CWindow() { if( hWnd != 0 ) DestroyWindow( hWnd ); };

	void FillDefaultWndClassStruct( WNDCLASS& w, const CString& className ) const;
	bool RegisterWndClass( const WNDCLASS& w ) const;
	bool RegisterWndClass( const CString& className ) const;
	bool CreateWnd( const CString& className, const CString& windowName, DWORD dwStyle, RECT rect, HWND hParent = 0, HMENU hMenu = 0 );

protected:

	void InvalidateClientRect();

	virtual LRESULT DefProc( UINT uMsg, WPARAM wParam, LPARAM lParam ) {
        return DefWindowProc( hWnd, uMsg, wParam, lParam ); 
	}
	virtual LRESULT OnMsg( UINT uMsg, WPARAM wParam, LPARAM lParam ) { 
		return DefProc( uMsg, wParam, lParam ); 
	}

private:
	static CList<CWndBase::CPair> wndMapTable;

	friend LRESULT CALLBACK WndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
};


LRESULT CALLBACK NewWndCtrlProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );