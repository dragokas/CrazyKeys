#include "SetupDlg.h"
#include <shlobj.h>
#include "Presume.h"
#include "Path.h"
#include "ShellFuncs.h"
#include "ExeHookManager.h"
#include "TmpFileBody.h"
#include "resource.h"
#include "resource.hm"
#include "MUI.h"
#include "CrazyKeysMUI.h"
#include "CrazyKeysConst.h"
#include "MsgBox.h"

void CSetupDlg::ShowDlg()
{
	CreateModalDlg( IDD_SETUP_DLG, 0 );
}

BOOL CSetupDlg::OnInitDlg()
{
	TranslateCrazyKeysDlg( hDlg );//локализируем диаложку
	CString copyToStr;
	copyToStr = GetItemText( IDC_RADIO_COPY_TO_PROGRAMFILES, MAX_PATH );
	SetDlgItemText( hDlg, IDC_RADIO_COPY_TO_PROGRAMFILES, copyToStr + GetFolderPath( CSIDL_PROGRAM_FILES ) );  
	copyToStr = GetItemText( IDC_RADIO_COPY_TO_APPDATA, MAX_PATH );
	SetDlgItemText( hDlg, IDC_RADIO_COPY_TO_APPDATA, copyToStr + getAppDataShortName() );  
	if( IsUserAnAdmin() == FALSE ) {
		EnableWindow( GetDlgItem( hDlg, IDC_RADIO_COPY_TO_PROGRAMFILES ), FALSE );
		CheckRadioButton( hDlg, IDC_RADIO_COPY_TO_PROGRAMFILES, IDC_RADIO_CURRENTDIR, IDC_RADIO_COPY_TO_APPDATA );
	} else {
#ifdef _DEBUG	
		CheckRadioButton( hDlg, IDC_RADIO_COPY_TO_PROGRAMFILES, IDC_RADIO_CURRENTDIR, IDC_RADIO_CURRENTDIR );
#else
		CheckRadioButton( hDlg, IDC_RADIO_COPY_TO_PROGRAMFILES, IDC_RADIO_CURRENTDIR, IDC_RADIO_COPY_TO_PROGRAMFILES );
#endif		
	}
#ifdef _DEBUG
	CheckDlgButton( hDlg, IDC_CHECK_DESKTOP, BST_UNCHECKED );
	CheckDlgButton( hDlg, IDC_CHECK_STARTUP, BST_UNCHECKED );
#else
	CheckDlgButton( hDlg, IDC_CHECK_DESKTOP, BST_CHECKED );
	CheckDlgButton( hDlg, IDC_CHECK_STARTUP, BST_CHECKED );
#endif	
	return TRUE;
}

BOOL CSetupDlg::OnHelpDlg( HELPINFO* helpInfo )
{
	ShowHelpTopic( hDlg, ( helpInfo->dwContextId == 0 ) ? HIDHELP_SETUP_DLG : helpInfo->dwContextId );
	return TRUE;
}

BOOL CSetupDlg::OnCommandDlg( WPARAM wParam, LPARAM /*lParam*/ )
{
	switch( LOWORD( wParam ) ) {
		case IDHELP_SETUP_DLG:
			ShowHelpTopic( hDlg, HIDHELP_SETUP_DLG );
			return TRUE;
		case IDOK: 
			if( onOK() ) {
				EndDialog( hDlg, LOWORD( wParam ) );
			} else {
				MsgBox( L"Error", TranslateStr( T("Can't write to:\n%s") ), getTargetProgramDir().GetBuffer() );
			}
			return TRUE;
		case IDCANCEL:
			isCancelSelected = true;
			EndDialog( hDlg, LOWORD( wParam ) );
			return TRUE;
	}
	return FALSE;
}

bool CSetupDlg::onOK()
{
	needToMoveToProgramFiles = ( IsDlgButtonChecked( hDlg, IDC_RADIO_COPY_TO_PROGRAMFILES ) == BST_CHECKED );
	needToMoveToApplicationData = ( IsDlgButtonChecked( hDlg, IDC_RADIO_COPY_TO_APPDATA ) == BST_CHECKED );
	if( needToMoveToProgramFiles || needToMoveToApplicationData ) {
		presume( needToMoveToProgramFiles != needToMoveToApplicationData );
		if( !copyToDir() ) {//если не удачно скопировали
			return false;
		}	
	} else if( !hasCurDirWriteAccess() ) {//если в текущей папке нельз€ создавать файлы 
		return false;
	}
	//путь пребывани€ проги уже выставлен, и можно делать линки
	if( IsDlgButtonChecked( hDlg, IDC_CHECK_DESKTOP ) == BST_CHECKED ) {
		createShortcut( GetFolderPath( CSIDL_DESKTOPDIRECTORY ) );
	}
	if( IsDlgButtonChecked( hDlg, IDC_CHECK_STARTUP ) == BST_CHECKED ) {
		createShortcut( GetFolderPath( CSIDL_STARTUP ) );
	}
	return true;
}

