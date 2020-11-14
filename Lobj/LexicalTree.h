#pragma once
#include "common.h"
#include "presume.h"
#include "StopDebugNew.h"
#include <new>

template<typename T1>
class CLexicalTree {
public:
	CLexicalTree(void) : root( 0, 0 ){}
	~CLexicalTree(void) { DeleteAll(); }

	template<typename K1> void Add( const K1& key, const T1& item ) { Add( &key, sizeof(K1), item ); };
	void Add( const void* key, int keySize, const T1& item ) {
		add( root.children[((unsigned char*)key)[0]], (unsigned char*)key, keySize, item );
	}

	template<typename K1> T1* Find( const K1& key ) const { return Find( &key, sizeof(K1) ); };
	T1* Find( const void* key, int keySize ) const {
		return find( root.children[((unsigned char*)key)[0]], (unsigned char*)key, keySize );
	}

	void DeleteAll();
	
private:
	struct CNode {
		CNode( const unsigned char* _pKey, int _keySize, const T1& item ) : isLeaf( true ), keySize( _keySize ) {
			setPointers( _pKey );
			new( (T1*)itemMem ) T1( item );
		}
		CNode( const unsigned char* _pKey, int _keySize ) : isLeaf( false ), keySize( _keySize ) {
			setPointers( _pKey );
		}

		void setPointers( const unsigned char* _pKey ) {
			ZeroData( children );
			pKey = (unsigned char*)this + sizeof( CNode );
			for( int i = 0; i < keySize; i++ ) {
				pKey[keySize - i - 1] = _pKey[i];
			}
		}
		int getCommonKeyPos( const unsigned char* _pKey, int _keySize ) const {
			int i = 0;
			for( i = 0; i < keySize && i < _keySize && pKey[keySize - i - 1] == _pKey[i]; i++ );
			return i;
		}
		unsigned char getFirstKeyByte() const { return pKey[keySize - 1]; }

		unsigned char* pKey;
		int keySize;
		bool isLeaf;
		char itemMem[sizeof(T1)];
		CNode* children[256];
	} root;

	void add( CNode*& r, const unsigned char* key, int keySize, const T1& item );
	T1* find( const CNode* r, const unsigned char* key, int keySize ) const;

	void newNode( CNode*& r, const unsigned char* key, int keySize, const T1& item ) {
		r = (CNode*)new unsigned char[sizeof( CNode ) + keySize];
		new( r ) CNode( key, keySize, item ); 
	}
	void newNode( CNode*& r, const unsigned char* key, int keySize ) {
		r = (CNode*)new unsigned char[sizeof( CNode ) + keySize];
		new( r ) CNode( key, keySize ); 
	}

	void deleteAll( CNode* r );
};

template<typename T1>
void CLexicalTree<T1>::add( CNode*& r, const unsigned char* key, int keySize, const T1& item )
{
	if( r == 0 ) {
		newNode( r, key, keySize, item );
	} else {
		int pos = r->getCommonKeyPos( key, keySize );
		if( pos < r->keySize ) {//разбиваем узел на части
			r->keySize -= pos;
			CNode* p = r;
			if( pos == keySize ) {
				newNode( r, key, keySize, item );
				r->children[p->getFirstKeyByte()] = p;
			} else {
				newNode( r, key, pos );
				r->children[p->getFirstKeyByte()] = p;
				newNode( r->children[key[pos]], key + pos, keySize - pos, item );
			}
		} else {
			if( pos < keySize ) {
				add( r->children[key[pos]], key + pos, keySize - pos, item );
			} else {
				assert( !r->isLeaf );
				r->isLeaf = true;
				new( (T1*)r->itemMem ) T1( item );
			}
		}
	}
}

template<typename T1>
T1* CLexicalTree<T1>::find( const CNode* r, const unsigned char* key, int keySize ) const
{
	if( r == 0 ) {
		return 0;
	} else {
		int pos = r->getCommonKeyPos( key, keySize );
		if( pos < r->keySize ) {
			return 0;
		} else {
			if( pos < keySize ) {
				return find( r->children[key[pos]], key + pos, keySize - pos );
			} else {
				if( r->isLeaf ) {
					return (T1*)(r->itemMem);
				} else {
					return 0;
				}
			}
		}
	}
}

template<typename T1>
void CLexicalTree<T1>::deleteAll( CNode* r )
{
	if( r == 0 ) {
		return;
	}
	for( int i = 0; i < 256; i++ ) {
		deleteAll( r->children[i] );
	}
	if( r->isLeaf ) {
		((T1*)(r->itemMem))->~T1();
	}
	delete r;
}

template<typename T1>
void CLexicalTree<T1>::DeleteAll()
{
	for( int i = 0; i < 256; i++ ) {
		deleteAll( root.children[i] );
	}
	ZeroData( root.children );
}


































