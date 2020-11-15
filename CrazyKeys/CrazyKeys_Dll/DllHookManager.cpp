#include "DllHookManager.h"
#include "Str.h"
#include "Options.h"
#include "CrazyKeysConst.h"

extern VOID CALLBACK TimerProc( HWND,UINT uMsg, UINT_PTR, DWORD );

//HS_Off - ������� �� �������� � ����� �� ���������
//HS_Pause - ������� �� �������������� � �� ���������
//HS_On - �� ���������

CDllHookManager::CDllHookManager() : hookState( HS_Off ), keysUpAndDownCounters( 256, 0 ), uptrTimer( 0 ),
isSysemVkCode( 256, false ), isSysemVkCodeDown( 256, false )
{
	isCapsDown = isShiftDown = false;
	nSystemVkCodesDown = 0;//������� � ������� ������� ����� ���� ������
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
	THookState savedHookState = hookState;//����� �� ������������ hookState, ������� ����� ���������� � ��������
	if( state == HS_On || ( state == HS_Pause && savedHookState == HS_Off ) ) {
		hWnd = hWndMain;//
		crazyKeysMsg = crazyKeysMessage;
	}
	if( savedHookState == HS_Off ) {//���� �� ������������� �� ���� ������� ����� 
		if( !loadOptions() ) {
			return false;//����� ������, ������ �� �������
		}
	}
	if( savedHookState == HS_On ) {//��������� ���
		stopTimer();
	}
	if( state == HS_On ) {//�������� ���
		startTimer();
	}
	sendInpMgr.SendInputScrollKey( state == HS_On );//��� hookState ����� ���������
	hookState = state;
	return true;
}

bool CDllHookManager::loadOptions()
{
	COptions options;
	if( !options.Load() ) {//���� � ������ ������
		return false;
	}
	keysUpAndDownCounters.SetValue( 0 );//�������� ���� ��������
	scanCodeToSourceLowLetter.CopyFrom( options.GetLayout( T_Source )->GetScanCodeToWcharMap() );//��������� �������� �����
	sendInpMgr.SetOptions( options.GetLayout( T_Target )->GetWcharToScanCodeMap(), options.GetSleepTime(), options.IsUnicode() );
	konAvt.BuildTranslitKA( options.GetRules() );//������ ������
	blockSwitchKey = options.GetBlockSwitchKey();
	switchKey = options.GetSwitchKey();
	delayTime = options.GetDelayTime();
	timerTime = options.GetTimerTime();
	return true;
}

void CDllHookManager::changeHookState( THookState state )
{//������������� ����� HS_Pause � HS_On
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
		isSysemVkCodeDown.SetValue( false );//���������� ������� �������
		nSystemVkCodesDown = 0;//� ������� ������� ������
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
		// return true; // �� ����� ������������� ��������� - �����, ��� ������������ ���-�� ���!
	}
	return false;
}

bool CDllHookManager::OnProcHook( const PKBDLLHOOKSTRUCT& hookStruct, WPARAM wParam )
{
	unsigned char vkCode = (unsigned char)hookStruct->vkCode;

	if( vkCode == switchKey ) {//�������������
		if( wParam == WM_KEYUP ) {//����� ����, �����������
			changeHookState( ( hookState == HS_On ) ? HS_Pause : HS_On );
		}
		return blockSwitchKey;
	}

	if( hookState != HS_On ) {//������ ��� ��� �� �����
		return false;
	}

	if( vkCode == VK_CAPITAL ) {//Caps �� �� ���� �� �������
		isCapsDown = ( wParam == WM_KEYDOWN );//� �� ��� ���������� �������
		return false;
	}
	if( vkCode == VK_LSHIFT || vkCode == VK_RSHIFT ) {//� ������ ������ ����
		isShiftDown = ( wParam == WM_KEYDOWN );//� �� ��� ���������� �������
		return false;//���������, �.�. ������������ ���������� �� �������!
	}

	if( wParam == WM_KEYUP && 0 < keysUpAndDownCounters[vkCode] ) {//���� ��� �����, ������ ������� �� ����������
		keysUpAndDownCounters[vkCode]--;
		return true;//��� ���������� ������� ������� �� ������, ��� ������ ��� ����
	}

	if( isSysemVkCode[vkCode] ) {//���� ������� ���������, �� �������� ���� �������
		if( wParam == WM_KEYDOWN && !isSysemVkCodeDown[vkCode] ) {//�� �������, �� �� �� repeat
			isSysemVkCodeDown[vkCode] = true;
			nSystemVkCodesDown++;
		}
		if( wParam == WM_KEYUP && isSysemVkCodeDown[vkCode] ) {//�� ������ ����������
			isSysemVkCodeDown[vkCode] = false;
			nSystemVkCodesDown--;
		}
		return false;//��������� ������� ����������
	}
	if( 0 < nSystemVkCodesDown ) {//���� ������ ��������� �������
		return false;//�� �� ����������� ���������� ����...
	}

	if( hookStruct->flags % 2 != 0 ) {//win & menu ������� ����� ���� 1, � ������� �� ������������ ���.������
		return false;//������� �������������� ���� pause/play, �� �� �������������
	}

	BYTE scanCode = (BYTE)hookStruct->scanCode;
	wchar_t sourceLetter = scanCodeToSourceLowLetter[scanCode];//��������������� ��� � ��������� �����
	if( vkCode == ' ' || konAvt.NeedTranslit( sourceLetter ) ) {//������ (�.�. �� ������ ������ � ������� � ����� �������� ���.��) ��� �����
		keysUpAndDownCounters[vkCode]++;
		if( IsFirstVKeyInSameSC( scanCode, vkCode ) ) {
			wcharTimeQueue.Add( hookStruct->time, sourceLetter, isShiftDown ^ isCapsDown );//��������� � ������� {�����, ����� � �������}
		}
		return true;//������� ��� ���������
	}
	return false;
}

void CDllHookManager::OnTimer()
{
	if( wcharTimeQueue.HasNewChars() ) {//���� ����� ��� ���������
		DWORD timeOdds;
		wchar_t sourceLetter;
		bool isUpper;
		wcharTimeQueue.Remove( timeOdds, sourceLetter, isUpper );
        if( delayTime < timeOdds ) {//����� ���������� ��������� ���� ������� �����
			konAvt.ResetState();//������� ���������� ��������� ������� � ������
		}
		sendInpMgr.SendInputPressedKey( konAvt, sourceLetter, isUpper );//��������			
	}
}
