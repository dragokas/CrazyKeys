#pragma once
#include <windows.h>
#include "Str.h"
#include "MsgBox.h"

#define Undefined_Error_Str T("Undefined error")
#define Unknown_Argument_Str T("Unknown argument")
#define Exeption_Head_Str T("Exeption")

class CException {
public:
	CException( CString errorDescriptionStr = Undefined_Error_Str, CString arg = Unknown_Argument_Str ) 
		: message(1024) {
		message = errorDescriptionStr + T("\n") + arg + T("\n") + GetLastErrorStr( GetLastError() );
	}	
	
	void ShowMessage() const {
		MessageBox( 0, message, Exeption_Head_Str, 0 );
	}
	
	void ShowMessageInDebug() const {
#ifdef _DEBUG 
		MessageBox( 0, message.GetBuffer(), Exeption_Head_Str, 0 );
#endif
	}

	CString message;
	DWORD lastError;
};


