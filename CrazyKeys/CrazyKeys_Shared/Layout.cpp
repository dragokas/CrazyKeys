#include <stdio.h>
#include "Layout.h"
#include "Queue.h"

wchar_t VirtualKeyToLowWchar( WORD vKey, HKL hkl ) 
{//из вкея ищет букву наиболее подходящую
	BYTE keyState[256]; 
	ZeroData( keyState );
	wchar_t wchars[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int nLetters = ToUnicodeEx( vKey, 0, keyState, wchars, 10, 0, hkl );
	if( nLetters == 1 ) {
		return wchars[0];
	}
	//не всегда ToUnicodeEx работает правильно, иногда выдаёт лишнии символы
	for( int i = 0; i < nLetters; i++ ) {
		if( vKey == (WORD)VkKeyScanEx( wchars[i], hkl ) ) {
			return wchars[i];//мы его руками нашли
		}
	}
	return 0;
}

bool CheckSetWchar( UINT scanCode, wchar_t& wchar, HKL hkl )
{//по сканкоду ставит букву, с супер проверкой: сканкод->вкей->буква->вкей->сканкод.
	UINT vKey = MapVirtualKeyEx( scanCode, 1, hkl );//сканкод->вкей
	wchar = VirtualKeyToLowWchar( (WORD)vKey, hkl );//вкей->буква
	USHORT vKey2 = VkKeyScanEx( wchar, hkl );//буква->вкей
	UINT scanCode2 = MapVirtualKeyEx( vKey2, 0, hkl );//вкей->сканкод
	return ( wchar != 0 && scanCode == scanCode2 && vKey == vKey2 );
}

CLayout::CLayout( HKL hkl ) : hKL( hkl ), 
lowWcharToScanCode( 256 * 256, 0 ), scanCodeToLowWchar( 256, 0 ), hotkey( EmptyStr )
{
	if( IsUnicode() ) {
		initHKLInfo();
	} else {
		HKL saved = ActivateKeyboardLayout( hkl, 0 );
		initHKLInfo();
		//заполняем 2 отображения: буква<->сканкод
		for( WORD scanCode = 0; scanCode < 256; scanCode++ ) {
			wchar_t wchar = 0;
			if( CheckSetWchar( scanCode, wchar, hkl ) ) {
				scanCodeToLowWchar[scanCode] = wchar;
				lowWcharToScanCode[wchar] = scanCode;
			}
		}
		ActivateKeyboardLayout( saved, 0 );
	}
}

CString CLayout::getLayoutName( DWORD id ) const
{
	wchar_t langBuf[300];
	swprintf( langBuf, L"%8X", id ); 
	for( int i = 0; langBuf[i] == L' '; i++ ) {
		langBuf[i] = L'0';
	}
	CString layoutName;
	GetStrFromRegistry( HKEY_LOCAL_MACHINE, 
		CString(L"System\\CurrentControlSet\\control\\keyboard layouts\\") + langBuf, L"Layout Text", layoutName );
	return layoutName;
}

CString CLayout::getLanguageName( DWORD id ) const
{
	wchar_t langBuf[512] = L"";
	GetLocaleInfo( MAKELCID( id, SORT_DEFAULT ), LOCALE_SLANGUAGE, langBuf, 512 );
	CString languageName = langBuf;
	return languageName.IsEmpty() ? L"???" : languageName;	
}

void CLayout::initHKLInfo()
{
	if( IsUnicode() ) {
		languageName = L"Unicode";
		layoutName = L"anyone";
	} else {
		hotkey.Language = LOWORD( hKL );
		wchar_t tmpStr[KL_NAMELENGTH];
		GetKeyboardLayoutName( tmpStr );
		hotkey.Layout = (WORD)Str2Int( tmpStr, 16 );
		languageName = getLanguageName( hotkey.Language );
		layoutName = getLayoutName( hotkey.Layout );
	}
}

bool CLayout::IsWordInLayout( const CString& word ) const
{
	if( IsUnicode() ) {
		return true;
	}
	for( int i = 0; i < word.Length(); i++ ) {
		if( lowWcharToScanCode[word[i]] == 0 ) {
			return false;
		}
	}
	return true;
}

CString CLayout::GetNameWithSeq() const 
{
	if( !changeLayoutSequenceString.IsEmpty() ) {
		return getName() + L" <" + changeLayoutSequenceString + L">"; 
	} 
	return getName();
}

void CLayout::SetChangeLayoutSequence( const CList<CRegHotKey>& hotKeys )
{
	changeLayoutSequenceString = EmptyStr;
	for( int i = 0; i < hotKeys.GetCount(); i++ ) {
		if( hotkey.LL == hotKeys[i].LL ) {
			hotkey = hotKeys[i];
			changeLayoutSequenceString = ( ( hotkey.CtrlOrAlt == 5 ) ? L"alt+shift+" : L"ctrl+shift+" ) + 
				Int2Str( (hotkey.ExKey)%16 );//не совсем понятно почему 16...
			return;
		}
	}
	hotkey.ClearHotkeyData();//не нашли значит обнуляем
}










