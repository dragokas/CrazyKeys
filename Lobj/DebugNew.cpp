#include <stdio.h>
#include <malloc.h>

#include "DebugNew.h"

#pragma warning( disable : 4996 )//fopen, printf...

#ifdef _DEBUG

static const char MemFileName[] = "MemFileName2.txt";

struct CAllocCount {
	CAllocCount() : allocCount( 0 ) { deleteLogFile(); }
	
	CAllocCount& operator++() { allocCount++; return *this; }
	CAllocCount& operator--() { allocCount--; return *this; }
	
	~CAllocCount() {
		FILE* stream = fopen( MemFileName, "a+" );
		fprintf( stream, "\n\n\t!!! %d bloks of memory back!!!", allocCount );
		fclose( stream );
	}

private:
	void deleteLogFile() {
		FILE* stream = fopen( MemFileName, "w" );
		fclose( stream );
	}
	int allocCount;
} AllocCount;

void TraceNewCall( void* p, size_t size, const char* fName, int nLine, bool isArray )
{
	FILE* stream = fopen( MemFileName, "a+" );
	fprintf( stream, "new%s 0x%08X : size = %d file = \n%s : %d \n\n", (isArray) ? "[]" : "",
		(int)(reinterpret_cast<__int64>(p)), size, fName, nLine );
	fclose( stream );
}

void TraceDeleteCall( void* p, const char* fName, int nLine, bool isExeption, bool isArray )
{
	FILE* stream = fopen( MemFileName, "a+" );
	fprintf( stream, "delete%s 0x%08X : file = \n%s : %d \n\n", (isExeption && isArray) ? "[]" : (!isExeption) ? "?" : "",
		(int)(reinterpret_cast<__int64>(p)), fName, nLine );
	fclose( stream );
}

void TraceDelete( const char* fName, int nLine )
{
	--AllocCount;
	TraceDeleteCall( 0, fName, nLine, false, false );
}
//////////////////////////////////////////////////////////////////////////
void* LObjNew( size_t size ) { ++AllocCount; return malloc( size ); }
void LObjDelete( void* p ) { --AllocCount; free(p); }

void * operator new ( size_t size, const char* fName, int nLine )
{
	void* p = LObjNew( size );
	TraceNewCall( p, size, fName, nLine, false );
	return p;
}

void * operator new[] ( size_t size, const char* fName, int nLine )
{
	void* p = LObjNew( size );
	TraceNewCall( p, size, fName, nLine, true );
	return p;
}

void operator delete ( void *p, const char* fName, int nLine )
{
	TraceDeleteCall( p, fName, nLine, true, false );
	LObjDelete( p );
}

void operator delete[] ( void *p, const char* fName, int nLine )
{
	TraceDeleteCall( p, fName, nLine, true, true );
	LObjDelete( p );
}

/*
void operator delete ( void *p )
{
	TraceDeleteCall( p, "", 0, false, false );
	LObjDelete( p );
}

void operator delete[] ( void *p )
{
	TraceDeleteCall( p, "", 0, false, true );
	LObjDelete( p );
}
*/

#endif









