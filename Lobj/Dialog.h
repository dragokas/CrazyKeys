#pragma once
#include <windows.h>
#include "WndBase.h"
#include "Str.h"
#include "Queue.h"

class CDialog : public CWndBase {
public:
	static HWND GetLastCreatedDlg() { return ( dlgMapTable.GetCount() == 0 ) ? 0 : dlgMapTable.First().handle; };

protected:
	CDialog( HINSTANCE hInstance = 0 ) : CWndBase( hInstance ) {};

#pragma warning( disable : 4100 )//unreferenced formal parameter
	virtual LPARAM DefProc( UINT uMsg, WPARAM wParam, LPARAM lParam ) { return FALSE; }
	virtual LPARAM OnMsg( UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual BOOL OnProcDlg( UINT uMsg, WPARAM wParam, LPARAM lParam ) { return FALSE; }
	virtual BOOL OnInitDlg() { return FALSE; }
	virtual BOOL OnHelpDlg( HELPINFO* helpInfo ) { return FALSE; }
	virtual BOOL OnCommandDlg( WPARAM wParam, LPARAM lParam ) { return FALSE; }
	virtual BOOL OnNotifyDlg( WPARAM wParam, LPARAM lParam ) { return FALSE; }
#pragma warning( default : 4100 )//unreferenced formal parameter

	CString GetItemText( int itemID, int maxLength = 512 ) const;
	void SetItemText( int itemID, CString text ) const { SetDlgItemText( hDlg, itemID, text );};
	HWND GetItem( int itemID ) const { return GetDlgItem( hDlg, itemID ); }

	void CreateModalDlg( WORD dlgID, HWND hParentWnd = 0 );

private:
	static CList<CWndBase::CPair> dlgMapTable;

	friend INT_PTR CALLBACK DlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );
};

INT_PTR CALLBACK DlgProcPropSheetPage( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );

