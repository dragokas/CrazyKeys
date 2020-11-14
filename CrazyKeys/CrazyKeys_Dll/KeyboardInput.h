#pragma once
#define _WIN32_WINNT 0x0500
#include "LanguageSwitcher.h"
#include "Vector.h"

class CKonAutTranslit;

class CKeyboardInput {
public:
	CKeyboardInput();

	void SetOptions( const CVector<WORD>& lowWcharToScanCode, int sleepTime, bool _isUnicode );

	void SendInputPressedKey( CKonAutTranslit& KonAvt, wchar_t sourceLetter, bool isUpper );
	void SendInputScrollKey( bool isToggled );

private:
	void initBackSpaceSeqUpDown();
	void sendChangeLayout( bool needShift, bool isAdvancedMode = false );

	void initBackspaceSeqInp();

	CVector<INPUT> backSpaceSeqInp;
	INPUT spaceSeqInp[2];//пробел, т.к. он попадает из-за соблюдения последовательности в очереди и сброса сост ка
	INPUT scrollSeqInp[2];//Scroll Lock, эмитация нажатия

	//переход от буквы которую надо нажать в VKey который надо послать клаве
	CVector<WORD> targetWcharToScanCode;
	bool isUnicode;

	CLanguageSwitcher langSwitcher;//переключатель расскладки

	int longSleepTime;//время, которое выжидается между посылами нажатия клавиш
	int shortSleepTime;//время, которое выжидается между сменами раскладок

	DISALLOW_EVIL_CONSTRUCTORS( CKeyboardInput );
};


