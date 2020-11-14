#pragma once
#include "Presume.h"
#include "Str.h"
#include "Ptr.h"
#include "Sort.h"
#include "common.h"

#include "StartDebugNew.h"
//#include "StopDebugNew.h"

template<typename T1> class  CList;
template<typename T1> void Serialize( CArchive& archive, CList<T1>& l );


template<typename T1>
class CList {
public:
	typedef T1 ElementType;

	CList() : head(0), tail(0), nItems(0) { resetMutables(); }
	CList(const T1& initItem) : head(0), tail(0), nItems(0) { resetMutables(); PushToHead(initItem); }
	virtual ~CList() { DeleteAll(); }
	
	template<typename T2>
	void CopyFrom( const T2& other );
	void CopyFrom( const CList& other );//скопировать в себя другой список
	void AppendFrom( const CList& other );//скопировать в конец себя другой список
	virtual void DeleteAll();//очистить очередь
	int PushToHead( const T1& newItem );//добавить в начало очереди	
	int PushToTail( const T1& newItem );//добавить в конец очереди
	int PushByIndex( const T1& newItem, int index );//добавить па заданому индексу
	
	T1 PopFromHead();//вынимает из очереди самый первый елемент
	T1 PopFromTail();//вынимает из очереди последний елемент
	T1 PopByIndex(int index);//вынимает из середины очереди елемент идущий номером index
	
	//константная индексация
	const T1& operator[](int index) const;
	const T1& Item( int index ) const;
	const T1& First() const { presume( 0 < nItems ); return head->item; }
	const T1& Last() const { presume( 0 < nItems ); return tail->item; }	
	//индексация НЕ константная
	T1& operator[](int index);
	T1& Item( int index );
	T1& First() { presume( 0 < nItems ); return head->item; }
	T1& Last() { presume( 0 < nItems ); return tail->item; }	

	//количество элементов
	int Size() const { return nItems; }
	int GetCount() const { return nItems; }

	//сортировки
	class CListItemCmpDefault;//класс сравнения по умолчанию
	template<typename T1> class CListItemCmpFunc;//класс сравнения по функции
	template<typename T1> class CListItemCmpClass;//класс сравнения по классу
	//сортировка пузырьком
	template<typename T2> void BubbleSort( const T2& cmpClass ) { bubbleSort( CListItemCmpClass<T2>( cmpClass ) ); }
	template<typename T2> void BubbleSort() { bubbleSort( CListItemCmpClass<T2>( T2() ) ); }
	void BubbleSort( int (*cmp)( const T1&, const T1& ) ) { bubbleSort( CListItemCmpFunc<T1>( cmp ) ); }
	void BubbleSort() { bubbleSort( CListItemCmpDefault() ); }
	//быстрая сортировка хоара
	template<typename T2> void QuickSort( const T2& cmpClass ) { quickSort( CListItemCmpClass<T2>( cmpClass ) ); }
	template<typename T2> void QuickSort() { quickSort( CListItemCmpClass<T2>( T2() ) ); }
	void QuickSort( int (*cmp)( const T1&, const T1& ) ) { quickSort( CListItemCmpFunc<T1>( cmp ) ); }
	void QuickSort() { quickSort( CListItemCmpDefault() ); }

	//поиск
	template<typename T2, typename T3>//поиск по классу равенства
	int Find( const T3& item, const T2& eqClass, int startIndex = 0 ) const;
	template<typename T2, typename T3>//поиск по классу равенства
	int Find( const T3& item, int startIndex = 0 ) const { return Find( item, T2(), startIndex ); }
	template<typename T2>//по функции сравнения 
	int Find( const T2& item, bool (*eq)( const T1&, const T2& ), int startIndex = 0 ) const
	{ return Find( item, CEqFunc<T1, T2>( eq ), startIndex ); }
	template<typename T2>//по умолчанию используя == 
	int Find( const T2& item, int startIndex = 0 ) const { return Find( item, CEqDefault<T1, T2>(), startIndex ); }

protected:
    struct CItem {
		CItem( const T1& newItem, CItem* pPrev, CItem* pNext ) 
			: item( newItem ), prev( pPrev ), next( pNext ) {}
		T1 item;
		CItem* next;
		CItem* prev;
	};

	CItem* head;
	CItem* tail;
	
