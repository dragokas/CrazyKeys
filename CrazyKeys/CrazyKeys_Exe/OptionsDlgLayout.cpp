#include "OptionsDlg.h"
#include "resource.h"
#include "MsgBox.h"
#include "CrazyKeysConst.h"

////////////////// ������ ////////////////////////////////////////////////////////

void COptionsDlg::fillButtonsText( int index, TLayouts which ) const
{
	const CVector<wchar_t>& letters = options.GetLayout( which )->GetScanCodeToWcharMap();
	for( int i = 0; i < 4; i++ ) {
		for( int j = 0; scanCodes[i][j] != 0; j++, index++ ) {
			wchar_t letter[2] = { letters[scanCodes[i][j]] == 0 ? L' ' : letters[scanCodes[i][j]], 0 };
			SendMessage( hWndButtons[index], WM_SETTEXT, 0, (LPARAM)letter );
		}
	}
}

void COptionsDlg::fillKeyboards() const
{
	fillButtonsText( 0, T_Source );
	fillButtonsText( nButtons/2, T_Target );
}

void COptionsDlg::initKeyboards()
{
	nButtons = 0;
	initKeyboard( IDC_STATIC_SOURCE );
	initKeyboard( IDC_STATIC_TARGET );
}

void COptionsDlg::calculateKbParams( int dlgStaticId, int& size, int& xOffset, int& yOffset ) const
{
	RECT rect;
	GetWindowRect( GetDlgItem( hDlg, dlgStaticId ), &rect );
	POINT pt1 = { rect.left, rect.top };
	POINT pt2 = { rect.right, rect.bottom };
	ScreenToClient( hDlg, &pt1 );
	ScreenToClient( hDlg, &pt2 );
	RECT kbRect = { pt1.x, pt1.y, pt2.x, pt2.y };
	size = min( ( kbRect.right - kbRect.left ) / 16 , ( kbRect.bottom - kbRect.top ) / 5 );
	xOffset = kbRect.left + (size / 2);
	yOffset = kbRect.top + (size / 2);
}

void COptionsDlg::initKeyboard( int dlgStaticId )
{
	int xOffset = 0, yOffset = 0;
	int size = 0;
	calculateKbParams( dlgStaticId, size, xOffset, yOffset );
	DWORD style = WS_CHILD | WS_VISIBLE | BS_CENTER | BS_VCENTER | BS_TEXT | BS_NOTIFY;
	int xOffsets[] = { xOffset, xOffset + size + ( size / 2 ), xOffset + 2 * size, xOffset + 2 * size + ( size / 2 ) };
	for( int i = 0, y = yOffset; i < 4; i++, y += size ) {
		int x = xOffsets[i];
		for( int j = 0; scanCodes[i][j] != 0; j++, x += size ) {
			hWndButtons[nButtons] = CreateWindow( L"BUTTON", L"", style, x, y, size, size, hDlg, 0, hInst, 0 );
			SendMessage( hWndButtons[nButtons], WM_SETFONT, (WPARAM)hKbKeyFont, (LPARAM)FALSE );
			nButtons++;
		}
	}	
}

////////////////////////////////////////////////////////////////////////// �������

void COptionsDlg::fillComboBox( HWND hCombo, TLayouts which ) const
{
	SendMessage( hCombo, CB_RESETCONTENT, 0, 0 );
	//const CList<CLayout*>& layouts = options.GetLayouts();
	const CPtrList<CLayout>& layouts = options.GetLayouts();
	for( int i = 0; i < layouts.GetCount(); i++ ) {
		if( which == T_Target || !layouts[i]->IsUnicode() ) {
			SendMessage( hCombo, CB_INSERTSTRING, WPARAM(-1), (LPARAM) layouts[i]->GetNameWithSeq().GetBuffer() ); 
		}	
	}
	SendMessage( hCombo, CB_SETCURSEL, options.GetLayoutIndex( which ), 0 );
}

void COptionsDlg::fillComboBoxes() const
{
	fillComboBox( GetItem( IDC_COMBO_SOURCE ), T_Source );
	fillComboBox( GetItem( IDC_COMBO_TARGET ), T_Target );
}

