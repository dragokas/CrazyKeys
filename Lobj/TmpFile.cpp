#include "TmpFile.h"
#include "TmpFileBody.h"
#include "ShellPathFuncs.h"
#include "Exception.h"

CTmpFile::CTmpFile( const CString& filePath ) 
: file( filePath ) {}

CTmpFile::CTmpFile( const CString& dirName, const CString& fileName ) 
: file( MakePath( dirName, fileName ) )
{
	CreateDirectory( dirName, 0 );
}

bool CTmpFile::Load( void*& buf, int* size )
{
	try {
		file.TryToOpen();
		file.TryToRead( false );
		buf = file.GetRowBuffer();
		if( size != 0 ) {
 			*size = file.GetFileSize();
 		}
	} catch( const CException&  ) {
		return false;
	}
	return true;
}

bool CTmpFile::Save( const void* buf, int size )
{
	try {
		file.TryToCreate();
		file.TryToWriteBuffer( buf, size );
	} catch( const CException&  ) {
		return false;
	}
	return true;
}
















