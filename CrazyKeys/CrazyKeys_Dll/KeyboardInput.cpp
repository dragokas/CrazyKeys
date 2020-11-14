#define _WIN32_WINNT 0x0500
#include <windows.h>
#include "KeyboardInput.h"
#include "KonAutTranslit.h"

static const int WordLength= 2;

#define SC_BACK 0x0E
#define SC_SPACE 0x39
#define SC_SCROLL 0x46

extern void initInputStruct( INPUT& inp, WORD sc, DWORD dwFlags = 0, bool isUnicode = false );

inline void initKeyDownAndUp( INPUT* inp, WORD sc )
{
	initInputStruct( inp[0], sc );
	initInputStruct( inp[1], sc, KEYEVENTF_KEYUP );
}

CKeyboardInput::CKeyboardInput()
: backSpaceSeqInp( 2 )
{
	initKeyDownAndUp( spaceSeqInp, SC_SPACE );
	initKeyDownAndUp( scrollSeqInp, SC_SCROLL );
	initBackspaceSeqInp();
	isUnicode = false;
}

void CKeyboardInput::initBackspaceSeqInp()
{
	for( int i = 0; i < backSpaceSeqInp.GetSize(); i += 2) {//инициализируем backSpace
		initKeyDownAndUp( backSpaceSeqInp.GetPtr() + i, SC_BACK );
	}
}

void CKeyboardInput::SetOptions( const CVector<WORD>& lowWcharToScanCode, int sleepTime, bool _isUnicode )
{
	targetWcharToScanCode.CopyFrom( lowWcharToScanCode );
	longSleepTime = min( 999, max( 1, sleepTime ) );
	shortSleepTime = longSleepTime / 3;
	isUnicode = _isUnicode;
	if( !isUnicode ) {
		langSwitcher.Load();
	}
}

void CKeyboardInput::SendInputPressedKey( CKonAutTranslit& KonAvt, wchar_t sourceLetter, bool isUpper )
{
	if( sourceLetter == L' ' ) {
		KonAvt.ResetState();
		SendInput( 2, spaceSeqInp, sizeof( INPUT ) );
		Sleep( shortSleepTime );
		return;
	}
	CString strToWrite( WordLength );
	int toDeleteCount = 0;
	KonAvt.GetNext( sourceLetter, toDeleteCount, strToWrite );
	if( 0 < toDeleteCount ) {//если нужно удалять буквы, то посылаем нажатия backSpaces
		if( backSpaceSeqInp.GetSize() < 2 * toDeleteCount ) {
			backSpaceSeqInp.SetSize( 2 * toDeleteCount );
			initBackspaceSeqInp();
		}
		SendInput( 2 * toDeleteCount, backSpaceSeqInp.GetPtr(), sizeof( INPUT ) );
		Sleep( longSleepTime );
	}
	if( !isUnicode ) {
		langSwitcher.SwitchTo( isUpper );//отсылаем смену раскладки
		Sleep( shortSleepTime );
	}	
	for( int i = 0; i < strToWrite.Length(); i++ ) {
        wchar_t targetChar = strToWrite[i];
		WORD sc = targetWcharToScanCode[targetChar];
		if( isUpper && isUnicode ) {
			wchar_t upperChar[2] = { targetChar, 0 };
			CharUpper( upperChar );
			targetChar = upperChar[0];
		}
		INPUT toWriteSeqInp[2];
		initInputStruct( toWriteSeqInp[0], isUnicode ? targetChar : sc, 0, isUnicode );
		initInputStruct( toWriteSeqInp[1], isUnicode ? targetChar : sc, KEYEVENTF_KEYUP, isUnicode );
		SendInput( 2, toWriteSeqInp, sizeof( INPUT ) );//отсылаем нажатие новой буквы
		Sleep( longSleepTime );
	}
	if( !isUnicode ) {
		langSwitcher.SwitchBack( isUpper );//отсылаем смену раскладкиu
		Sleep( shortSleepTime );
	}	
}

void CKeyboardInput::SendInputScrollKey( bool isToggled )
{//дело в том, что GetKeyState идр, глючат если клавиша была нажата тут программой а не физически
	if( isToggled || ( (GetKeyState( VK_SCROLL )&1) != 0 ) ) {
		CheckZero( SendInput( 1, scrollSeqInp, sizeof( INPUT ) ) );
		Sleep( 5 );
		CheckZero( SendInput( 1, scrollSeqInp + 1, sizeof( INPUT ) ) );
		Sleep( 5 );
	}
}
