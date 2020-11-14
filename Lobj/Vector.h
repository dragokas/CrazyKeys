#pragma once
#include "StopDebugNew.h"
#include <new>

#include "common.h"
#include "Presume.h"
#include "Sort.h"
#include "Str.h"
#include "Archive.h"

#include "StopDebugNew.h"

template<typename T1> class  CVector;
template<typename T1> void Serialize( CArchive& archive, CVector<T1>& v );

template<typename T1>
class CVector {
public:
	typedef T1 ElementType;

	CVector() : nItems(0), buffer( allocRowMemory( 0 ) ) {}
	explicit CVector( int n ) : nItems(n), buffer( allocRowMemory( n ) )
	{ initItems(); }
	CVector( int n, const T1& defaultItem ) : nItems(n), buffer( allocRowMemory( n ) )
	{ initItems( defaultItem ); }

	virtual ~CVector() { DeleteAll(); operator delete( buffer ); }

	int Size() const { return nItems; }
	int GetSize() const { return nItems; }
	int GetBufferSize() const { return bufferSize; }

	bool operator==( const CVector& other ) const;
	bool operator!=( const CVector& other ) const { return !(*this == other); };
	void CopyFrom( const CVector<T1>& other ) { CopyFrom( other.buffer, other.nItems ); }
	void CopyFrom( const T1* buf, int n );
	void Insert( int pos, const T1& item );
	void Append( const T1& item ) { Insert( nItems, item ); };
	void Append( const CVector<T1>& other ) { Append( other.buffer, other.nItems ); };
	void Append( const T1* buf, int n );
	void Delete( int pos );
	void DeleteAll();
	void SetValue( const T1& item );

	void SetBufferSize( int newBufferSize );
	void SetSize( int newSize );
	void GrowAt( int toAdd ) { SetSize( nItems + toAdd ); };

	const T1& operator[](int index) const { presume( 0 <= index && index < nItems ); return buffer[index]; }
	T1& operator[](int index) { presume( 0 <= index && index < nItems ); return buffer[index]; }
	const T1& First() const { presume( 0 < nItems ); return buffer[0]; }
	const T1& Last() const { presume( 0 < nItems ); return buffer[nItems - 1]; }
	T1& First() { presume( 0 < nItems ); return buffer[0]; }
	T1& Last() { presume( 0 < nItems ); return buffer[nItems - 1]; }

	T1* GetPtr() { return buffer; }
	const T1* GetPtr() const { return buffer; }

	//сортировки
	//сортировка пузырьком
	template<typename T2> void BubbleSort( const T2& cmpClass ) { ::BubbleSort( buffer, nItems, cmpClass ); }
	template<typename T2> void BubbleSort() { BubbleSort( T2() ); }
	void BubbleSort( int (*cmp)( const T1&, const T1& ) ) { BubbleSort( CCmpFunc<T1>( cmp ) ); }
	void BubbleSort() { BubbleSort( CCmpDefault<T1>() ); }
	//быстра€ сортировка хоара
	template<typename T2> void QuickSort( const T2& cmpClass ) { ::QuickSort( buffer, nItems, cmpClass ); }
	template<typename T2> void QuickSort() { QuickSort( T2() ); }
	void QuickSort( int (*cmp)( const T1&, const T1& ) ) { QuickSort( CCmpFunc<T1>( cmp ) ); }
	void QuickSort() { QuickSort( CCmpDefault<T1>() ); }

	//поиск линейный
	template<typename T2, typename T3>//поиск по классу равенства
		int Find( const T3& item, const T2& eqClass, int starIndex = 0 ) const;
	template<typename T2, typename T3>//поиск по классу равенства
		int Find( const T3& item, int starIndex = 0 ) const
	{ return Find( item, T2(), starIndex ); }
	template<typename T2>//по функции сравнени€ 
		int Find( const T2& item, bool (*eq)( const T1&, const T2& ), int starIndex = 0 ) const
	{ return Find( item, CEqFunc<T1, T2>( eq ), starIndex ); }
	template<typename T2>//по умолчанию использу€ == 
		int Find( const T2& item, int starIndex = 0 ) const
	{ return Find( item, CEqDefault<T1, T2>(), starIndex ); }

