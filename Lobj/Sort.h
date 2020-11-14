#pragma once
#include "common.h"

template<typename T1, typename T2 = T1>//класс равенства по функции равенства
class CEqFunc {
public:
	CEqFunc( bool (*pfUserEq)( const T1&, const T2& ) ) : pfEq( pfUserEq ) {}
	//равенство, которое вызывается из поиска
	template<typename T1, typename T2> bool Eq( const T1& a, const T2& b ) const { return pfEq( a, b ); }
private:
	bool (*pfEq)( const T1&, const T2& );//указатель на функцию, которая будет вызвана
};

template<typename T1, typename T2 = T1>//класс равенства по умолчанию
class CEqDefault {
public:
	template<typename T1, typename T2> bool Eq( const T1& a, const T2& b ) const { return ( a == b ); }
};

template<typename T1>//класс сравнения по функции сравнения
class CCmpFunc{
public:
	CCmpFunc( int (*pfUserCmp)( const T1&, const T1& ) ) : pfCmp( pfUserCmp ) {}
	//функция ставнения, которая вызывается из сортировки
	template<typename T1> int Cmp( const T1& a, const T1& b ) const { return pfCmp( a, b ); }
private:
	int (*pfCmp)( const T1&, const T1& );//указатель на функцию, которая будет вызвана
};

template<typename T1>//класс сравнения по умолчанию
class CCmpDefault{
public:
	template<typename T1> int Cmp( const T1& a, const T1& b ) const { return ( a < b ) ? -1 : ( b < a ) ? 1 : 0; }
};
//////////////////////////////////////////////////////////////////////////
#pragma warning( disable : 4100 )//ошибка компилятора .NET 2003
template<typename T1, typename T2 >
void BubbleSort( T1* v, int n, const T2& cmpClass )
{
	bool isSorted = false;
	for( int i = 0; !isSorted; i++ ) {
		isSorted = true;
		if( i % 2 == 0 ) {
			for( int j = i/2; j <= n -(i/2)- 2; j++ ) {
				if( cmpClass.Cmp( v[j + 1], v[j] ) < 0 ) {
					isSorted = false;
					Swap( v[j + 1], v[j] );
				}
			}
		} else {
			for( int j = n -(i/2) - 3; (i/2) <= j; j-- ) {
				if( cmpClass.Cmp( v[j + 1], v[j] ) < 0 ) {
					isSorted = false;
					Swap( v[j + 1], v[j] );
				}
			}
		}
	}
}

#pragma warning( default : 4100 )

template< typename T1, typename T2 >
void QuickSort( T1* v, int n, const T2& cmpClass )
{
	const int fewItems = 7;
	int lo = 0;
	int hi = n - 1;
	while( fewItems < hi - lo ) {
		int mid = (hi + lo) / 2;
		if( cmpClass.Cmp( v[mid], v[lo] ) < 0 ) {
			Swap( v[lo], v[mid] );
		}
		if( cmpClass.Cmp( v[hi], v[lo] ) < 0 ) {
			Swap( v[lo], v[hi] );
		}
		if( cmpClass.Cmp( v[hi], v[mid] ) < 0 ) {
			Swap( v[hi], v[mid] );
		}	
		int i = lo;
		int j = hi;	
		while( i < j ) {
			while( i < j && cmpClass.Cmp( v[i], v[mid] ) <= 0 ) {
				i++;
			}
			while( i < j && cmpClass.Cmp( v[mid], v[j] ) <= 0 ) {
				j--;
			}
			if( i < j ) {
				Swap( v[i], v[j] );
			} else {
				if( hi - i < j - lo ) {//последний кусок меньше поэтому запускаемся на нём
					QuickSort( v + i, hi - i + 1, cmpClass );
					hi = j - 1;
				} else {
					QuickSort( v + lo, j - lo + 1, cmpClass );
					lo = i;
				}
			}
		}
	}	
	BubbleSort( v + lo, hi - lo + 1, cmpClass ); 
}

template< typename T1, typename T2, typename T3 >
int BinaryFindIndex( T1* v, int n, const T3& item, const T2& cmpClass )
{
	int a = 0;
	int c = n;
	while( a < c ) {
		int b = (a + c) / 2;
		int cmpRes = cmpClass.Cmp( v[b], item ); 
		if( cmpRes == 0 ) {
			return b;
		} else if( 0 < cmpRes ) {
			c = b;
		} else {
			a = b + 1;			
		}
	}
	if( a == n ) {
		return n;
	}
	return ( 0 < cmpClass.Cmp( v[c], item ) ) ? a : c + 1;
}
