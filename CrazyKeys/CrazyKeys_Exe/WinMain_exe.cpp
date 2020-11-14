//������� ����
#include <windows.h>
#include <crtdbg.h>
#include "SetupDlg.h"
#include "CrazyKeysMain.h"
#include "Exception.h"

int WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	try {//���� ����� ��� �������� �� �� ��� �� ����������� �������
		if( CSetupDlg::IsFirstTimeRun() ) {//��� ������ �������
			CSetupDlg dlg( hInst );//���� �������, �.�. ����������� ��� �� �����������
			dlg.ShowDlg();//���������� �������� ������� �������
			if( dlg.NeedToExit() ) {//���� exe ���� ���������
				if( dlg.NeedToRestart() ) {//���� ������������� � ...
					dlg.Restart();//�� ���������� ������
				}
				return 0;
			}
		}
		CCrazyKeysMain crazyKeysMain( hInst );
		if( !crazyKeysMain.Init() ) {//�� ������ ������
			return 0;//�������� ��� ��������, (��������� ������������ ������)
		}
		MSG  msg;
		BOOL bRet;
		while( ( bRet = GetMessage( &msg, NULL, 0, 0 ) ) != 0 ) {
			if( bRet == -1 ) {//������� ����� � ���...
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