	//поиск индекса где должен находитьс€ элемент
	template<typename T2, typename T3>//поиск по классу равенства
		int BinaryFindIndex( const T3& item, const T2& eqClass ) const
	{ return ::BinaryFindIndex( buffer, nItems, item, eqClass ); }
	//поиск по функции сравнени€
	int BinaryFindIndex( const T1& item, int (*cmp)( const T1&, const T1& ) = 0 ) const
	{ return BinaryFindIndex( item, CCompare<T1>( cmp ) ); }

	//поиск бинарный, если нет элемента то -1
	template<typename T2, typename T3>//поиск по классу равенства
		int BinarySearch( const T3& item, const T2& eqClass ) const
	{ 	int index = ::BinaryFindIndex( buffer, nItems, item, eqClass );
	return ( index < nItems && eqClass.Cmp( buffer[index], item ) == 0 ) ? index : -1; }
	//поиск по функции сравнени€
	int BinarySearch( const T1& item, int (*cmp)( const T1&, const T1& ) = 0 ) const
	{ return BinarySearch( item, CCompare<T1>( cmp ) ); }

private:
	int getNewBufferSize( int minBufferSize ) const;
	T1* allocRowMemory( int n );
	void initItems();
	void initItems( const T1& initValue );
	void moveItem( T1* source, T1* target );
	void swap( CVector<T1>& v ) { 
		Swap( buffer, v.buffer );
		Swap( bufferSize, v.bufferSize );
		Swap( nItems, v.nItems );		
	}

	T1* buffer;
	int nItems;
	int bufferSize;

	friend void Serialize<T1>( CArchive& archive, CVector<T1>& v );

	DISALLOW_EVIL_CONSTRUCTORS( CVector<T1> );
};

typedef CVector<CString> CStrVector;
//////////////////////////////////////////////////////////////////////////

template<typename T1>
T1* CVector<T1>::allocRowMemory( int n ) 
{
	bufferSize = n;
	return ( (n == 0) ? 0 : static_cast<T1*>(operator new( sizeof(T1) * n )) );
}

template<typename T1>
void CVector<T1>::initItems()
{
	for( int i = 0; i < nItems; i++ ) {
		new(buffer + i) T1;
	}
}

template<typename T1>
void CVector<T1>::initItems( const T1& defaultItem )
{
	for( int i = 0; i < nItems; i++ ) {
		new(buffer + i) T1( defaultItem );
	}
}

template<typename T1>
int CVector<T1>::getNewBufferSize( int minBufferSize ) const
{
	int newBufferSize = ( bufferSize == 0 ) ? 1 : bufferSize;
	while( newBufferSize < minBufferSize ) {
		newBufferSize *= 2;
	}
	return newBufferSize;
}

template<typename T1>
void CVector<T1>::moveItem( T1* source, T1* target )
{
	new(target) T1(*source);
	source->~T1();
}

//////////////////////////////////////////////////////////////////////////

template<typename T1>
bool CVector<T1>::operator==( const CVector& other ) const
{
	if( nItems != other.nItems ) {
		return false;
	}
	for( int i = 0; i < nItems; i++ ) {
		if( !( buffer[i] == other.buffer[i] ) ) {
			return false;
		}
	}
	return true;
}

template<typename T1>
void CVector<T1>::CopyFrom( const T1* buf, int n )
{
	DeleteAll(); 
	operator delete( buffer );
	buffer = allocRowMemory( n );
	for( int i = 0; i < n; i++ ) {
		new(buffer + i) T1( buf[i] );
	}
	nItems = n;
}

template<typename T1>
void CVector<T1>::Append( const T1* buf, int n )
{
	SetBufferSize( nItems + n );
	for( int i = 0; i < n; i++ ) {
		Append( buf[i] );
	}
}

