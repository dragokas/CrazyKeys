#include "CrazyKeysMain.h"
#include "resource.h"
#include "Presume.h"
#include "TmpFileBody.h"
#include "OptionsDlg.h"
#include "ExeHookManager.h"
#include "Taskbar.h"
#include "MUI.h"
#include "CrazyKeysMUI.h"
#include "CrazyKeysConst.h"
#include "Exception.h"

const wchar_t CrazyKeysClassName[] = L"CrazyKeysClassName";

static CCrazyKeysMain* pThis;

CCrazyKeysMain::CCrazyKeysMain( HINSTANCE hInstance ) 
: hInst( hInstance ), hookManager( 0 ), taskbar( 0 ), hMutex( 0 ), isInOptionsDialog( false )
{}

CCrazyKeysMain::~CCrazyKeysMain() 
{
	delete taskbar; 
	delete hookManager;
	if( hMutex != 0 ) {
		CheckZero( CloseHandle( hMutex ) );
	}
}

bool CCrazyKeysMain::Init()
{
	if( 0x80000000 <= GetVersion() ) {//если мы под 98 или раньше, то увы :|
		MessageBox( 0, TranslateStr(T("This program can be run only on windows NT/2000/XP/2003\n\
			but not on windows 3.1/95/98/Millennium")), T("Error"), 0 );
		return false;
	}
	hMutex = CreateMutex( 0, FALSE, L"TransCrazyKeys" );
	CheckZero( hMutex );
	if( GetLastError() == ERROR_ALREADY_EXISTS ) {
		MessageBox( 0, TranslateStr(T("The program is already running, another copy can not be run")), 
			TranslateStr(T("Error")), 0 );
		return false;
	}
	if( !createFilesFromResources( hInst ) ) {
		CheckZero( CloseHandle( hMutex ) );
		MessageBox( 0, TranslateStr(T("Problems with resource files")), 
			TranslateStr(T("Internal error")), 0 );
		return false;
	}
	createMainWindow();//создаём главное окно
	
	taskbar = new CTaskbar( hInst, hWnd );
	if( !taskbar->Init() ) {
		return false;
	}
	
	hookManager = new CЕхеHookManager( hWnd );
	setState( HS_Pause );
	pThis = this;
	return true;
}

void CCrazyKeysMain::createMainWindow()
{
	WNDCLASS w;
	ZeroMemory( &w, sizeof( WNDCLASS ) );
	w.lpfnWndProc = MainWndProc;
	w.hInstance = hInst;  
	w.lpszClassName = CrazyKeysClassName;  
	CheckZero( RegisterClass( &w ) );
	hWnd = CreateWindow( CrazyKeysClassName, CrazyKeysProgramName, WS_OVERLAPPEDWINDOW, 0, 0, 0, 0, 0, 0, hInst, 0 );
	CheckZero( hWnd );
}

LRESULT CALLBACK MainWndProc( HWND hWnd, UINT imsg, WPARAM wParam, LPARAM lParam )
{
	if( pThis != 0 ) {
		return pThis->mainWndProc( hWnd, imsg, wParam, lParam );
	}
	return DefWindowProc( hWnd, imsg, wParam, lParam );
}

