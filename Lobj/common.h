#pragma once
#include <windows.h>

//#pragma warning( disable : 4127 )//conditional expression is constant
//#pragma warning( disable : 4100 )//unreferenced formal parameter
//Warning	3	warning C4996: '_vsnprintf': This function or variable may be unsafe. Consider using _vsnprintf_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.	e:\wdlioka\!develop\vs2003\!other2003\sequence\lobj\debuglogtrace.cpp	32
#pragma warning( disable : 4996 )

template<typename T1, typename T2 = T1>
struct CPair {
	CPair(){}
	CPair( const T1& _first, const T2& _second ) : first( _first ), second( _second ) {}
	T1 first;
	T2 second;

	bool operator<( const CPair& other ) const {
		return ( first < other.first || ( first == other.first && second < other.second ) );
	}
	bool operator==( const CPair& other ) const {
		return ( first == other.first && second == other.second );
	}
};

template<typename T>
void Swap( T& a, T& b )
{
	T tmp = a;
	a = b;
	b = tmp;
}

template<typename T>
const T& MinMax( const T& minimum, const T& val, const T& maximum ) 
{
	if( val < minimum ) {
		return minimum;
	} else if( maximum < val ) {
		return maximum;
	}
	return val;
}


template<typename T>
void ZeroData( T& data )
{
	ZeroMemory( &data, sizeof( T ) );
}

template<typename T>
void ZeroDataAndSetSize( T& data )
{
	ZeroMemory( &data, sizeof( T ) );
	data.cbSize = sizeof( T );
}

#define DISALLOW_EVIL_CONSTRUCTORS(Type) private: Type( const Type& ); Type& operator=( const Type& );

#ifdef _UNICODE
#define T(x) L ## x
#else
#define T(x) x
#endif

void MemCpy( void* target, const void* source, size_t length );
bool MemCmp( const void* p1, const void* p2, size_t length );