	int nItems;

	mutable int iNode;
	mutable CItem* pNode;

	void swap( CList<T1>& l ) {
		Swap( head, l.head );
		Swap( tail, l.tail );
		Swap( nItems, l.nItems );
		Swap( iNode, l.iNode );
		Swap( pNode, l.pNode );
	}

	void moveMutables(int index) const;
	void resetMutables() const;
	void setMutablesToHead() const { presume( 0 < nItems ); pNode = head; iNode = 0;  }
	void setMutablesToTail() const { presume( 0 < nItems ); pNode = tail; iNode = nItems - 1; }
	bool checkMutableVariables() const {
		if( iNode == -1 && pNode == 0 && nItems == 0 ) { 
			return true;//не инициализированы оба
		}
		presume( iNode != -1 && pNode != 0 );//оба установлены
		CItem *tmpNode = head;
		for( int i = 0; i < iNode; i++, tmpNode = tmpNode->next ) {
		}//tmpNode делает iNode шагов
		presume( tmpNode == pNode );
		return true;
	}
//////////////////////////////////////////////////////////////////////////
	CItem** getArrayToSort();
	void setSortedArray( CItem** arrayOfItems );
	template<typename T2> void bubbleSort( const T2& cmpClass );
	template<typename T2> void quickSort( const T2& cmpClass );
//////////////////////////////////////////////////////////////////////////
	class CListItemCmpDefault {//класс сравнения по умолчанию
	public:
		template<typename T3> int Cmp( const T3& a, const T3& b ) const { return ( a->item < b->item ) ? -1: ( b->item < a->item ) ? 1 : 0; }
	};
	template<typename T3>//класс сравнения по функции сравнения
	class CListItemCmpFunc{
	public:
		CListItemCmpFunc( int (*pfUserCmp)( const T3&, const T3& ) ) : pfCmp( pfUserCmp ) {}
		//функция ставнения, которая вызывается из сортировки
		template<typename T3> int Cmp( const T3& a, const T3& b ) const { return pfCmp( a->item, b->item ); }
	private:
		int (*pfCmp)( const T3&, const T3& );//указатель на функцию, которая будет вызвана
	};
	template<typename T2>//класс сравнения по классу сравнения
	class CListItemCmpClass{
	public:
		CListItemCmpClass( const T2& cmpUserClass ) : cmpClass( cmpUserClass ) {}
		//функция ставнения, которая вызывается из сортировки
		template<typename T3> int Cmp( const T3& a, const T3& b ) const { return cmpClass.Cmp( a->item, b->item ); }
	private:
		const T2& cmpClass;//класс сравнения переданный пользовательм
	};

	friend void Serialize<T1>( CArchive& archive, CList<T1>& l );

	DISALLOW_EVIL_CONSTRUCTORS(CList<T1>);   
};

typedef CList<CString> CStrList;
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
template<typename T1>
void CList<T1>::CopyFrom( const CList& other )
{
	DeleteAll();
	AppendFrom( other );
}

template<typename T1>
template<typename T2>
void CList<T1>::CopyFrom( const T2& other )
{
	for( int i = 0; i < other.Size(); i++ ) {
		PushToTail( other[i] );
	}
}

template<typename T1>
void CList<T1>::AppendFrom( const CList& other )
{
	for( int i = 0; i < other.Size(); i++ ) {
		PushToTail( other[i] );
	}
}

template<typename T1>
void CList<T1>::DeleteAll()
{
	presume( checkMutableVariables() );
	for( nItems; 0 < nItems; nItems-- ) {
		CItem *tmpNode = head;
		head = head->next;
		delete tmpNode;
	}
	presume( head == 0 );
	tail = 0;
	resetMutables();
}

template<typename T1>
int CList<T1>::PushToHead( const T1& newItem ) 
{
	head = pNode = new CItem( newItem, 0, head );
	( pNode->next != 0 ) ? pNode->next->prev = pNode : tail = pNode;
	iNode = 0;
//	presume( checkMutableVariables() );
	return ++nItems;
}

template<typename T1>
int CList<T1>::PushToTail(const T1& newItem) 
{
	tail = pNode = new CItem( newItem, tail, 0 );
	( pNode->prev != 0 ) ? pNode->prev->next = pNode : head = pNode;
	iNode = nItems;
//	presume( checkMutableVariables() );
	return ++nItems;
}

