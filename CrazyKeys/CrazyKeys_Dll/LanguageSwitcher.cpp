#include "LanguageSwitcher.h"
#include "TextFileFuncs.h"
#include "CrazyKeysConst.h"

#define SC_SHIFT 0x2A
#define SC_CONTROL 0x1D
#define SC_MENU 0x38

inline void initInputStruct( INPUT& inp, WORD sc, DWORD dwFlags = 0, bool isUnicode = false )
{
	ZeroData( inp );
	inp.type = INPUT_KEYBOARD;
	inp.ki.dwExtraInfo = MY_KEY_MESSAGE;
	inp.ki.wScan = sc;
	presume( !isUnicode || dwFlags == KEYEVENTF_KEYUP || dwFlags == 0 );
	inp.ki.dwFlags = ( isUnicode ) ? ( dwFlags | KEYEVENTF_UNICODE ) : ( dwFlags | KEYEVENTF_SCANCODE );
}

void CLanguageSwitcher::CSwitcher::CSequence::initWithShift( WORD wSysKey, char vChar )
{
	presume( ( wSysKey == 0 && vChar == 0 && size == 2 ) || ( vChar == 0 && size == 4 ) || ( vChar != 0 && size == 6 ) );
	if( wSysKey == 0 ) {
		initInputStruct( input[0], SC_SHIFT );//������ ��� ����� ������ �� ������� ������ ���� � ��������� ������
		initInputStruct( input[1], SC_SHIFT, KEYEVENTF_KEYUP );
	} else if( vChar == 0 ) {
		initInputStruct( input[0], SC_SHIFT );//������ ��� ����� ������ �� ������� ������ ���� � ��������� ������
		initInputStruct( input[1], wSysKey );
		initInputStruct( input[2], wSysKey, KEYEVENTF_KEYUP );		
		initInputStruct( input[3], SC_SHIFT, KEYEVENTF_KEYUP );
	} else {
		WORD charScanCode = 0x0B;
		if( '1' <= vChar && vChar <= '9' ) {
			charScanCode = (vChar - '1') + 2;
		}
		initInputStruct( input[0], wSysKey );
		initInputStruct( input[1], SC_SHIFT );	
		initInputStruct( input[2], charScanCode );

		initInputStruct( input[3], wSysKey, KEYEVENTF_KEYUP );		
		initInputStruct( input[4], SC_SHIFT, KEYEVENTF_KEYUP );
		initInputStruct( input[5], charScanCode, KEYEVENTF_KEYUP );			
	}
}

void CLanguageSwitcher::CSwitcher::CSequence::initWithoutShift( WORD wSysKey, char vChar )
{//[SysDown, SysUp] ���� [SysDown, vkDown, SysUp, vkUp], ��� Sys ��� ctrl ��� alt, � vk ��� �������, �������� 0
	presume( ( wSysKey == 0 && vChar == 0 && size == 0 ) || ( vChar == 0 && size == 2 ) || ( vChar != 0 && size == 4 ) );
	if( size == 0 ) {
		return;
	}
	initInputStruct( input[0], wSysKey );
	initInputStruct( input[(vChar == 0 ) ? 1 : 2], wSysKey, KEYEVENTF_KEYUP );//1 ��� 2
	if( ( ( '0' <= vChar && vChar <= '9' ) || vChar == '~' ) ) {//���������� �� ���� �����
		WORD charScanCode = 0x0B;
		if( '1' <= vChar && vChar <= '9' ) {
			charScanCode = (vChar - '1') + 2;
		}
		initInputStruct( input[1], charScanCode );//1, 3
		initInputStruct( input[3], charScanCode, KEYEVENTF_KEYUP );		
	}
}

void CLanguageSwitcher::initSwitcher( const CString& str, CSwitcher& switcher )
{
	switcher.withShift.size = 2 * ( str.PosCount( L"+" ) + 1 );
	switcher.withoutShift.size = 2 * str.PosCount( L"+" );
	CString sysKeyStr;//ctrl ��� alt, ��� ��������� ������� ����� �� ������ ���������
	CString exCharStr;//0..9 ��� ~, ��� ���. ������� ����� �� ������ ���������, �������� 0
	if( !str.Split( sysKeyStr, L"+shift+", exCharStr ) ) {//���������� �� ����
		str.Split( sysKeyStr, L"+shift", exCharStr );//���������� ����� �������
	}
	WORD scSysKey =  ( !sysKeyStr.IsEmpty() ) ? ( sysKeyStr == L"alt" ) ? SC_MENU : SC_CONTROL : 0;
	char exChar = exCharStr.IsEmpty() ? 0 : (char)exCharStr[0];
	switcher.withShift.initWithShift( scSysKey, exChar );
	switcher.withoutShift.initWithoutShift( scSysKey, exChar );
}

void CLanguageSwitcher::Load()
{
	isInited = false;
	CStrVector lines;
	if( ReadFileLines( CfgCangeLangFileName, lines ) && 0 < lines.GetSize() ) {
		initSwitcher( lines[0], back );
		initSwitcher( lines.GetSize() == 1 ? lines[0] : lines[1], to );
		isInited = true;
	}
}

//////////////////////////////////////////////////////////////////////////

void CLanguageSwitcher::SwitchTo( bool isUpper )
{
	if( !isInited ) {//���� ��� ������������������
		return;//�� ������ �� ������
	}
	CLanguageSwitcher::CSwitcher::CSequence& shiftY = to.withShift; 
	CLanguageSwitcher::CSwitcher::CSequence& shiftN = to.withoutShift;

	if( isUpper ) {//���� ����� ���������� ������� �����
		short shiftState = GetKeyState( VK_SHIFT );
		if( ( shiftState & 0x80 ) == 0 ) {//���� ���� �� ��� ������
			SendInput( shiftY.size, shiftY.input, sizeof( INPUT ) );
		} else if( 0 < shiftN.size ) {//���� ���� ��� ���������, � ���� ��� �����
			SendInput( shiftN.size, shiftN.input, sizeof( INPUT ) );
		}
	} else {//���� ���������� ���������
		SendInput( shiftY.size, shiftY.input, sizeof( INPUT ) );
	}
	isSwitchedTo = true;
}

void CLanguageSwitcher::SwitchBack( bool isUpper )
{
	if( !isInited ) {//���� ��� ������������������
		return;//�� ������ �� ������
	}
	CLanguageSwitcher::CSwitcher::CSequence& shiftY = back.withShift; 
	CLanguageSwitcher::CSwitcher::CSequence& shiftN = back.withoutShift;

	if( isUpper ) {//���������� ������� �����
		short shiftState = GetKeyState( VK_SHIFT );
		if( ( shiftState & 0x8000 ) == 0 ) {
			SendInput( shiftY.size, shiftY.input, sizeof( INPUT ) );
		} else if( 0 < shiftN.size ) {//���� ���� ��� �����
			SendInput( shiftN.size, shiftN.input, sizeof( INPUT ) );
		}
	} else {//���������� ��������� �����
		SendInput( shiftY.size, shiftY.input, sizeof( INPUT ) );
	}
	isSwitchedTo = false;
}