void COptionsDlg::onChangeInLayoutCombo( HWND hCombo, TLayouts which )
{
	int index = SendMessage( hCombo, CB_GETCURSEL, 0, 0 );
	if( index != CB_ERR ) {
		options.SetLayout( index, which );
		fillButtonsText( ( which == T_Source ) ? 0 : nButtons/2, which );
	}
}

void COptionsDlg::onAddRussianLanguageLayout()
{
	HKL savedHKL = LoadKeyboardLayout( L"00000419", 0 );
/*	if( savedHKL != 0 ) {
		//onDialogLanguagesLayout();
		onRefreshLanguagesLayout();		
		applyWindowsOptions();
		options.SetLayout( options.GetLayouts().GetCount() - 1, T_Target );
		fillComboBox( GetDlgItem( hDlg, IDC_COMBO_TARGET ), T_Target );
		fillButtonsText( nButtons/2, T_Target );
	}
	*/
	ActivateKeyboardLayout( savedHKL, 0 );
}

void COptionsDlg::onRefreshLanguagesLayout()
{
	options.UpdateLayouts();
	fillKeyboards();
	fillComboBoxes();
	SetDlgItemText( hDlg, IDC_STATIC_LANGUAGE_SEQ, options.GetLanguageSwitchSeqStr().GetBuffer() );
	SetDlgItemText( hDlg, IDC_STATIC_LAYOUT_SEQ, options.GetLayoutSwitchSeqStr().GetBuffer() );
	EnableWindow( GetDlgItem( hDlg, IDC_RUSSIAN_LL ), options.HasRussianLayout() ? FALSE : TRUE );
}

void COptionsDlg::onDialogLanguagesLayout()
{
	WinExec( "rundll32.exe MSCTF.dll, TF_RunInputCPL", SW_SHOW );
}

void COptionsDlg::onOptimalLanguagesLayout()
{
	options.SetOptimalLayoutSequences();//������� ��������� � ������
	applyWindowsOptions();
	onRefreshLanguagesLayout();//���������
}

bool COptionsDlg::applyWindowsOptions() const
{//����� ��������� �� ������� �������� � ����, ������� ������ � ����� ��� ������� ��
	for( int i = 0; i < 3; i++ ) {//������� ��� ������ 3 ����
		HWND hCurrentWnd = GetForegroundWindow();//������� ����
		WinExec( "rundll32.exe MSCTF.dll, TF_RunInputCPL", SW_SHOW );
		HWND hLanguageWnd = GetForegroundWindow();//���� �������, �������
		int delay = 10;//�������� �� ���
		int dialogDelay = 5000;//����� ������� ����� ��������� ������� ������
		for( int j = 0; j < dialogDelay && hLanguageWnd == hCurrentWnd; j += delay ) {
			Sleep( delay );//���
			hLanguageWnd = GetForegroundWindow();//���� �������, �������
		}
		wchar_t className[30];//��� ������ ����, ��� ������������ ��� ��� ����� ������� ����������
		if( hLanguageWnd != hCurrentWnd && //���� �����
			0 != GetClassName( hLanguageWnd, className, 30 ) &&//���� ��� ������ � ����
			CString(L"#32770") == className )//��� ��� ������ ��������� � ���������� ��� ������� ����
		{//�� ������ ��������, � �� ����� ����� �� ����� "#32770", ���� ����� � ������� �����
			int dlgButtonDelay = 2000;//������� ����� ������ ������ ��
			HWND hOkButton = FindWindowEx( hLanguageWnd, 0, L"Button", L"OK" );
			for( int j = 0; j < dlgButtonDelay && hOkButton == 0; j += delay ) {
				Sleep( delay );//���
				hOkButton = FindWindowEx( hLanguageWnd, 0, L"Button", L"OK" );
			}
			if( hOkButton != 0 ) {//���� ����� ������ �� �������� �� �������
				SendMessage( hOkButton, BM_CLICK, 0, 0 ); 
			} else {//���� �� ����� ������, ������ ������������ �� �������
				MsgBox( L"Notify", L"Please press OK button in dialog" );
			}
			return true;//���, �������
		}
	}
	return false;
}











































