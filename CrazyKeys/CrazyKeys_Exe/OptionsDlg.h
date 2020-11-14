#pragma once
#include <windows.h>
#include "Options.h"
#include "ToolTip.h"
#include "Dialog.h"

struct CSpin;

class COptionsDlg : public CDialog {
public:
	COptionsDlg( HINSTANCE hInstance ) : CDialog( hInstance ) {}
	void ShowDlg( HWND hParentWnd, UINT dlgMsg );
//	static HWND GetDlghWnd();

protected:
	virtual BOOL OnInitDlg();
	virtual BOOL OnHelpDlg( HELPINFO* helpInfo );
	virtual BOOL OnCommandDlg( WPARAM wParam, LPARAM lParam );
	virtual BOOL OnNotifyDlg( WPARAM wParam, LPARAM lParam );
	virtual BOOL OnProcDlg( UINT uMsg, WPARAM wParam, LPARAM lParam );

private:
			///�������
		///���� ������
	//������ ������ ������ � ��������� ���
	void initRuleList() const;
	//��������� ������ ������
	void fillRuleList() const;
		///������ � ��������
	//����������� �� ��������� ������� ����
	void setVisibleModifyed() const;
	//�������� �������
	void onAddNewRule();
	//�������� �������
	void onEditRule();
	//������� �������
	void onDeleteRule();
	//��������� ����� �������
	void onLoadDefaultRules();
	//������� ������ �������
	void onRemoveBadRules();

			///���������
		///������
	//�� ��������� ������ �����
	void fillButtonsText( int firstIndex, TLayouts which ) const;
		///������ ���������
	//����������� ������������ �����
	void calculateKbParams( int dlgStaticId, int& size, int& xOffset, int& yOffset ) const;
	//������ ���������
	void initKeyboard( int dlgStaticId );
	//������ � ��������� 2 ���������
	void initKeyboards();
	//��������� 2 ���������
	void fillKeyboards() const;

		///����� �����
	//��������� ������ �����������
	void fillComboBox( HWND hCombo, TLayouts which ) const;
	//��������� ��� ������� �����������
	void fillComboBoxes() const;
		///����� � ���������
	//������� ��������� �� �� ��� ������� � �������
	void onChangeInLayoutCombo( HWND hCombo, TLayouts which );
	//��������� � ������� ������� ���������
	void onAddRussianLanguageLayout();
	//������������ ����� ������ � ���������
    void onRefreshLanguagesLayout();
	//�������� ��������� ������ ������ � ���������
	void onDialogLanguagesLayout();
	//���������� ����������� ������� ���������
	void onOptimalLanguagesLayout();
	//� ��������� ������� ������ ����� ���� 
	bool applyWindowsOptions() const;

		///��� ������� � �����
	//��������� ������
	void fillDlg() const;
	//��������� �����, ����� ���������� � ����� ������
    bool onOk();
	//���������� ��������� ��� OpenSave Common �������
	void fillFileNameStruct( OPENFILENAME& ofn, wchar_t* title, wchar_t* buffer, wchar_t* currentDirName ) const;
	//��������� ����� �� �����
	void onLoadOptionsFile();
	//��������� ����� � ����
	void onSaveOptionsFile( CString saveAsFileName = EmptyStr );
	//��������� ����� � ���� ������� �������������
	void onSaveAsOptionsFile();

		///�����
	//��������� �����
	void setSpinCtrl( int spinID, const CSpin& spin, short value ) const;
	void setSpinCtrls() const;
	//������� �����
	short getSpinCtrl( int spinID, const CSpin& spin ) const;
	void getSpinCtrls();

		///������ �������������
	//��������� ������ �������������
	void initSwitchCombo() const;
	//��������� ������ ���������� 256 �������� � �������������
	void fillSwitchKey() const;
	//�� ��������� ���������� ���� � �������
	void onChangeInVKNamesCombo( HWND hCombo );
	//�� ��������� �� ������ �� OnProcDlg
	void onKeyDownInCombo( BYTE vk );
	//������ ������ ����� �������
	void setVKeyFace( BYTE vk ) const;

		///���� ������� � ����
	//��������� ����� � ������
	void fillLanguageCombo() const;
	//�� ��������� ������ � �������
	void onChangeInLanguageCombo( HWND hCombo );

	void showReport() const;

	HFONT hVKkeyFaceFont;
	HFONT hKbKeyFont;
	//�������� ��������
	int nButtons;
	HWND hWndButtons[2 * 4 * 15];//2 �����, �� 4 ����, ��� �� ����� 15

	CToolTip toolTip;

	HWND hParent;
	UINT msgToParentWnd;
	COptions options;//�� ��� ������ � ���� �������

	//����������� ������ �������
	INT_PTR onButton( WPARAM wParam );

	DISALLOW_EVIL_CONSTRUCTORS( COptionsDlg );
};