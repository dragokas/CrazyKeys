#include "Dialog.h"
#include "Queue.h"
#include "WinMsgName.h"

CList<CDialog::CPair> CDialog::dlgMapTable;

#include "DebugLogTrace.h"

INT_PTR CALLBACK DlgProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	int dlgIndex = 0;
	CDialog* dlg = 0;
	if( uMsg == WM_INITDIALOG ) {
		dlg = (CDialog*)lParam;
		dlg->BindWindowHandle( CDialog::dlgMapTable, hwnd );
	} else {
		dlgIndex = CDialog::dlgMapTable.Find( hwnd );
		if( dlgIndex == -1 ) {
			return FALSE;
		}
		dlg = (CDialog*)CDialog::dlgMapTable[dlgIndex].window;
	}
	LRESULT lResult = dlg->OnMsg( uMsg, wParam, lParam );

	if( uMsg == WM_DESTROY ) {
		dlg->hWnd = 0;
		CDialog::dlgMapTable.PopByIndex( dlgIndex );
	}
	return lResult;
}

INT_PTR CALLBACK DlgProcPropSheetPage( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( uMsg == WM_INITDIALOG ) {
		PROPSHEETPAGE* pPropSheetPage = (PROPSHEETPAGE*)lParam;
		return DlgProc( hwnd, uMsg, wParam, pPropSheetPage->lParam );
	} 
	return DlgProc( hwnd, uMsg, wParam, lParam );
}

LPARAM CDialog::OnMsg( UINT uMsg, WPARAM wParam, LPARAM lParam ) 
{
	if( OnProcDlg( uMsg, wParam, lParam ) == TRUE ) {
		return TRUE;
	}
	switch( uMsg ) {
		case WM_INITDIALOG: return OnInitDlg();
		case WM_COMMAND: return OnCommandDlg( wParam, lParam );
		case WM_NOTIFY: return OnNotifyDlg( wParam, lParam );
		case WM_HELP: return OnHelpDlg( (HELPINFO*)lParam );
	}
	return DefProc( uMsg, wParam, lParam );
}

//////////////////////////////////////////////////////////////////////////

void CDialog::CreateModalDlg( WORD dlgID, HWND hParentWnd )
{
	CheckError( DialogBoxParam( hInst, MAKEINTRESOURCE( dlgID ), hParentWnd, ::DlgProc, (LPARAM)this ), -1 );
}

CString CDialog::GetItemText( int itemID, int maxLength /*= 512*/ ) const
{
	CString tmpStr( maxLength );
	::GetDlgItemText( hDlg, itemID, tmpStr.GetRowBuffer(), maxLength );
	tmpStr.ValidateStrBuffer();
	return tmpStr;
}