LRESULT CCrazyKeysMain::mainWndProc( HWND hWnd, UINT imsg, WPARAM wParam, LPARAM lParam )
{
	if( taskbar != 0 && imsg == taskbar->GetTaskbarWM() && (UINT)lParam == WM_RBUTTONUP ) {//сообщение для таскбара
		if( !isInOptionsDialog ) {
			taskbar->ShowMenu( hookManager->GetHookState() );
		} else {
			HWND hDlg = COptionsDlg::GetLastCreatedDlg();
			if( hDlg != 0 ) {
				SetForegroundWindow( hDlg );
			}
		}
		return 0;
	}
	if (taskbar != 0 && imsg == taskbar->GetTaskbarWM() && (UINT)lParam == WM_LBUTTONUP) {//сообщение для таскбара
		if (!isInOptionsDialog) {
			THookState hookState = hookManager->GetHookState();
			if (hookState != HS_Off)
			{
				hookState = (hookState == HS_On) ? HS_Pause : HS_On;
				setState(hookState);
			}
		}
		return 0;
	}
	if( hookManager != 0 && imsg == hookManager->GetCrazyKeysMsg() ) {//сообщение хука от длл-ки или из диалога
		if( wParam == WM_FROM_DLL_CHANGE_ICON ) {
			taskbar->SetIconColor( (THookState)lParam );
			hookManager->OnHookStateIsChanged( (THookState)lParam );
		} 
		return 0;
	}
	switch( imsg ) {
		case WM_DESTROY:
			taskbar->SetIconColor( HS_NoIcon );//убиваем иконку до того как убили окно
			PostQuitMessage( 0 );
			return 0;
		case WM_COMMAND: onMenuWM_Command( wParam ); break;
	}
	return DefWindowProc( hWnd, imsg, wParam, lParam );
}

void CCrazyKeysMain::onMenuWM_Command( WPARAM wParam )
{
	switch( LOWORD( wParam ) ) {
		case ID_TRANS_HELP: ShowHelpTopic( hWnd ); break;
		case ID_TRANS_OPTIONS: showOptions(); break;	
		case ID_TRANS_RUN: 
			if( !setState( HS_On ) ) {//мало ли почему не смогли начать
				showOptions();//показываем опции
			} break;	
		case ID_TRANS_PAUSE: 
			if( !setState( HS_Pause ) ) {//если не смогли снять с Off, значит беда с опциями 
				showOptions();//показываем опции
			} break;	
		case ID_TRANS_STOP: setState( HS_Off ); break;	
		case ID_TRANS_EXIT: setState( HS_Off );	CheckZero( DestroyWindow( hWnd ) ); break;		
	}
}

bool CCrazyKeysMain::setState( THookState state )
{
    presume( hookManager != 0 && taskbar != 0 );
	if( state != hookManager->GetHookState() ) {
		bool res = hookManager->SetHookState( state );//может и не поставится из-за опций
		taskbar->SetIconColor( hookManager->GetHookState() );
		return res;
	}
	return true;
}

void CCrazyKeysMain::showOptions()
{
	isInOptionsDialog = true;
	COptionsDlg optionsDlg( hInst );
	THookState savedState = hookManager->GetHookState();
	setState( HS_Off );
	optionsDlg.ShowDlg( hWnd, hookManager->GetCrazyKeysMsg() );
	setState( savedState );
	isInOptionsDialog = false;
}

bool CCrazyKeysMain::createFilesFromResources( HINSTANCE hInst )
{
	return ( //под DEBUG будет подгружаться длл-ка DEBUG аналогично с RELEASE
		createFileFromResources( hInst, L"IDR_CRAZYKEYS_DLL", HookDllFileName ) &&
		createFileFromResources( hInst, L"IDR_CRAZYKEYS_RULES", CfgDefaultRulesFileName ) && 
		createFileFromResources( hInst, L"IDR_CRAZYKEYS_HELP", HelpFileName ) );	
}

bool CCrazyKeysMain::createFileFromResources( HINSTANCE hInst, const wchar_t* resourceName, const wchar_t* fileName )
{
	HRSRC hRes = FindResourceEx( hInst, L"CRAZYKEYSFILE", resourceName, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ) );
	CheckZero( hRes );
	HGLOBAL hGlobal = LoadResource( hInst, hRes );
	CheckZero( hGlobal );
	DWORD fSize = SizeofResource( hInst, hRes );
	char* pBuffer = (char *)LockResource( hGlobal );
	try {
		CTmpFileBody file( fileName );
		file.TryToCreate();
		file.TryToWriteBuffer( pBuffer, fSize );
	} catch( CException& e ) {
		e.ShowMessage();
	}
	FreeResource( hGlobal );
	return ExistFile( fileName );
}












