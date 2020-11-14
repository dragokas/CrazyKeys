#pragma once
#include <windows.h>

enum TLanguageMUI {
	MUI_Eng = 0,
	MUI_Rus = 1,
	MUI_Count
};

void SetLanguageMUI( TLanguageMUI language );
TLanguageMUI GetLanguageMUI();

void ShowHelpTopic( HWND hWnd, DWORD topicRef = 0 );//показать помощь
CString GetCrazyKeysVersionEx();
void TranslateCrazyKeysDlg( HWND hDlg );//перевести диалог на язык операционной системы
