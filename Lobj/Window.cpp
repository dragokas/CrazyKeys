#include "Window.h"

CList<CWindow::CPair> CWindow::wndMapTable;

#include "DebugLogTrace.h"

LRESULT CALLBACK WndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LogTraceWM( "!!!CRATE.txt", uMsg );
	int wndIndex = 0;
	CWindow* wnd = 0;
	if( uMsg == WM_CREATE ) {
		wnd = (CWindow*)(((CREATESTRUCT*)lParam)->lpCreateParams);
		wnd->BindWindowHandle( CWindow::wndMapTable, hwnd );
	} else {
		wndIndex = CWindow::wndMapTable.Find( hwnd );
		if( wndIndex == -1 ) {
			return DefWindowProc( hwnd, uMsg, wParam, lParam );
		}
        wnd = (CWindow*)CWindow::wndMapTable[wndIndex].window;
	}

    LRESULT lResult = wnd->OnMsg( uMsg, wParam, lParam );
	
	if( uMsg == WM_DESTROY ) {
		wnd->hWnd = 0;
		CWindow::wndMapTable.PopByIndex( wndIndex );
	}
	return lResult;
}

//////////////////////////////////////////////////////////////////////////
bool CWindow::RegisterWndClass( const WNDCLASS& w ) const
{
	return ( RegisterClass(&w) != 0 );
}

void CWindow::FillDefaultWndClassStruct( WNDCLASS& w, const CString& className ) const
{
	ZeroData( w );//��������
	//3 ������� ����������
	w.lpszClassName = className.GetBuffer();//��� ������ ��� ������������ �������
	w.hInstance = hInst;//��� ������ ������� ���������� ���������
	w.lpfnWndProc = WndProc;//����� ������� ��������� ���������
	//w.hbrBackground = ( HBRUSH ) GetStockObject( GRAY_BRUSH );
	w.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;//����� ����
	w.hIcon = LoadIcon( NULL, IDI_APPLICATION );//������ - ��������
	w.hCursor = LoadCursor( NULL, IDC_ARROW );//��������� - ��������
}

bool CWindow::RegisterWndClass( const CString& className ) const
{
	WNDCLASS w = {0};//����� ����
	FillDefaultWndClassStruct( w, className );
	return RegisterWndClass( w );
}

bool CWindow::CreateWnd( const CString& className, const CString& windowName, 
						DWORD dwStyle, RECT rect, HWND hParent, HMENU hMenu)
{
	CreateWindow( className.GetBuffer(), windowName.GetBuffer(), dwStyle,
		rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, hParent, hMenu, hInst, this );		
	return ( hWnd != 0 );
}

// CWindow::~CWindow()
// {
// 	
// }

//////////////////////////////////////////////////////////////////////////
void CWindow::InvalidateClientRect()
{
	RECT rect;
	GetClientRect( hWnd, &rect );
	InvalidateRect( hWnd, &rect , FALSE );
}


