bool CSetupDlg::IsFirstTimeRun()
{//не в или нет р€дом —¬≈∆≈… dll-ки, значит запуск первый раз
	if( GetCurrentDirectory().Find( GetFolderPath( CSIDL_PROGRAM_FILES ) ) == 0 ||
		GetCurrentDirectory().Find( GetFolderPath( CSIDL_APPDATA ) ) == 0 )
    {
		return false;
	}
	if( !ExistFile( HookDllFileName ) ) {
		return true;
	}
	CFilePath dllFile( HookDllFileName );
	CFilePath exeFile( GetCurrentFileName() );
	FILETIME dllFileTime = dllFile.GetLastWriteTime();//врем€ dll, т.к. м.б. она устарела
	FILETIME exeFileTime = exeFile.GetLastWriteTime();//дл€ данного exe, т.е. не из этого exe
	return ( CompareFileTime( &dllFileTime, &exeFileTime ) < 0 );
}

CString CSetupDlg::getAppDataShortName() const
{
	CString fullStr = GetFolderPath( CSIDL_APPDATA );
	CString userName = GetUserName();
	int pos = fullStr.Find( userName );
	CString appData( fullStr.GetBuffer() + pos + userName.Length() + 1 );
	return userName + L"'s " + appData;
}

CString CSetupDlg::getTargetProgramDir() const
{ 
	if( !needToMoveExe() ) {
		return GetCurrentDirectory();
	}
	return GetFolderPath( needToMoveToApplicationData ? CSIDL_APPDATA : CSIDL_PROGRAM_FILES ) + L"\\" + CrazyKeysProgramName;
}

CString CSetupDlg::getTargetProgramPath() const
{
	if( !needToMoveExe() ) {
		return GetCurrentFileName();
	}
	return getTargetProgramDir() + L"\\" + CrazyKeysProgramName + L".exe";
}

bool CSetupDlg::hasCurDirWriteAccess() const
{
	CString sourcePath = GetCurrentFileName();//текущее местоположение
	CString targetPath = getTargetProgramDir()+L"\\CrazyKeystmp.tmp";//куда надо будет поместить
	BOOL copyRes = CopyFile( sourcePath, targetPath, FALSE );
	BOOL delRes = DeleteFile( targetPath );
	return ( copyRes == TRUE && delRes == TRUE );
}

bool CSetupDlg::copyToDir()
{
	presume( needToMoveExe() );
	CString sourcePath = GetCurrentFileName();//текущее местоположение
	CString targetPath = getTargetProgramPath();//куда надо будет поместить
	CreateDirectory( getTargetProgramDir(), 0 );//создаЄм папку куда будем копировать
	return ( CopyFile( sourcePath, targetPath, FALSE ) == TRUE );//копируем в нужное место
}

void CSetupDlg::Restart() const
{//запускаем тот exe который скопировали в program files
	STARTUPINFO si;
	ZeroData( si );
	si.cb = sizeof( si );
	PROCESS_INFORMATION pi;	
	ZeroMemory( &pi, sizeof(pi) );
	CheckZero( CreateProcess( getTargetProgramPath(), 0, 0, 0, FALSE, 0, 0, getTargetProgramDir(), &si, &pi ) ); 
	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );
}

void CSetupDlg::createShortcut( const CString& linkDir ) const
{//создание €рлыка через COM
	if( CoInitialize( 0 ) == S_OK ) {//инициализируем COM
		IShellLink* pLink = 0;
		IPersistFile* pLinkFile = 0;
		if( SUCCEEDED( CoCreateInstance( CLSID_ShellLink, 0, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&pLink) ) &&
			SUCCEEDED( pLink->SetWorkingDirectory( getTargetProgramDir() ) ) &&
			SUCCEEDED( pLink->SetIconLocation( getTargetProgramPath(), 0 ) ) &&
			SUCCEEDED( pLink->SetPath( getTargetProgramPath() ) ) &&
			SUCCEEDED( pLink->SetArguments( L"" ) ) &&
			SUCCEEDED( pLink->SetHotkey( 0 ) ) &&
			SUCCEEDED( pLink->SetShowCmd( SW_SHOWNORMAL ) ) &&
			SUCCEEDED( pLink->QueryInterface( IID_IPersistFile, (LPVOID*)&pLinkFile ) ) )
		{
			pLinkFile->Save( linkDir + L"\\" + CrazyKeysProgramName + L".lnk", TRUE ); 
            pLinkFile->Release(); 
		}
		CoUninitialize();//деинициализируем COM
	}
}