template<typename T1>
int CList<T1>::PushByIndex( const T1& newItem, int index ) 
{
	assert( 0 <= index && index <= nItems );
//	presume( checkMutableVariables() );
	if( index == 0 ) {
		PushToHead( newItem );
	} else if( index == nItems ) {
		PushToTail( newItem );
	} else {
		moveMutables( index );
		pNode = new CItem( newItem, pNode->prev, pNode );
		pNode->next->prev = pNode;		
		pNode->prev->next = pNode;
		nItems++;
	}
//	presume( checkMutableVariables() );
	return nItems;
}

template<typename T1>
T1 CList<T1>::PopFromHead()
{ 
	assert( 0 < nItems );
	nItems--;
	CItem *tmp_pNode = head;
	head = tmp_pNode->next;
	(head != 0) ? head->prev = 0 : tail = 0;
	T1 oldItem = tmp_pNode->item;
	delete tmp_pNode;
	if( nItems == 0 || iNode == 0 ) {
		resetMutables();
	} else {
		iNode--;
	}
	return oldItem;
}

template<typename T1>
T1 CList<T1>::PopFromTail()
{
	assert( 0 < nItems );
	nItems--;
	CItem *tmp_pNode = tail;
	tail = tmp_pNode->prev;
	(tail != 0) ? tail->next = 0 : head = 0;
	T1 oldItem = tmp_pNode->item;
	delete tmp_pNode;
	if( nItems == 0 || iNode == nItems ) {
		resetMutables();
	}
	return oldItem;
}

template<typename T1>
T1 CList<T1>::PopByIndex(int index)
{
	//presume( checkMutableVariables() );
	assert( 0 <= index && index <= nItems );
	if( index == 0 ) {
		return PopFromHead();
	} else if( index == nItems - 1 ) {
		return PopFromTail();
	} else {
		moveMutables( index );
		CItem *tmp_pNode = pNode;
		tmp_pNode->next->prev = tmp_pNode->prev;
		tmp_pNode->prev->next = tmp_pNode->next;
		T1 oldItem = tmp_pNode->item;
		pNode = tmp_pNode->next;
		delete tmp_pNode;
		nItems--;
		presume( checkMutableVariables() );
		return oldItem;
	}
}

template<typename T1>
const T1& CList<T1>::operator[](int index) const 
{
	return (*(const_cast<CList<T1>*>(this)))[index];
}

template<typename T1>
T1& CList<T1>::operator[](int index)
{
//	presume( checkMutableVariables() );
	assert( 0 <= index && index < nItems );
	moveMutables( index );
	return pNode->item;	
}

template<typename T1>
const T1& CList<T1>::Item( int index ) const
{
	//presume( checkMutableVariables() );
	assert( 0 <= index && index < nItems );
	moveMutables( index );
	return pNode->item;
}

template<typename T1>
T1& CList<T1>::Item( int index )
{
	//presume( checkMutableVariables() );
	assert( 0 <= index && index < nItems );
	moveMutables( index );
	return pNode->item;
}

/////////////////////////////////////////////////////////////////////////

template<typename T1>
void CList<T1>::moveMutables( int index ) const
{
	presume( 0 <= index && index < nItems );
	//presume( checkMutableVariables() );
	if( index == iNode ) {//уже стоят где надо
		return;
	} else if( index == 0 ) {//ставим на первый
		setMutablesToHead();
	} else if( index == nItems - 1 ) {//ставим на последний
		setMutablesToTail();
	} else {
		if( index + 1 < iNode - index) {//индекс ближе к head чем к iNode
			setMutablesToHead();
		} else if( nItems - index < index - iNode ) {//индекс ближе к tail чем к iNode
			setMutablesToTail();
		}
		if( iNode < index ) {//отходим вправо
			for( ; iNode < index; pNode = pNode->next, iNode++ ) {
			}
		} else if( index < iNode ) {//отходим влево
			for( ; index < iNode; pNode = pNode->prev, iNode-- ) {
			}
		}
		presume( checkMutableVariables() );
	}
}

template<typename T1>
void CList<T1>::resetMutables() const 
{ 
	if( 0 < nItems ) {
		setMutablesToHead();
	} else {
		iNode = -1; 
		pNode = 0; 
	}
}

