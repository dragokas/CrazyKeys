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

	THookState hookState;//если HS_Off, значит опции не загружены

	HWND hWnd;//кому слать о событи€х переключени€ статуса
	UINT crazyKeysMsg;//сообщение которое надо слать, и в wParam передавать статус 

	bool isShiftDown;//зажат ли шифт
	bool isCapsDown;//зажат ли CapsLock
	CVector<bool> isSysemVkCode;//клавиши которые могут быть зажаты, ctrl, alt ...
	CVector<bool> isSysemVkCodeDown;//состо€ние клавишы котора€ может быть зажата
	int nSystemVkCodesDown;//количество зажатых клавиш

	CKeyboardInput sendInpMgr;//отсылает сообщени€
	CKonAutTranslit konAvt;//конечный автомат, преобразователь
	CTimerQueue wcharTimeQueue;//очередь обработки букв, чтобы всех обработать
	UINT_PTR uptrTimer;//таймер
	short switchKey;//клавиша переключени€
	bool blockSwitchKey;//блокировать переключатель т.е. не отдавать системе
	unsigned short timerTime;//врем€ дл€ таймера
	unsigned short delayTime;//врем€ ожидани€ дл€ склеивани€ последовательности в единое целое
	CVector<int> keysUpAndDownCounters;//количество down++ Up-- 
	CVector<wchar_t> scanCodeToSourceLowLetter;//таблица перевода клавиш из scan code в source символы

	DISALLOW_EVIL_CONSTRUCTORS( CDllHookManager );
};
