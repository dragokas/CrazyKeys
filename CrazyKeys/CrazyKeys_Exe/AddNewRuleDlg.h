#pragma once
#include <Windows.h>
#include "Str.h"
#include "Dialog.h"

class CAddNewRuleDlg : public CDialog { 
public:
	CAddNewRuleDlg( HINSTANCE hInstance );
	void ShowDlg( HWND hParentDlg, HKL sourceKbHKL, HKL targetKbHKL );
	void ShowDlg( HWND hParentDlg, CString sourceStr, HKL targetKbHKL );

	bool IsRuleAdded() const { return ( !source.IsEmpty() && !target.IsEmpty() ); };
	CString GetSource() const { return source; };
	CString GetTarget() const { return target; };

protected:
	virtual BOOL OnInitDlg();
	virtual BOOL OnCommandDlg( WPARAM wParam, LPARAM lParam );

private:
	CString source;
	CString target;

	HKL sourceHKL;
	HKL targetHKL;

	HKL savedHKL;

/*	INT_PTR CALLBACK addNewRuleDlgProc( HWND hDialog, UINT uMsg, WPARAM wParam );
	friend INT_PTR CALLBACK AddNewRuleDlgProc( HWND hDialog, UINT uMsg, WPARAM wParam, LPARAM lParam );
	*/
};
