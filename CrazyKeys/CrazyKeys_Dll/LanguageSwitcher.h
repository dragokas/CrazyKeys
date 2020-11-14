#pragma once
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include "Str.h"

class CLanguageSwitcher {
public:
	CLanguageSwitcher() : isSwitchedTo(false), isInited(false) {};
	
	void Load();
	void SwitchTo( bool isUpper );
	void SwitchBack( bool isUpper );

private:
	struct CSwitcher {
		struct CSequence {
			INPUT input[6];
			int size;
			void initWithShift( WORD wSysKey, char vChar );
			void initWithoutShift( WORD wSysKey, char vChar );
		} withShift, withoutShift;
	} back, to; 
	
	void initSwitcher( const CString& str, CSwitcher& switcher );
	
	bool isInited;
	bool isSwitchedTo;

	DISALLOW_EVIL_CONSTRUCTORS( CLanguageSwitcher );
};
