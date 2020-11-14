#include "MUI.h"
#include "CrazyKeysConst.h"
#include "ShellFuncs.h"
#include "TextFileFuncs.h"
#include "CrazyKeysMUIStrings.h"

#include "CrazyKeysMUI.h"

TLanguageMUI initLanguageMUI();
TLanguageMUI LanguageMUI = initLanguageMUI();

CString getUserLanguageFileName() 
{
	return GetUserName() + T("_Language.cfg");
}

void setDictionary( TLanguageMUI targetLanguage )
{
	CStrVector source, target;
	for( int i = 0; Translation[i][MUI_Eng][0] != 0; i++ ) {
		source.Append( Translation[i][MUI_Eng] );
		target.Append( Translation[i][targetLanguage] );
	}
	SetDictionary( source, target );
}

TLanguageMUI initLanguageMUI()
{
	TLanguageMUI language = MUI_Eng;
	CString line;
	if( ReadFileBuffer( getUserLanguageFileName(), line ) ) {
		switch( (TLanguageMUI)(Str2Int( line )) ) {
			case MUI_Rus: language = MUI_Rus; break;
			default: language = MUI_Eng; break;
		}
	} else {
		switch( PRIMARYLANGID( GetUserDefaultUILanguage() ) ) {
			case LANG_RUSSIAN: language = MUI_Rus; break;
			default: language = MUI_Eng;//MUI_Eng; break;
		}		
	}
	setDictionary( language );
	return language;
}

void SetLanguageMUI( TLanguageMUI language ) 
{
	if( LanguageMUI != language ) {
		LanguageMUI = language;
		WriteFileBuffer( getUserLanguageFileName(), Int2Str( (int)( language ) ) );
		setDictionary( language );
	}
}

TLanguageMUI GetLanguageMUI() 
{
	return LanguageMUI; 
}

CString getCrazyKeysVersion()
{
	return T("1.74");
}

void ShowHelpTopic( HWND hWnd, DWORD topicRef )
{
	CheckZero( WinHelp( hWnd, HelpFileName, HELP_CONTEXT, topicRef ) );
}

CString GetCrazyKeysVersionEx()
{
	return TranslateStr(T("version ")) + getCrazyKeysVersion() + TranslateStr(T(", from ")) + 
		GetStr(__DATE__).GetBuffer() + T(", ") + GetStr(__TIME__).GetBuffer();
}

void TranslateCrazyKeysDlg( HWND hDlg )
{
	TranslateDlg( hDlg );

	TCHAR caption[MaxStrLength];
	GetWindowText( hDlg, caption, MaxStrLength );
	CString captionStr = caption;
	CString version = T(" (v.") + getCrazyKeysVersion() + T(")");
	int versionPos = captionStr.Find( version );
	if( versionPos == -1 ) {
		CString newCaption = captionStr + version;
		SetWindowText( hDlg, newCaption.GetBuffer() );
	}	
}


