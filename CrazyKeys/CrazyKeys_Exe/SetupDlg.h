#pragma once
#include <windows.h>
#include "Str.h"
#include "Dialog.h"

class CSetupDlg : public CDialog {
public:
	CSetupDlg( HINSTANCE hInstance ) : CDialog( hInstance ), 
		needToMoveToProgramFiles(false), needToMoveToApplicationData( false ), isCancelSelected(false) {}

	static bool IsFirstTimeRun();//запускается ли программа в первый раз
	
	void ShowDlg();
	bool NeedToRestart() const { return needToMoveExe(); };
	bool NeedToExit() const { return ( needToMoveExe() || isCancelSelected ); };
	void Restart() const;

protected:
	virtual BOOL OnInitDlg();
	virtual BOOL OnHelpDlg( HELPINFO* helpInfo );
	virtual BOOL OnCommandDlg( WPARAM wParam, LPARAM lParam );

private:
	bool onOK();
   
	CString getTargetProgramDir() const;
	CString getTargetProgramPath() const;

	CString getAppDataShortName() const;

	bool needToMoveExe() const { return ( needToMoveToProgramFiles || needToMoveToApplicationData ); }
	bool hasCurDirWriteAccess() const;
	bool copyToDir();
	void createShortcut( const CString& linkDir ) const;

	bool needToMoveToProgramFiles;//надо скопироваться в ProgramFiles
	bool needToMoveToApplicationData;//надо скопироваться в ProgramFiles
	bool isCancelSelected;//выбран cancel в диалоге
};