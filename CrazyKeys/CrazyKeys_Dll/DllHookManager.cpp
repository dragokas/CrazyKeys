#include "DllHookManager.h"
#include "Str.h"
#include "Options.h"
#include "CrazyKeysConst.h"

extern VOID CALLBACK TimerProc( HWND,UINT uMsg, UINT_PTR, DWORD );

//HS_Off - ловушка не включена и опции не загружены
//HS_Pause - ловушка на предохранителе и всё загружено
//HS_On - всё загружено

CDllHookManager::CDllHookManager() : hookState( HS_Off ), keysUpAndDownCounters( 256, 0 ), uptrTimer( 0 ),
isSysemVkCode( 256, false ), isSysemVkCodeDown( 256, false )
{
	isCapsDown = isShiftDown = false;
	nSystemVkCodesDown = 0;//счётчик и клавиши которые могут быть зажаты
	isSysemVkCode[VK_LCONTROL] = isSysemVkCode[VK_RCONTROL] = true;
	isSysemVkCode[VK_LWIN] = isSysemVkCode[VK_RWIN] = true;
	isSysemVkCode[VK_LMENU] = isSysemVkCode[VK_RMENU] = true;
	isSysemVkCode[VK_APPS] = true;
}

void CDllHookManager::startTimer()
{
	presume( uptrTimer == 0 );
	uptrTimer = SetTimer( NULL, 0, timerTime, TimerProc );
}

void CDllHookManager::stopTimer()
{
	presume( uptrTimer != 0 ); 
	CheckZero( KillTimer( 0, uptrTimer ) );
	uptrTimer = 0;
}

bool CDllHookManager::SetHookState( THookState state, HWND hWndMain, UINT crazyKeysMessage )
{
	presume( state == HS_On || state == HS_Pause || state == HS_Off );
	if( hookState == state ) {
		return true;
	}
	THookState savedHookState = hookState;//чтобы не использовать hookState, который может измениться в процессе
	if( state == HS_On || ( state == HS_Pause && savedHookState == HS_Off ) ) {
		hWnd = hWndMain;//
		crazyKeysMsg = crazyKeysMessage;
	}
	if( savedHookState == HS_Off ) {//если из выклключеного то надо считать опции 
		if( !loadOptions() ) {
			return false;//опции плохие, ничего не заводим
		}
	}
	if( savedHookState == HS_On ) {//выключаем хук
		stopTimer();
	}
	if( state == HS_On ) {//включаем хук
		startTimer();
	}
	sendInpMgr.SendInputScrollKey( state == HS_On );//тут hookState может смениться
	hookState = state;
	return true;
}

bool CDllHookManager::loadOptions()
{
	COptions options;
	if( !options.Load() ) {//если в опциях ошибки
		return false;
	}
	keysUpAndDownCounters.SetValue( 0 );//обнуляем наши счётчики
	scanCodeToSourceLowLetter.CopyFrom( options.GetLayout( T_Source )->GetScanCodeToWcharMap() );//раскладка исходной клавы
	sendInpMgr.SetOptions( options.GetLayout( T_Target )->GetWcharToScanCodeMap(), options.GetSleepTime(), options.IsUnicode() );
	konAvt.BuildTranslitKA( options.GetRules() );//строим конавт
	blockSwitchKey = options.GetBlockSwitchKey();
	switchKey = options.GetSwitchKey();
	delayTime = options.GetDelayTime();
	timerTime = options.GetTimerTime();
	return true;
}

void CDllHookManager::changeHookState( THookState state )
{//переключатель между HS_Pause и HS_On
	presume( state == HS_On || state == HS_Pause );
	presume( hookState == HS_On || hookState == HS_Pause );
	presume( state != hookState ); 
	if( hookState == state ) {
		return;
	}
	hookState = state;
	if( state == HS_On ) {
		SendMessage( hWnd, crazyKeysMsg, WM_FROM_DLL_CHANGE_ICON, HS_On );
		startTimer();
		isSysemVkCodeDown.SetValue( false );//сбрасываем зажатые клавиши
		nSystemVkCodesDown = 0;//и счётчик зажатых клавиш
	} else {
		SendMessage( hWnd, crazyKeysMsg, WM_FROM_DLL_CHANGE_ICON, HS_Pause );
		stopTimer();
	}	
}

