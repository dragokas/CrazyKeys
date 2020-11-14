#pragma once
#include <wtypes.h>
#include <windows.h>
#include <typeinfo.h>
//сюда никаких инцлудов нельзя

enum TArhiveDirection { AD_Read, AD_Write };

template <typename CharType> class CStr;
typedef CStr<TCHAR> CString;
template <typename T1> class CList;
class CTmpFileBody;

class CArchive {
public:
	CArchive( const CString& fileName, TArhiveDirection archiveDirection );
	~CArchive();

 	bool IsReading() const { return ( direction == AD_Read ); }
 	bool IsWriting() const { return ( direction == AD_Write ); }

	void Flush();
	bool IsEndOfFile() const; 

	template<typename T1> void Write( const T1& value ) { Write( &value, sizeof( T1 ), typeid( T1 ).name() ); }
	void Write( const void* buffer, int typeSize, const char* typeName );

	template<typename T1> void Read( T1& value ) { Read( &value, sizeof( T1 ), typeid( T1 ).name() ); }
	void Read( void* buffer, int typeSize, const char* typeName );

private:
	void write( const void* valBuffer, int valSize );
	void read( void* valBuffer, int valSize );

	struct CRec {
		CRec() : pBuffer( 0 ), size( 0 ), pos( 0 ) {}
		CRec( char* _pBuffer, int _size, int _pos = 0 ) { Set( _pBuffer, _size, _pos ); }
		void Set( char* _pBuffer, int _size, int _pos = 0 ) {
			pBuffer = _pBuffer; size = _size; pos = _pos;
		}
		char* pBuffer;
		int size;
		int pos;
	} inputRec;
	
	CList<CRec>* pOutputRecs;//т.к. классы шаблонные, и сами имеют архивацию
	CTmpFileBody* pTmpFile;//и включают в себя архив
	TArhiveDirection direction;
};



// template<typename T1> 
// void CArchive::Read( T1& value ) 
// { 
// 	const TCHAR* typeName = typeid( T1 ).name();
// 	int typeNameLen = 0;
// 	for( int typeNameLen = 0; typeName[typeNameLen] != 0; typeNameLen++ );
// 	int typeSize = sizeof( T1 );
// 	Read( &value, sizeof( T1 ) ); 
// }
// 
// template<typename T1> 
// void CArchive::Write( const T1& value )
// {
// 	const TCHAR* typeName = typeid( T1 ).name();
// 	int typeNameLen = 0;
// 	for( int typeNameLen = 0; typeName[typeNameLen] != 0; typeNameLen++ );
// 	int typeSize = sizeof( T1 );
// 	Write( &value, sizeof( T1 ) ); 
// }


template<typename T1> 
void Serialize( CArchive& archive, T1& value )
{
	if( archive.IsReading() ) {
		archive.Read( value );
	} else {
		archive.Write( value );
	}
}

// template<typename T1> 
// void Serialize( CArhive& archive, void* valBuffer, int valSize )
// {
// 	if( archive.IsReading() ) {
// 		archive.Read( valBuffer, valSize );
// 	} else {
// 		archive.Write( valBuffer, valSize );
// 	}
// }