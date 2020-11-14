#pragma once
/*#include "TextFile.h"
//класс а-л€ ini файл, наследник текстового файла
//между ключЄм и значением ставитс€ пробел


/*
class CTextDataFile : public CTextFile
{
public:
	CTextDataFile( CString fileName ) : CTextFile( fileName, true ) { LoadTextLinesFromFile( false ); };
	~CTextDataFile() {};

	//если не наход€т то став€т дефолтные сначени€
	int GetValue( CString key, int dafaultValue = 0, CString comment = EmptyStr );
	CString GetValue( CString key, CString dafaultValue = EmptyStr, CString comment = EmptyStr );
	
	//замен€ют значени€, если наход€т существующие
	void SetValue( CString key, CString value, CString comment = EmptyStr );
	void SetValue( CString key, int value, CString comment = EmptyStr );

	//это мини защита, пока в отключке
	bool CheckFileSum() const { return true; };

	DISALLOW_EVIL_CONSTRUCTORS(CTextDataFile);
};
*/
