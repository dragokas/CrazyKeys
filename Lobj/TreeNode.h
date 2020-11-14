#pragma once
#include "Presume.h"
#include "Queue.h"
#include "Ptr.h"

#include "StartDebugNew.h"

template<typename TypeOfItems>
class CTreeNode : protected CList< CPtr< CTreeNode<TypeOfItems> > > {

public:
	CTreeNode( const TypeOfItems& newItem ) : item(newItem), parent(0) {}
	TypeOfItems& Item() { return item; }
	const TypeOfItems& Item() const { return item; }
	int GetChildrenCount() const { return GetCount(); }
	CTreeNode* GetChild( int index ) const { return operator[](index); }
	void AddChild( const TypeOfItems& newItem ) { PushToTail( new CTreeNode<TypeOfItems>( newItem, this ) ); } 

private:
	CTreeNode( const TypeOfItems& newItem, CTreeNode* _parent ) : item(newItem), parent(_parent) {}

	CTreeNode* parent;
	TypeOfItems item;
};
