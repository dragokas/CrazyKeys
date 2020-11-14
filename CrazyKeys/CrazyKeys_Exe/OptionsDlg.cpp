#include "OptionsDlg.h"
#define _WIN32_WINNT 0x0501
#include <commctrl.h>
#include <windows.h>
#include "resource.h"
#include "resource.hm"
#include "CrazyKeysMUI.h"
#include "Path.h"
#include "CrazyKeysConst.h"

struct CSpin {
	short MinVal;
	short DefVal;
	short MaxVal;
};

const CSpin DelaySpin = { 50, DefaultDelayTime, 9999 };
const CSpin SleepSpin = { 5, DefaultSleepTime, 999 };
const CSpin TimerSpin = { 5, DefaultTimerTime, 999 };

void COptionsDlg::ShowDlg( HWND hParentWnd, UINT dlgMsg )
{
	hParent = hParentWnd;
	msgToParentWnd = dlgMsg;
	options.Load();

	LOGFONT lf = {0};
	ZeroData( lf );
	//шрифт для кнопок 
	wcscpy( lf.lfFaceName, L"Arial" );
	lf.lfHeight = 20;
	hKbKeyFont = CreateFontIndirect( &lf );
	//шрифт для смайлика у переключателя 
	wcscpy( lf.lfFaceName, L"Wingdings" );
	lf.lfHeight = 22;
	lf.lfCharSet = SYMBOL_CHARSET;
	hVKkeyFaceFont = CreateFontIndirect( &lf );

	CreateModalDlg( IDD_OPTIONS_DLG, 0 );
	
	DeleteObject( hVKkeyFaceFont );
	DeleteObject( hKbKeyFont );	
}

BOOL COptionsDlg::OnInitDlg()
{
	TranslateCrazyKeysDlg( hDlg );
	initRuleList();
	initKeyboards();
	SetDlgItemText( hDlg, IDC_STATIC_LANGUAGE_SEQ, options.GetLanguageSwitchSeqStr().GetBuffer() );
	SetDlgItemText( hDlg, IDC_STATIC_LAYOUT_SEQ, options.GetLayoutSwitchSeqStr().GetBuffer() );
	SetDlgItemText( hDlg, IDC_EDIT_FILENAME, options.GetFileName().GetBuffer() );
	initSwitchCombo();
	fillDlg();
	return TRUE;
}

void COptionsDlg::fillDlg() const
{
	fillRuleList();
	fillKeyboards();
	setSpinCtrls();
	fillSwitchKey();
	fillLanguageCombo();
	fillComboBoxes();
	SetDlgItemText( hDlg, IDC_EDIT_FILENAME, options.GetFileName().GetBuffer() );
	SetDlgItemText( hDlg, IDC_STATIC_VERSION, GetCrazyKeysVersionEx() );
	EnableWindow( GetDlgItem( hDlg, IDC_RUSSIAN_LL ), options.HasRussianLayout() ? FALSE : TRUE );
}

BOOL COptionsDlg::OnHelpDlg( HELPINFO* helpInfo )
{
	ShowHelpTopic( hDlg, ( helpInfo->dwContextId == 0 ) ? HIDHELP_OPTIONS_DLG : helpInfo->dwContextId );
	return TRUE;
}

BOOL COptionsDlg::OnCommandDlg( WPARAM wParam, LPARAM lParam )
{
	WORD param = LOWORD( wParam );
	if( ( param == IDC_COMBO_SOURCE || param == IDC_COMBO_TARGET ) && HIWORD( wParam ) == CBN_SELCHANGE ) {
		onChangeInLayoutCombo( (HWND)lParam, ( LOWORD( wParam ) == IDC_COMBO_SOURCE ) ? T_Source : T_Target );
		return TRUE;
	}
	if( param == IDC_COMBO_VK_NAMES && HIWORD( wParam ) == CBN_SELCHANGE ) {
		onChangeInVKNamesCombo( (HWND)lParam );
		return TRUE;
	}
	if( param == IDC_COMBO_LANGUAGE_MUI && HIWORD( wParam ) == CBN_SELCHANGE ) {
		onChangeInLanguageCombo( (HWND)lParam );
		return TRUE;
	}
	return onButton( wParam );
}

