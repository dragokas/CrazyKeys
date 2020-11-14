#include "Archive.h"

#include "Str.h"
#include "TmpFileBody.h"
#include "Queue.h"
#include "Exception.h"

#include "StartDebugNew.h"

CArchive::CArchive( const CString& fileName, TArhiveDirection archiveDirection )
: direction( archiveDirection )
{
	pOutputRecs = 0;
	pTmpFile = new CTmpFileBody( fileName );
	if( IsReading() ) {
		pTmpFile->TryToOpen();
		pTmpFile->TryToRead( false );
		inputRec.Set( pTmpFile->GetRowBuffer(), pTmpFile->GetFileSize() );
	} else {
		pTmpFile->TryToCreate();
	}
}

CArchive::~CArchive()
{
	Flush();
	delete pTmpFile;
	delete pOutputRecs;
}

void CArchive::Flush()
{
	if( IsReading() || pOutputRecs == 0 ) {
		return;
	}
	while( 0 < pOutputRecs->Size() ) {
		CRec& rec = pOutputRecs->First();
		pTmpFile->TryToWriteBuffer( rec.pBuffer, rec.pos, true );
		delete[] rec.pBuffer;
		pOutputRecs->PopFromHead();
	}
}

bool CArchive::IsEndOfFile() const
{
	presume( IsReading() ); 
	return ( inputRec.pos == inputRec.size );
}

void CArchive::Read( void* buffer, int typeSize, const char* typeName )
{
	int typeNameLenght = StrLen( typeName );//настоящая длина имени типа
	int checkTypeNameLenght = -1;//длина имени типа
	read( &checkTypeNameLenght, sizeof( int ) );
	if( checkTypeNameLenght != typeNameLenght ) {
		throw CException( T("Arhive is broken in serialize READ"), 
			CString( T("Different lenght of type name : ") ) + GetStr( typeName ) );
	}
	char checkTypeName[256] = {0};//имя типа
	read( &checkTypeName, typeNameLenght );
	if( StrCmp( checkTypeName, typeName ) != 0 ) {
		throw CException( T("Arhive is broken in serialize READ"), 
			CString( T("Different type name : ") ) + GetStr( checkTypeName ) + T(" =/= ") + GetStr( typeName ) );
	}
	int checkTypeSize = -1;//размер типа
	read( &checkTypeSize, sizeof( int ) );
	if( checkTypeSize != typeSize ) {
		throw CException( T("Arhive is broken in serialize READ"), 
			CString( T("Different size of : ") ) + GetStr( typeName ) );
	}
	read( buffer, typeSize );
}

void CArchive::read( void* valBuffer, int valSize ) 
{
	presume( IsReading() );
	if( inputRec.pos + valSize <= inputRec.size ) {
		MemCpy( valBuffer, inputRec.pBuffer + inputRec.pos, valSize );
		inputRec.pos += valSize;
	} else {
		throw CException( T("End of file in serialize READ") );
	}
}

void CArchive::Write( const void* buffer, int typeSize, const char* typeName )
{
	int typeNameLenght = StrLen( typeName );
	write( &typeNameLenght, sizeof( int ) );//длина имени типа
	write( typeName, typeNameLenght );//имя типа
	write( &typeSize, sizeof( int ) );//размер типа
	write( buffer, typeSize );//данные у типа
}

void CArchive::write( const void* valBuffer, int valSize ) 
{
	if( pOutputRecs == 0 ) {
		pOutputRecs = new CList<CRec>;
	}
	if( pOutputRecs->Size() == 0 || pOutputRecs->Last().pos + valSize > pOutputRecs->Last().size ) {
		pOutputRecs->PushToTail( CRec() );
		int bufferSize = max( 16, valSize );
		pOutputRecs->Last().Set( new char[bufferSize], bufferSize );
	}
	CRec& rec = pOutputRecs->Last();
	MemCpy( rec.pBuffer + rec.pos, valBuffer, valSize );
	rec.pos += valSize;
}




















