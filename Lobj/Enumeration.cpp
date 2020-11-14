#include "Enumeration.h"
#include "common.h"

CPermutation::CPermutation( int n )
{
	v.SetSize( n );
	maxCount = 1;
	for( int i = 1; i <= n; i++, maxCount *= i );
	GetFirst();
}

const CVector<int>& CPermutation::GetFirst()
{
	for( int i = 0; i < v.GetSize(); i++ ) {
		v[i] = i;
	}
	curCount = 1;
	wasLast = ( maxCount < curCount );
	return v;
}

const CVector<int>& CPermutation::GetNext()
{
	int n = v.GetSize();
	int i = n-1;
	for( i = n-1; 0 < i && v[i] < v[i-1]; i-- ) {
	}
	if( i == 0 ) {
		return v;
	}
	int j = i + 1;
	for( j = i + 1; j < n && v[i-1] < v[j]; j++ ) {
	}
	Swap( v[i-1], v[j-1] );
	for( int k = 0; k < (n - i)/2; k++ ) {
		Swap( v[n-k-1], v[i+k] );
	}
	curCount++;
	wasLast = ( maxCount < curCount );
	return v;
}

//////////////////////////////////////////////////////////////////////////

CCombination::CCombination( int n, int k )
{
	v.SetSize( n );
	maxCount = 1;
	maxValue = k - 1;
	for( int i = 1; i <= n; i++, maxCount *= k );
	GetFirst();
}

const CVector<int>& CCombination::GetFirst()
{
	v.SetValue( 0 );
	curCount = 1;
	wasLast = ( maxCount < curCount );
	return v;
}

const CVector<int>& CCombination::GetNext()
{
	int i = v.GetSize() - 1;
	for( i; 0 <= i && v[i] == maxValue; i-- ) {
		v[i] = 0;
	}
	if( 0 <= i ) {
		v[i]++;
	}
	curCount++;
	wasLast = ( maxCount < curCount );
	return v;
}




















