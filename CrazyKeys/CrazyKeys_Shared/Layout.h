#pragma once
#include <windows.h>
#include "Str.h"
#include "Vector.h"
#include "Registry.h"

class CLayouts;

struct CRegHotKey {
//	[HKEY_CURRENT_USER\Control Panel\Input Method\Hot Keys\00000X] X = {100...200} в 16-ти ричной системе
//	"Key Modifiers"=hex: 1 байт - битовая маска какие клавиши зажаты {alt=1, ctrl=2, shift=4},c0,00,00
//	"Target IME"=hex: 2 байта - (language) первый оригинальный язык, 2 байта - (layout) подязык который определяет раскладку
//	"Virtual Key"=hex: 1 байт - добавочный вкей в комбинации,00,00,00
//	пример : 
//	"Key Modifiers"=hex:06,c0,00,00
//	"Target IME"=hex:06,04,06,04
//	"Virtual Key"=hex:35,00,00,00 
	CRegHotKey( const CString& regStr = EmptyStr ) : regLocation( regStr ), CtrlOrAlt( km.bVal[0] ),
	Language( ti.wVal[0] ), Layout( ti.wVal[1] ), LL( ti.dwVal ), ExKey( vk.bVal[0] )
	{//чтобы не мучаться с запоминанием битов, они сделаны как ссылки на нужные данные
		km.dwVal = ti.dwVal = vk.dwVal = 0;
		km.bVal[1] = 0xc0;//в реестре она всегда есть
	}
	CRegHotKey( const CRegHotKey& other ) : regLocation( other.regLocation ), CtrlOrAlt( km.bVal[0] ),
	Language( ti.wVal[0] ), Layout( ti.wVal[1] ), LL( ti.dwVal ), ExKey( vk.bVal[0] )
	{//чтобы не мучаться с запоминанием битов, они сделаны как ссылки на нужные данные
		km = other.km; ti = other.ti; vk = other.vk;
		presume( km.bVal[1] == 0xc0 );
		km.bVal[1] = 0xc0;//в реестре она всегда есть
	}
	CRegHotKey& operator=( const CRegHotKey& other ) 
	{ 
		regLocation = other.regLocation;
		km = other.km; ti = other.ti; vk = other.vk;  
		return *this;
	}

	void ClearHotkeyData() { CtrlOrAlt = ExKey = 0; regLocation = EmptyStr; }
	bool IsEmptyHotkey() const { presume( ( CtrlOrAlt == 0 ) == ( ExKey == 0 ) ); return ( CtrlOrAlt == 0 ); }

	BYTE& CtrlOrAlt;//бит зажатия : 6-ctrl или 5-alt 
	WORD& Language;//язык
	WORD& Layout;//раскладка
	DWORD& LL;//язык + раскладка
	BYTE& ExKey;//цифра с зажимания от ['0'..'9']

	bool operator==( const BYTE& exKey ) const { return ( CtrlOrAlt == 6 && ExKey == exKey ); }
	bool operator==( const CString& regPath ) const { return ( regLocation == regPath ); } 

private:
	CString regLocation;//строка в реестре
	UBinRegValue km;//"Key Modifiers", бит с 6-ctrl или 5-alt 
	UBinRegValue ti;//"Target IME", язык и раскладка
	UBinRegValue vk;//"Virtual Key", цифра зажимания

	friend CLayouts;//т.к. этот класс эти структуры заполняет
};

//------------------------------------------------------------------------

template <typename T> class CList;

class CLayout {
public:
	CLayout( HKL hkl );

	bool IsUnicode() const { return ( hKL == 0 );}
	const HKL GetHKL() const { return hKL; }
	CString GetNameWithSeq() const;
	CString GetNameWithDigits() const { return getName() + L" (" + Int2Str( hotkey.LL, 16 ) + L")"; }
	CString GetLayoutDigitName() const { return Int2Str( hotkey.Layout, 16 ); }
	const CVector<wchar_t>& GetScanCodeToWcharMap() const { return scanCodeToLowWchar; } 
	const CVector<WORD>& GetWcharToScanCodeMap() const { return lowWcharToScanCode; } 
	bool IsWordInLayout( const CString& word ) const;

	CString GetChangeLayoutSequenceStr() const { return changeLayoutSequenceString; }
	const CRegHotKey& GetHotkey() const { return hotkey; }
	void SetChangeLayoutSequence( const CList<CRegHotKey>& hotKeys );

	bool operator==( const HKL& other ) const { return ( hKL == other ); }
	bool operator==( const DWORD& otherLL ) const { return ( hotkey.LL == otherLL ); }

private:
	CString getName() const { return L"[" + languageName + L" - " + layoutName + L"] "; };
	void initHKLInfo();
	CString getLayoutName( DWORD id ) const;
	CString getLanguageName( DWORD id ) const;
	
	CRegHotKey hotkey;//информация о раскладке и переключение на неё из реестра
	CString changeLayoutSequenceString;//комбинация смены на эту раскладку

	const HKL hKL;

	CString languageName;//имя языка
	CString layoutName;//имя раскладки

	CVector<wchar_t> scanCodeToLowWchar;//из сканкодов в буквы
	CVector<WORD> lowWcharToScanCode;//из букв в сканкоды

	DISALLOW_EVIL_CONSTRUCTORS( CLayout );
};