bool IsFirstVKeyInSameSC( BYTE scanCode, unsigned char vkCode ) 
{
	static BYTE scanCodeSaved = 0;
	static unsigned char vkCodeSaved = 0;
	if( scanCode != scanCodeSaved || vkCode == vkCodeSaved ) {
		scanCodeSaved = scanCode;
		vkCodeSaved = vkCode;
		return true;
	}
	return false;
}

bool CDllHookManager::OnProcMouseHook(const PMSLLHOOKSTRUCT& hookStruct, WPARAM wParam)
{
	if (wParam == WM_MBUTTONUP && GetAsyncKeyState(VK_SHIFT)) // Shift + Middle mouse click
	{
		changeHookState((hookState == HS_On) ? HS_Pause : HS_On);
		// return true; // не будем перехватывать сочетание - вдруг, оно используется где-то еще!
	}
	return false;
}

bool CDllHookManager::OnProcHook( const PKBDLLHOOKSTRUCT& hookStruct, WPARAM wParam )
{
	unsigned char vkCode = (unsigned char)hookStruct->vkCode;

	if( vkCode == switchKey ) {//переключатель
		if( wParam == WM_KEYUP ) {//нажат вниз, переключаем
			changeHookState( ( hookState == HS_On ) ? HS_Pause : HS_On );
		}
		return blockSwitchKey;
	}

	if( hookState != HS_On ) {//сейчас они нам не нужны
		return false;
	}

	if( vkCode == VK_CAPITAL ) {//Caps ни от чего не зависит
		isCapsDown = ( wParam == WM_KEYDOWN );//и за ним необходимо следить
		return false;
	}
	if( vkCode == VK_LSHIFT || vkCode == VK_RSHIFT ) {//с шифтом полная беда
		isShiftDown = ( wParam == WM_KEYDOWN );//и за ним необходимо следить
		return false;//отпускаем, т.к. притормозить отпускание не реально!
	}

	if( wParam == WM_KEYUP && 0 < keysUpAndDownCounters[vkCode] ) {//если это буква, парную которой мы проглатили
		keysUpAndDownCounters[vkCode]--;
		return true;//это отпускание клавиши которыю мы кушали, его кушаем для пары
	}

	if( isSysemVkCode[vkCode] ) {//если клавиша системная, за которыми надо следить
		if( wParam == WM_KEYDOWN && !isSysemVkCodeDown[vkCode] ) {//на нажатие, но не на repeat
			isSysemVkCodeDown[vkCode] = true;
			nSystemVkCodesDown++;
		}
		if( wParam == WM_KEYUP && isSysemVkCodeDown[vkCode] ) {//на первое отпускание
			isSysemVkCodeDown[vkCode] = false;
			nSystemVkCodesDown--;
		}
		return false;//системные клавиши пропускаем
	}
	if( 0 < nSystemVkCodesDown ) {//если зажата системная клавиша
		return false;//то всё последующее пропускаем мимо...
	}

	if( hookStruct->flags % 2 != 0 ) {//win & menu клавиши имеют флаг 1, и поэтому не пропускается доп.кнопка
		return false;//клавиша дополнительная типа pause/play, но не переключатель
	}

	BYTE scanCode = (BYTE)hookStruct->scanCode;
	wchar_t sourceLetter = scanCodeToSourceLowLetter[scanCode];//преобразовываем код в маленькие буквы
	if( vkCode == ' ' || konAvt.NeedTranslit( sourceLetter ) ) {//пробел (т.к. он должен встать в очередь а также сбросить сос.ка) или буква
		keysUpAndDownCounters[vkCode]++;
		if( IsFirstVKeyInSameSC( scanCode, vkCode ) ) {
			wcharTimeQueue.Add( hookStruct->time, sourceLetter, isShiftDown ^ isCapsDown );//добавляем в очередь {время, буква и регистр}
		}
		return true;//глотаем это сообщение
	}
	return false;
}

void CDllHookManager::OnTimer()
{
	if( wcharTimeQueue.HasNewChars() ) {//есть буквы для обработки
		DWORD timeOdds;
		wchar_t sourceLetter;
		bool isUpper;
		wcharTimeQueue.Remove( timeOdds, sourceLetter, isUpper );
        if( delayTime < timeOdds ) {//между последними нажатиями была длинная пауза
			konAvt.ResetState();//поэтому сбрасываем состояние конавта в начало
		}
		sendInpMgr.SendInputPressedKey( konAvt, sourceLetter, isUpper );//отсылаем			
	}
}
