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
	INPUT spaceSeqInp[2];//������, �.�. �� �������� ��-�� ���������� ������������������ � ������� � ������ ���� ��
	INPUT scrollSeqInp[2];//Scroll Lock, �������� �������

	//������� �� ����� ������� ���� ������ � VKey ������� ���� ������� �����
	CVector<WORD> targetWcharToScanCode;
	bool isUnicode;

	CLanguageSwitcher langSwitcher;//������������� ����������

	int longSleepTime;//�����, ������� ���������� ����� �������� ������� ������
	int shortSleepTime;//�����, ������� ���������� ����� ������� ���������

	DISALLOW_EVIL_CONSTRUCTORS( CKeyboardInput );
};