//////////////////////////////////////////////////////////////////////////
template<typename T1>
typename CList<T1>::CItem** CList<T1>::getArrayToSort()
{
	CItem** arrayOfItems = new CItem*[nItems];
	for( iNode = 0, pNode = head; iNode < nItems; pNode = pNode->next, iNode++ ) {
		arrayOfItems[iNode] = pNode;
	}
	return arrayOfItems;
}

template<typename T1>
void CList<T1>::setSortedArray( CItem** arrayOfItems )
{
	head = arrayOfItems[0];
	for( iNode = 0, pNode = head; iNode < nItems - 1; pNode = pNode->next, iNode++ ) {
		pNode->next = arrayOfItems[iNode + 1];
		pNode->next->prev = pNode;
	}
	tail = pNode;
	head->prev = tail->next = 0;
	delete[] arrayOfItems;
}

template<typename T1>
template<typename T2>
void CList<T1>::bubbleSort( const T2& cmpClass )
{
	CItem** arrayOfItems = getArrayToSort();
	::BubbleSort( arrayOfItems, nItems, cmpClass );
	setSortedArray( arrayOfItems );
}

template<typename T1>
template<typename T2>
void CList<T1>::quickSort( const T2& cmpClass )
{
	CItem** arrayOfItems = getArrayToSort();
	::QuickSort( arrayOfItems, nItems, cmpClass );
	setSortedArray( arrayOfItems );
}

#pragma warning (push)
#pragma warning( disable : 4100 )//ошибка компилятора .NET 2003
template< typename T1 >
template<typename T2, typename T3>//поиск по классу равенства
int CList<T1>::Find( const T3& item, const T2& eqClass, int startIndex = 0 ) const
{

	//presume( checkMutableVariables() );
	presume( startIndex <= nItems );
	if( nItems == 0 || nItems <= startIndex ) {
		return -1;
	}
	moveMutables( startIndex );
	for( ; iNode < nItems && !eqClass.Eq( pNode->item, item ); pNode = pNode->next, iNode++ ) {
	}
	if( iNode < nItems ) {
		return iNode;
	}
	setMutablesToHead();
	return -1;
}
#pragma warning( pop )

template<typename T1>
void Serialize( CArchive& archive, CList<T1>& l )
{
	int nItems = l.Size();
	Serialize( archive, nItems );
	if( archive.IsReading() ) {
		CList<T1> copy;
        while( copy.Size() < nItems ) {
			copy.PushToTail( T1() );
		}
		for( int i = 0; i < nItems; i++ ) {
			Serialize( archive, copy[i] );
		}
		l.swap( copy );//меняем местами копию и оригинал
	} else {
		for( int i = 0; i < nItems; i++ ) {
			Serialize( archive, l[i] );
		}
	}
}



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
template<typename T1>
class CPtrList : public CList<CPtr<T1> > {
public:
		//поиск
	template<typename T2, typename T3>//поиск по классу равенства
	int Find( const T3& item, const T2& eqClass, int startIndex = 0 ) const;
	template<typename T2, typename T3>//поиск по классу равенства
	int Find( const T3& item, int startIndex = 0 ) const { return Find( item, T2(), startIndex ); }
	template<typename T2>//по функции сравнения 
	int Find( const T2& item, bool (*eq)( const T1&, const T2& ), int startIndex = 0 ) const
	{ return Find( item, CEqFunc<T1, T2>( eq ), startIndex ); }
	template<typename T2>//по умолчанию используя == 
	int Find( const T2& item, int startIndex = 0 ) const { return Find( item, CEqDefault<T1, T2>(), startIndex ); }

};

template< typename T1 >
template< typename T2, typename T3 >
int CPtrList<T1>::Find( const T3& item, const T2& eqClass, int starIndex ) const
{
	//presume( checkMutableVariables() );
	if( nItems < 1 || nItems <= starIndex ) {
		return -1;
	}
	moveMutables( starIndex );
	for( ; iNode < nItems && !eqClass.Eq( *(pNode->item), item ); pNode = pNode->next, iNode++ ) {
	}
	if( iNode < nItems ) {
		return iNode;
	}
	iNode = 0;
	pNode = head;
	return -1;
}

