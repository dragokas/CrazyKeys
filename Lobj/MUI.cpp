#include "MUI.h"

#include "LexicalTree.h"
#include "Str.h"

static const int MaxStrLength = 512;//макс. длина строчки в диалоге

class CMUI {
public:
	CMUI() : needTranslate( false ) {}
	void SetDictionary( const CStrVector& source, const CStrVector& target );
	void SetEmptyDictionary() { needTranslate = false; strToStrDictionary.DeleteAll(); };
	CString TranslateStr( const CString& str );
	void TranslateMenu( HMENU hMenu );
	void TranslateDlg( HWND hDlg );	
private:
	CLexicalTree<CString> strToStrDictionary;
	CLexicalTree<CString> idToSourceLanguege;
	bool needTranslate;
};

//////////////////////////////////////////////////////////////////////////

void CMUI::SetDictionary( const CStrVector& source, const CStrVector& target )
{
	presume( source.Size() == target.Size() );
	SetEmptyDictionary();
    for( int i = 0; i < source.Size(); i++ ) {
		const CString& str = source[i];
		CString* pString = strToStrDictionary.Find( str.GetBuffer(), str.Length() * sizeof( CString::ElementType ) );
		presume( pString == 0 );
		if( pString == 0 ) {
			strToStrDictionary.Add( str.GetBuffer(), str.Length() * sizeof( CString::ElementType ), target[i] );
		}
	}
	needTranslate = ( 0 < source.Size() );
}

CString CMUI::TranslateStr( const CString& str )
{
	if( !needTranslate || str.IsEmpty() ) {
		return str;
	}
	CString* pString = strToStrDictionary.Find( str.GetBuffer(), str.Length() * sizeof( CString::ElementType ) );
	return ( pString != 0 ) ? *pString : str;
}

void CMUI::TranslateMenu( HMENU hMenu )
{
	if( !needTranslate ) {
		return;
	}
	int count = GetMenuItemCount( hMenu );
	for( int i = 0; i < count; i++ ) {
		MENUITEMINFO mii = { 0 };
		ZeroData( mii );
		mii.cbSize = sizeof( MENUITEMINFO );
		mii.fMask = MIIM_STRING;
		mii.fType = MFT_STRING;
		TCHAR textInMenuItem[MaxStrLength] = { 0 };
		mii.cch = MaxStrLength;
		mii.dwTypeData = textInMenuItem;
		if( GetMenuItemInfo( hMenu, i, TRUE, &mii ) != 0 ) {
			CString newStr = TranslateStr( mii.dwTypeData );
			mii.cch = newStr.Length();
			mii.dwTypeData = (TCHAR*)newStr.GetBuffer();
			BOOL res = SetMenuItemInfo( hMenu, i, TRUE, &mii );
			presume( res != 0 );
		}
	}
}

static bool isCtrlForTranslate( HWND hwnd, LONG wndId )
{
	if( wndId < 0 ) {
		return false;
	}
	TCHAR className[256] = {0};
    GetClassName( hwnd, className, 256 );
	if( StrCmpLow( className, T("EDIT") ) ) {
		return false;
	}
	return true;
}

static BOOL CALLBACK EnumChildProc( HWND hwnd, LPARAM lParam = 0 )
{
	LONG wndId = GetWindowLong( hwnd, GWL_ID );
	if( isCtrlForTranslate( hwnd, wndId ) ) {
		CLexicalTree<CString>* pIdToSourceLanguege = (CLexicalTree<CString>*)lParam;
		CString* pSourceText = pIdToSourceLanguege->Find( wndId );
		if( pSourceText != 0 ) {// этот контрол мы уже переводили, и его надо переводить из исходного языка
			CString newStr = TranslateStr( *pSourceText );
			SetWindowText( hwnd, newStr.GetBuffer() );
		} else {// этот контрол ещё не был переведён, и исходныи язык на нём сейчас
			TCHAR text[MaxStrLength];
			GetWindowText( hwnd, text, MaxStrLength );
			CString textInDlgItem = text;
			CString newItemText = TranslateStr( textInDlgItem );
			if( newItemText != textInDlgItem ) {// нашли перевод
				pIdToSourceLanguege->Add( wndId, textInDlgItem );
				SetWindowText( hwnd, newItemText.GetBuffer() );
			}
		}
	}
	return TRUE;
}

void CMUI::TranslateDlg( HWND hDlg )
{
	if( !needTranslate ) {
		return;
	}
	TCHAR oldCaption[MaxStrLength];
	GetWindowText( hDlg, oldCaption, MaxStrLength );
    CString newCaption = TranslateStr( oldCaption );
	SetWindowText( hDlg, newCaption );
	EnumChildWindows( hDlg, EnumChildProc, (LPARAM)&(idToSourceLanguege) );
}

//////////////////////////////////////////////////////////////////////////

CMUI& GetMUI() 
{ 
	static CMUI mui; 
	return mui; 
}

void SetDictionary( const CStrVector& source, const CStrVector& target ) 
{
	GetMUI().SetDictionary( source, target ); 
}

CString TranslateStr( const CString& str )
{
	return GetMUI().TranslateStr( str );
}

void TranslateDlg( HWND hDlg )
{
	GetMUI().TranslateDlg( hDlg );
}

void TranslateMenu( HMENU hMenu ) 
{
	GetMUI().TranslateMenu( hMenu );
}







































