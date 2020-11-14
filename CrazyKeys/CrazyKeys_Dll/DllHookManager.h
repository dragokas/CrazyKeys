#pragma once
#include "KeyboardInput.h"
#include "TimerQueue.h"
#include "KonAutTranslit.h"

enum THookState;

class CDllHookManager {
public:
	CDllHookManager();
	~CDllHookManager() {};

	bool SetHookState( THookState state, HWND hWndMain, UINT crazyKeysMessage );

	bool OnProcHook( const PKBDLLHOOKSTRUCT& hookStruct, WPARAM wParam );
	bool OnProcMouseHook(const PMSLLHOOKSTRUCT& hookStruct, WPARAM wParam);
	void OnTimer();

private:
	void startTimer();
	void stopTimer();
	bool loadOptions();
	void changeHookState( THookState state );
	bool onShift( WPARAM wParam, const PKBDLLHOOKSTRUCT& hookStruct );

	THookState hookState;//���� HS_Off, ������ ����� �� ���������

	HWND hWnd;//���� ����� � �������� ������������ �������
	UINT crazyKeysMsg;//��������� ������� ���� �����, � � wParam ���������� ������ 

	bool isShiftDown;//����� �� ����
	bool isCapsDown;//����� �� CapsLock
	CVector<bool> isSysemVkCode;//������� ������� ����� ���� ������, ctrl, alt ...
	CVector<bool> isSysemVkCodeDown;//��������� ������� ������� ����� ���� ������
	int nSystemVkCodesDown;//���������� ������� ������

	CKeyboardInput sendInpMgr;//�������� ���������
	CKonAutTranslit konAvt;//�������� �������, ���������������
	CTimerQueue wcharTimeQueue;//������� ��������� ����, ����� ���� ����������
	UINT_PTR uptrTimer;//������
	short switchKey;//������� ������������
	bool blockSwitchKey;//����������� ������������� �.�. �� �������� �������
	unsigned short timerTime;//����� ��� �������
	unsigned short delayTime;//����� �������� ��� ���������� ������������������ � ������ �����
	CVector<int> keysUpAndDownCounters;//���������� down++ Up-- 
	CVector<wchar_t> scanCodeToSourceLowLetter;//������� �������� ������ �� scan code � source �������

	DISALLOW_EVIL_CONSTRUCTORS( CDllHookManager );
};
