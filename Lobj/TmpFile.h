#pragma once
#include "Str.h"
#include "TmpFileBody.h"

class CTmpFile {//хранилище в фале
public:
	explicit CTmpFile( const CString& filePath );
	CTmpFile( const CString& dirName, const CString& fileName );	
	bool Save( const void* buf, int size );
	bool Load( void*& buf, int* size = 0 );

private:
	CTmpFileBody file;
};

