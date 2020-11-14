#include "AddNewRuleDlg.h"
#include "resource.h"
#include "CrazyKeysMUI.h"

CAddNewRuleDlg::CAddNewRuleDlg( HINSTANCE hInstance )
: CDialog( hInstance ), sourceHKL( 0 ) {}

void CAddNewRuleDlg::ShowDlg( HWND hParentDlg, HKL sourceKbHKL, HKL targetKbHKL )
{
	sourceHKL = sourceKbHKL;
	targetHKL = targetKbHKL;
	CreateModalDlg( IDD_ADD_NEW_RULE_DLG, hParentDlg );
}

void CAddNewRuleDlg::ShowDlg( HWND hParentDlg, CString sourceStr, HKL targetKbHKL )
{
	source = sourceStr;
	targetHKL = targetKbHKL;
	CreateModalDlg( IDD_ADD_NEW_RULE_DLG, hParentDlg );
}

BOOL CAddNewRuleDlg::OnInitDlg()
{
	TranslateCrazyKeysDlg( hDlg );
	savedHKL = GetKeyboardLayout( 0 );
	SetFocus( GetDlgItem( hDlg, source.IsEmpty() ? IDC_EDIT_SOURCE : IDC_EDIT_TARGET ) );
	if( !source.IsEmpty() ) {
		SetItemText( IDC_EDIT_SOURCE, source );
		EnableWindow( GetItem( IDC_EDIT_SOURCE ), FALSE );
	}
	return FALSE;
}

BOOL CAddNewRuleDlg::OnCommandDlg( WPARAM wParam, LPARAM /*lParam*/ )
{
	switch( LOWORD( wParam ) ) {
		case IDC_EDIT_SOURCE:
			if( HIWORD( wParam ) == EN_SETFOCUS && sourceHKL != 0 ) {
				ActivateKeyboardLayout( sourceHKL, 0 );
			}
            return TRUE;
		case IDC_EDIT_TARGET: 
            if( HIWORD( wParam ) == EN_SETFOCUS && targetHKL != 0 ) {
				ActivateKeyboardLayout( targetHKL, 0 );
			}
            return TRUE;
		case IDOK:
			source = GetItemText( IDC_EDIT_SOURCE );
			source.RemoveAll( L" " );
			target = GetItemText( IDC_EDIT_TARGET );
			target.RemoveAll( L" " );
		case IDCANCEL:
			ActivateKeyboardLayout( savedHKL, 0 );
			EndDialog( hDlg, LOWORD( wParam ) );
			return TRUE;
	}
	return FALSE;
}
