#pragma once
#include "common.h"
#include "Vector.h"

class CEnumeration {
public:
	virtual const CVector<int>& GetFirst() = 0;
	virtual const CVector<int>& GetNext() = 0;

	bool WasLast() const { return wasLast; };
	int operator[]( int index ) const { return v[index]; }
	int GetSize() const { return v.GetSize(); }

protected:
	CVector<int> v;
	int curCount;
	int maxCount;
	bool wasLast;
};

class CPermutation : public CEnumeration {
public:
	CPermutation( int n );

	const CVector<int>& GetFirst();
	const CVector<int>& GetNext();

	DISALLOW_EVIL_CONSTRUCTORS(CPermutation);
};

class CCombination : public CEnumeration {
public:
	CCombination( int n, int k );

	const CVector<int>& GetFirst();
	const CVector<int>& GetNext();

private:
	int maxValue;
	DISALLOW_EVIL_CONSTRUCTORS(CCombination);
};