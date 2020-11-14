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
			///правила
		///лист правил
	//создаёт список правил и заполняет его
	void initRuleList() const;
	//заполняет список правил
	void fillRuleList() const;
		///кнопки к правилам
	//подкручивет на изменённый элемент лист
	void setVisibleModifyed() const;
	//добавить правило
	void onAddNewRule();
	//изменить правило
	void onEditRule();
	//удалить правило
	void onDeleteRule();
	//загрузить новые правила
	void onLoadDefaultRules();
	//удалить плохие правила
	void onRemoveBadRules();

			///раскладка
		///кнопки
	//на кнопочках рисует буквы
	void fillButtonsText( int firstIndex, TLayouts which ) const;
		///кнопки раскладки
	//расщитывает расположение клавы
	void calculateKbParams( int dlgStaticId, int& size, int& xOffset, int& yOffset ) const;
	//создаёт раскладку
	void initKeyboard( int dlgStaticId );
	//создаёт и заполняет 2 раскладки
	void initKeyboards();
	//заполняет 2 раскладки
	void fillKeyboards() const;

		///комбо боксы
	//заполняет комбик раскладками
	void fillComboBox( HWND hCombo, TLayouts which ) const;
	//заполняет оба комбика раскладками
	void fillComboBoxes() const;
		///языки и раскладки
	//сменить раскладку на ту что выбрана в комбике
	void onChangeInLayoutCombo( HWND hCombo, TLayouts which );
	//загрузить в систему русскую раскладку
	void onAddRussianLanguageLayout();
	//перечитывает опции языков и раскладок
    void onRefreshLanguagesLayout();
	//вызывает системный диалог языков и раскладок
	void onDialogLanguagesLayout();
	//выставляет оптимальные клавиши сочетаний
	void onOptimalLanguagesLayout();
	//в системном диалоге нажать нопку окей 
	bool applyWindowsOptions() const;

		///для диалога в целом
	//заполнить диалог
	void fillDlg() const;
	//сохранить опции, иначе руганыться и ждать кенсэл
    bool onOk();
	//заполнения структуры для OpenSave Common диалога
	void fillFileNameStruct( OPENFILENAME& ofn, wchar_t* title, wchar_t* buffer, wchar_t* currentDirName ) const;
	//загрузить опции из файла
	void onLoadOptionsFile();
	//сохранить опции в фаил
	void onSaveOptionsFile( CString saveAsFileName = EmptyStr );
	//сохранить опции в фаил который запрашивается
	void onSaveAsOptionsFile();

		///спины
	//выставить спины
	void setSpinCtrl( int spinID, const CSpin& spin, short value ) const;
	void setSpinCtrls() const;
	//считать спины
	short getSpinCtrl( int spinID, const CSpin& spin ) const;
	void getSpinCtrls();

		///кнопка переключатель
	//сабкласит комбик переключателя
	void initSwitchCombo() const;
	//заполнить комбик возможными 256 кнопками и отсабклассить
	void fillSwitchKey() const;
	//на изменение выбранного поля в комбике
	void onChangeInVKNamesCombo( HWND hCombo );
	//на изменение по кнопке из OnProcDlg
	void onKeyDownInCombo( BYTE vk );
	//ставит рожицу около комбика
	void setVKeyFace( BYTE vk ) const;

		///язык диалога и меню
	//поставить языки в комбик
	void fillLanguageCombo() const;
	//на изменение языков в комбике
	void onChangeInLanguageCombo( HWND hCombo );

	void showReport() const;

	HFONT hVKkeyFaceFont;
	HFONT hKbKeyFont;
	//созданые клавишки
	int nButtons;
	HWND hWndButtons[2 * 4 * 15];//2 клавы, по 4 ряда, где не более 15

	CToolTip toolTip;

	HWND hParent;
	UINT msgToParentWnd;
	COptions options;//то что меняем в етом диалоге

	//обработчики кнопок диалога
	INT_PTR onButton( WPARAM wParam );

	DISALLOW_EVIL_CONSTRUCTORS( COptionsDlg );
};