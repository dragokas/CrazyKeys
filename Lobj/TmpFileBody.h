#pragma once
#include "Str.h"
#include "Ptr.h"

#pragma warning( disable : 4290 )

class CException;

class CTmpFileBody {
public:
	explicit CTmpFileBody( const CString& _fileName );
	~CTmpFileBody();

	int GetFileSize() { return fileSize; }
	
	void TryToCreate() throw(CException);
	void TryToOpen() throw(CException);
	void TryToGetFileInfo() throw(CException);
	void TryToRead( bool isTextFile = true ) throw(CException);

	char* GetRowBuffer() { return pBuffer; }
	
	void ReceiveBuffer( CStr<char>& str ) throw(CException);
	void ReceiveBuffer( CStr<wchar_t>& str ) throw(CException);

	void TryToWriteBufferStr( CStr<wchar_t> str, bool append = false ) throw(CException);
	void TryToWriteBufferStr( CStr<char> str, bool append = false ) throw(CException);
	void TryToWriteBuffer( const void* buf, DWORD size, bool append = false ) throw(CException);

private:
	template<typename T>
	void setStr( CStr<T>& str, T* buffer, int strLength ); 

	CString fileName;
	HANDLE hFile;
	int fileSize;
	char* pBuffer;
	CStr<char> charStr;
	CStr<wchar_t> wcharStr;

	DISALLOW_EVIL_CONSTRUCTORS( CTmpFileBody );
};

int GetFileLength( CString fileName );
bool ExistFile( CString fileName );

/*
void RecieveStandartLineSeparators( CList< CStr< wchar_t > >& lineBreaks );
void RecieveStandartLineSeparators( CList< CStr< char > >& lineBreaks );
*/