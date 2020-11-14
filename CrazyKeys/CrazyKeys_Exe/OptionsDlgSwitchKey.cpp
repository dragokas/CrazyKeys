#include "OptionsDlg.h"
#include "resource.h"
#include "CrazyKeysConst.h"
#include "CrazyKeysMUI.h"

BOOL COptionsDlg::OnProcDlg(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/ )
{
	if( uMsg == WM_KEYDOWN ) {
		onKeyDownInCombo( (BYTE)wParam );
		return TRUE;
	}
	return FALSE;
}

static WNDPROC OldComboProc = 0;

LRESULT APIENTRY NewComboProc( HWND hwnd,  UINT uMsg, WPARAM wParam, LPARAM lParam) 
{ 
	if( uMsg == WM_GETDLGCODE ) {
		return ( GetKeyState( VK_TAB ) == 0 ) ? DLGC_WANTALLKEYS :
			CallWindowProc( OldComboProc, hwnd, uMsg, wParam, lParam );
	}
	if( uMsg == WM_CHAR ) {
		return 0;
	}
	if( ( uMsg == WM_KEYDOWN || uMsg == WM_KEYUP ) &&
        wParam != VK_TAB && wParam != VK_ESCAPE && wParam != VK_RETURN &&
		wParam != VK_UP && wParam != VK_DOWN && 
		wParam != VK_PRIOR && wParam != VK_NEXT ) 
	{
		if( uMsg == WM_KEYDOWN ) {
			SendMessage( GetParent( hwnd ), WM_KEYDOWN, wParam, lParam );
		}
		return 0;
	}
	return CallWindowProc( OldComboProc, hwnd, uMsg, wParam, lParam ); 
} 

void COptionsDlg::initSwitchCombo() const
{
	HWND hCombo = GetItem( IDC_COMBO_VK_NAMES );
	OldComboProc = (WNDPROC)SetWindowLongPtr( hCombo, GWL_WNDPROC, (LONG_PTR)NewComboProc );
}

void COptionsDlg::fillSwitchKey() const
{
	SendMessage( GetItem( IDC_STATIC_VK_FACE ), WM_SETFONT, (WPARAM)hVKkeyFaceFont, (LPARAM)FALSE );
	
	HWND hCombo = GetItem( IDC_COMBO_VK_NAMES );
	for( short vk = 0; vk < 255; vk++ ) {
		CString comboStr = ( vk < 16 ) ? L"0x0" + Int2Str( vk, 16 ) : L"0x" + Int2Str( vk, 16 );
		comboStr += L"   ";
		comboStr += VKeyNames[vk];
		SendMessage( hCombo, CB_INSERTSTRING, WPARAM(-1), (LPARAM)comboStr.GetBuffer() ); 
	}
	SendMessage( hCombo, CB_SETCURSEL, options.GetSwitchKey(), 0 );
	setVKeyFace( options.GetSwitchKey() );
	CheckDlgButton( hDlg, IDC_BLOCK_SWITCH_KEY, options.GetBlockSwitchKey() ? BST_CHECKED : BST_UNCHECKED );
}

void COptionsDlg::onChangeInVKNamesCombo( HWND hCombo )
{
	BYTE vk = (BYTE)SendMessage( hCombo, CB_GETCURSEL, 0, 0 );
	if( vk != CB_ERR ) {
		options.SetSwitchKey( vk );
		setVKeyFace( vk );
	}
}

void COptionsDlg::onKeyDownInCombo( BYTE vk )
{
	options.SetSwitchKey( vk );
	setVKeyFace( vk );
	SendMessage( GetItem( IDC_COMBO_VK_NAMES ), CB_SETCURSEL, vk, 0 );
}

void COptionsDlg::setVKeyFace( BYTE vk ) const
{
	wchar_t vkFace[3][3] = { { 0x4A, 0 }, { 0x4B, 0 }, { 0x4C, 0 } };
	SetItemText( IDC_STATIC_VK_FACE, vkFace[VKeyGoodness[vk]] );
	ShowWindow( GetItem( IDC_STATIC_VK_COMMENT ), ( VKeyGoodness[vk] == 2 ) ? SW_SHOW : SW_HIDE ); 
}








