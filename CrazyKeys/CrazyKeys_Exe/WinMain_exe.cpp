//ехешник маин
#include <windows.h>
#include <crtdbg.h>
#include "SetupDlg.h"
#include "CrazyKeysMain.h"
#include "Exception.h"

int WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	try {//если вдруг что свалится то мы это по возможности подберём
		if( CSetupDlg::IsFirstTimeRun() ) {//при первом запуске
			CSetupDlg dlg( hInst );//мини сетапер, т.к. стандартные мне не понравились
			dlg.ShowDlg();//показываем диаложку первого запуска
			if( dlg.NeedToExit() ) {//этот exe надо закончить
				if( dlg.NeedToRestart() ) {//если скопировались в ...
					dlg.Restart();//то вызываемся оттуда
				}
				return 0;
			}
		}
		CCrazyKeysMain crazyKeysMain( hInst );
		if( !crazyKeysMain.Init() ) {//не смогли начать
			return 0;//наверное уже запушены, (сообщение показывается внутри)
		}
		MSG  msg;
		BOOL bRet;
		while( ( bRet = GetMessage( &msg, NULL, 0, 0 ) ) != 0 ) {
			if( bRet == -1 ) {//виндовс сошёл с ума...
				throw CException( T( "Windows become crazy..." ), T( "GetMessage( &msg, NULL, 0, 0 ) == -1 )" ) );
			} else {
			    TranslateMessage( &msg ); 
			    DispatchMessage( &msg ); 
			}
		}
	} catch( const CException& e ) {
		e.ShowMessage();
	} catch( ... ) {
		assertFalse;
		MsgBox( T( "Unhanded exception" ), T( "Something wrong" ) );
	}
	return 0;
}