BOOL COptionsDlg::OnNotifyDlg( WPARAM , LPARAM lParam )
{
	NMHDR* pNMHDR = (NMHDR*)lParam;
	if( pNMHDR->idFrom == IDC_LIST_RULES ) {
		if( pNMHDR->code == LVN_COLUMNCLICK ) {
			NMLISTVIEW* pListView = (NMLISTVIEW*)lParam;
			options.GetRules().Sort( pListView->iSubItem == 0 ? CRules::ST_Source : CRules::ST_Target );
			fillRuleList();
			return TRUE;
		} else if( pNMHDR->code == NM_DBLCLK ) {
			onEditRule();
			return TRUE;
		}
	}
	return FALSE;
}

INT_PTR COptionsDlg::onButton( WPARAM wParam )
{
	switch( LOWORD( wParam ) ) {
		case IDC_ADD_RULE: onAddNewRule(); return TRUE;
		case IDC_EDIT_RULE: onEditRule(); return TRUE;
		case IDC_DELETE_RULE: onDeleteRule(); return TRUE;
		case IDC_DEFAULT_RULES: onLoadDefaultRules(); return TRUE;
		case IDC_REMOVE_RULES: onRemoveBadRules(); return TRUE;
		case IDC_LOAD: onLoadOptionsFile(); return TRUE;
		case IDC_SAVE: onSaveOptionsFile(); return TRUE;
		case IDC_SAVEAS: onSaveAsOptionsFile(); return TRUE;
		
		case IDC_RUSSIAN_LL: 
			onDialogLanguagesLayout(); return TRUE;
			//onAddRussianLanguageLayout(); return TRUE;
		
		case IDC_REFRESH_LL: onRefreshLanguagesLayout(); return TRUE;
		case IDC_DIALOG_LL: onDialogLanguagesLayout(); return TRUE;
		case IDC_OPTIMAL_LL: onOptimalLanguagesLayout(); return TRUE;
		
		case IDHELP_OPTIONS_DLG:
			ShowHelpTopic( hDlg, HIDHELP_OPTIONS_DLG );
			return TRUE;
		case IDC_QUICK_OPTIONS:
			ShowHelpTopic( hDlg, HIDC_QUICK_OPTIONS );
			return TRUE;
		case IDC_REPORT:
			showReport();
			return TRUE;
		case IDCANCEL:
			EndDialog( hDlg, LOWORD( wParam ) );
			return TRUE;
		case IDOK:
			if( onOk() ) {
				EndDialog( hDlg, LOWORD( wParam ) );
			}
			return TRUE;
	}
	return FALSE;
}