template<typename T1>
void CVector<T1>::Insert( int pos, const T1& item )
{
	if( nItems + 1 <= bufferSize ) {
		for( int i = nItems - 1; pos <= i; i-- ) {
			moveItem( buffer + i, buffer + i + 1 );
		}
	} else {
		T1* oldBuffer = buffer;
		buffer = allocRowMemory( getNewBufferSize( nItems + 1 ) );
		for( int i = 0; i < pos; i++ ) {
			moveItem( oldBuffer + i, buffer + i );
		}
		for( int i = pos; i < nItems; i++ ) {
			moveItem( oldBuffer + i, buffer + i + 1 );
		}
		operator delete(oldBuffer);
	}
	new(buffer + pos) T1( item );
	nItems += 1;	
}

template<typename T1>
void CVector<T1>::Delete( int pos )
{
	presume( pos < nItems );
	(buffer + pos)->~T1();
	if( pos < nItems - 1 ) {
		for( int i = pos; i < nItems - 1; i++ ) {
			moveItem( buffer + i + 1, buffer + i ) ;
		}
	}
	nItems -= 1;
}

template<typename T1>
void CVector<T1>::DeleteAll()
{
	for( int i = 0; i < nItems; i++ ) {
		(buffer + i)->~T1();
	}
	nItems = 0;
}

template<typename T1>
void CVector<T1>::SetBufferSize( int newBufferSize )
{
	if( bufferSize < newBufferSize ) {
		T1* oldBuffer = buffer;
		buffer = allocRowMemory( newBufferSize );
		for( int i = 0; i < nItems; i++ ) {
			moveItem( oldBuffer + i, buffer + i );
		}
		operator delete(oldBuffer);
	}
}

template<typename T1>
void CVector<T1>::SetSize( int newSize )
{
	if( bufferSize < newSize ) {
		T1* oldBuffer = buffer;
		buffer = allocRowMemory( getNewBufferSize( newSize ) );
		for( int i = 0; i < nItems; i++ ) {
			moveItem( oldBuffer + i, buffer + i );
		}
		operator delete(oldBuffer);
	} 
	if( nItems < newSize ) {
		for( int i = nItems; i < newSize; i++ ) {
			new(buffer + i) T1;
		}
	} else {//newSize <= nItems
		for( int i = newSize; i < nItems; i++ ) {
			(buffer + i)->~T1();
		}
	}
	nItems = newSize;
}

template<typename T1>
void CVector<T1>::SetValue( const T1& initValue )
{
	for( int i = 0; i < nItems; i++ ) {
		(buffer + i)->~T1();
		new(buffer + i) T1(initValue);
	}
}

#pragma warning (push)
#pragma warning( disable : 4100 )//ошибка компил€тора .NET 2003
template< typename T1 >
template<typename T2, typename T3>//поиск по классу равенства
int CVector<T1>::Find( const T3& item, const T2& eqClass, int starIndex = 0 ) const
{
	for( int i = starIndex; i < nItems; i++ ) {
		if( eqClass.Eq( buffer[i], item ) ) {
			return i;
		}
	}
	return -1;
}
#pragma warning( pop )

template<typename T1>
void Serialize( CArchive& archive, CVector<T1>& v )
{
	int nItems = v.Size();
	Serialize( archive, nItems );
	if( archive.IsReading() ) {
		CVector<T1> copy( nItems );
		for( int i = 0; i < nItems; i++ ) {
			Serialize( archive, copy[i] );
		}
		v.swap( copy );//успешно всЄ сериализовалось, и старый v сейчас удалитьс€
	} else {
		for( int i = 0; i < nItems; i++ ) {
			Serialize( archive, v[i] );
		}
	}
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
template<typename T1>
class CPtrVector : public CVector<T1*> {
public:
	virtual ~CPtrVector();

	template<typename T2>
		int Find( const T2& value ) const;

};

template<typename T1>
CPtrVector<T1>::~CPtrVector()
{
	for( int i = 0; i < nItems; i++ ) {
		delete buffer[i];
	}
}

template<typename T1>
template<typename T2>
int CPtrVector<T1>::Find( const T2& value ) const
{
	for( int i = 0; i < nItems; i++ ) {
		if( *(buffer[i]) == value ) {
			return i;
		}
	}
	return -1;
}






















































