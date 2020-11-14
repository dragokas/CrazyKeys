#pragma once
#include "Presume.h"
#include "Vector.h"
#include "Ptr.h"

#include "StartDebugNew.h"

template<typename TypeOfItems>
class CMatrix {
public:
	CMatrix() : nColumn(0) {};
	CMatrix( int size );
	CMatrix( int rows, int columns );
	~CMatrix() {};

	void SetNewSize( int rows, int columns );
	void SetNewSize( int size ) { SetNewSize( size, size ); }

//	void GrowAtRows( int count );
//	void GrowAtColumn( int count );

	void SetValue( TypeOfItems value );

//	void DeleteColumn( int column );
//	void DeleteRow( int row );
//	void DeleteRC( int index );
	
	int GetRowCount() const { assert( 0 <= nColumn ); return matrix.GetSize(); }
	int GetColumnCount() const { return nColumn; }

	const CVector<TypeOfItems>& operator[](int index) const;
	CVector<TypeOfItems>& operator[](int index);

protected:
	int nColumn;
	CVector<CPtr<CVector<TypeOfItems> > > matrix;
};

typedef CMatrix<CString> CStrMatrix;

template<typename TypeOfItems>
CMatrix<TypeOfItems>::CMatrix( int size )
: matrix( size )
{
	nColumn = size;
	for( int i = 0; i < size; i++ ) {
		matrix[i] = new CVector<TypeOfItems>( nColumn );
	}
}

template<typename TypeOfItems>
CMatrix<TypeOfItems>::CMatrix( int rows, int columns )
: matrix( rows )
{
	nColumn = columns;
	for( int i = 0; i < rows; i++ ) {
		matrix[i] = new CVector<TypeOfItems>( nColumn );
	}
}

/*
template<typename TypeOfItems>
void CMatrix<TypeOfItems>::GrowAtRows( int count )
{
	int nRows = matrix.GetSize();
	matrix.GrowAtSize( count );
	for( int i = nRows; i < nRows + count; i++ ) { 
		matrix[i].GrowAtSize( nColumn );
	}
}*/
/*
template<typename TypeOfItems>
void CMatrix<TypeOfItems>::GrowAtColumn( int count )
{
	nColumn += count;
	for( int i = 0; i < matrix.GetSize(); i++ ) {
		matrix[i].GrowAtSize( count );
	}	
}*/

template<typename TypeOfItems>
void CMatrix<TypeOfItems>::SetNewSize( int rows, int columns )
{
	matrix.SetSize( rows );
	nColumn = columns;
	for( int i = 0; i < matrix.GetSize(); i++ ) {
		if( matrix[i] == 0 ) {
			matrix[i] = new CVector<TypeOfItems>( nColumn );
		} else {
			matrix[i]->SetSize( nColumn );
		}
	}
}

template<typename TypeOfItems>
void CMatrix<TypeOfItems>::SetValue( TypeOfItems value )
{
	for( int i = 0; i < matrix.GetSize(); i++ ) {
		for( int j = 0; j < matrix[i]->GetSize(); j++ ) {
			(*matrix[i])[j] = value;
		}
	}
}

/*
template<typename TypeOfItems>
void CMatrix<TypeOfItems>::DeleteColumn( int column )
{
	presume( 0 < nColumn );
	for( int i = 0; i < matrix.GetSize(); i++ ) {
		matrix[i].Delete( column );
	}
	nColumn--;
}

template<typename TypeOfItems>
void CMatrix<TypeOfItems>::DeleteRow( int row )
{
	presume( row <= matrix.GetSize() );
	matrix.Delete( row );
}

template<typename TypeOfItems>
void CMatrix<TypeOfItems>::DeleteRC( int index )
{
	DeleteRow( index );
	DeleteColumn( index );
}*/

template<typename TypeOfItems>
const CVector<TypeOfItems>& CMatrix<TypeOfItems>::operator[]( int index ) const 
{
	return *(matrix[index]);
}

template<typename TypeOfItems>
CVector<TypeOfItems>& CMatrix<TypeOfItems>::operator[]( int index )
{
	return *(matrix[index]);
}