bool COptionsDlg::onOk()
{
	getSpinCtrls();
	options.SetBlockSwitchKey( IsDlgButtonChecked( hDlg, IDC_BLOCK_SWITCH_KEY ) == BST_CHECKED );
	if( options.AreOptionsGood( true ) ) {
		options.Save();
		return true;
	} else {
		fillRuleList();//в AreOptionsGood могли отредактироваться правила
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////

void COptionsDlg::onSaveOptionsFile( CString saveAsFileName /*= EmptyStr*/ )
{
	getSpinCtrls();
	options.SetBlockSwitchKey( IsDlgButtonChecked( hDlg, IDC_BLOCK_SWITCH_KEY ) == BST_CHECKED );
	options.AreOptionsGood( true );
	options.Save( saveAsFileName );
}

void COptionsDlg::fillFileNameStruct( OPENFILENAME& ofn, wchar_t* title, wchar_t* buffer, wchar_t* currentDirName ) const
{
	ZeroMemory( &ofn, sizeof(ofn) );
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hDlg;
	ofn.lpstrTitle = title;
	ofn.lpstrFilter = L"saves\0*.sav\0\0";
	ofn.lpstrFile = buffer;
	ofn.nMaxFile = 256; 
	ofn.lpstrInitialDir = currentDirName;
	ofn.Flags = OFN_NOCHANGEDIR | OFN_EXPLORER | OFN_LONGNAMES | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
}

void COptionsDlg::onSaveAsOptionsFile()
{
	wchar_t currentDirName[260];
	GetCurrentDirectory( 260, currentDirName );
	wchar_t fileNameBuffer[500];
	ZeroMemory( &fileNameBuffer, sizeof(fileNameBuffer) );

	OPENFILENAME ofn; 
	fillFileNameStruct( ofn, L"Save Option File", fileNameBuffer, currentDirName );

	CString fileName;
	if( GetSaveFileName( &ofn ) != 0 ) {
		fileName = fileNameBuffer + ofn.nFileOffset;
		if( fileName.Find( L".sav", max( 0 , fileName.Length() - 4 ) ) == -1 ) {
			fileName += L".sav";
		}
		onSaveOptionsFile( fileName );
		SetDlgItemText( hDlg, IDC_EDIT_FILENAME, options.GetFileName().GetBuffer() );
	}
}

void COptionsDlg::onLoadOptionsFile()
{
	wchar_t currentDirName[260];
	GetCurrentDirectory( 260, currentDirName );
	wchar_t fileNameBuffer[500];
	ZeroMemory( &fileNameBuffer, sizeof(fileNameBuffer) );

	OPENFILENAME ofn;
	fillFileNameStruct( ofn, L"Load Option File", fileNameBuffer, currentDirName );

	CString fileName;
	if( GetOpenFileName( &ofn ) != 0 ) {
		fileName = fileNameBuffer + ofn.nFileOffset;
		options.LoadAs( fileName );
		fillDlg();
	}
}

void COptionsDlg::setSpinCtrl( int spinID, const CSpin& spin, short value ) const
{
	SendMessage( GetDlgItem( hDlg, spinID ), UDM_SETRANGE, 0, (LPARAM) MAKELONG( spin.MaxVal, spin.MinVal ) );
	SendMessage( GetDlgItem( hDlg, spinID ), UDM_SETPOS, 0, (LPARAM) MAKELONG( value, 0 ) );
}

void COptionsDlg::setSpinCtrls() const
{
	setSpinCtrl( IDC_SPIN_DELAY, DelaySpin, options.GetDelayTime() );
	setSpinCtrl( IDC_SPIN_SLEEP, SleepSpin, options.GetSleepTime() );
	setSpinCtrl( IDC_SPIN_TIMER, TimerSpin, options.GetTimerTime() );
}

short COptionsDlg::getSpinCtrl( int spinID, const CSpin& spin ) const
{
	LRESULT spinValue = SendMessage( GetDlgItem( hDlg, spinID ), UDM_GETPOS, 0, 0 );
	return ( HIWORD( spinValue ) != 0 ) ? spin.DefVal : LOWORD( spinValue );
}

void COptionsDlg::getSpinCtrls()
{
	options.SetTimes( getSpinCtrl( IDC_SPIN_DELAY, DelaySpin ), getSpinCtrl( IDC_SPIN_SLEEP, SleepSpin ),
		getSpinCtrl( IDC_SPIN_TIMER, TimerSpin ) );
}

void COptionsDlg::fillLanguageCombo() const
{
	HWND hCombo = GetItem( IDC_COMBO_LANGUAGE_MUI );
	SendMessage( hCombo, CB_INSERTSTRING, WPARAM(-1), (LPARAM)(T("English")) ); 
	SendMessage( hCombo, CB_INSERTSTRING, WPARAM(-1), (LPARAM)(T("Русский")) ); 
	SendMessage( hCombo, CB_SETCURSEL, ( GetLanguageMUI() == MUI_Eng)  ? 0 : 1, 0 );
}

void COptionsDlg::onChangeInLanguageCombo( HWND hCombo )
{
	int selectedIndex = SendMessage( hCombo, CB_GETCURSEL, 0, 0 );
	if( selectedIndex != CB_ERR ) {
		SetLanguageMUI( ( selectedIndex == 0 ) ? MUI_Eng : MUI_Rus );
		TranslateCrazyKeysDlg( hDlg );
		SetDlgItemText( hDlg, IDC_STATIC_VERSION, GetCrazyKeysVersionEx() );
	}
}

void COptionsDlg::showReport() const
{
	ShellExecute( hDlg, L"edit", options.CreateReport(), 0, 0, SW_SHOW );
}
 











