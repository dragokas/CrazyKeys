#pragma once
#include "Presume.h"

//#include "StartDebugNew.h"
//#include "StopDebugNew.h"

template<class Type>
class CPtr {
public:
	CPtr() { 
		ptr = 0; 
	}
	CPtr( Type* p ) { 
		ptr = new CPointer( p ); 
	}
	CPtr( const CPtr<Type>& other ) { 
		ptr = other.ptr; 
		if( ptr != 0 ) {
			ptr->IncRef(); 
		}
	}
	~CPtr() { 
		if( ptr != 0 ) {
			ptr->DecRef(); 
		}
	}
	CPtr<Type>& operator=( const CPtr<Type>& other )
	{
		other.ptr->IncRef();
		if( ptr != 0 ) {
			ptr->DecRef();
		}
		ptr = other.ptr;
		return *this;
	};
//////////////////////////////////////////////////////////////////////////
	Type* operator->() const { presume( ptr != 0 ); return ( ptr == 0 ) ? 0 : ptr->p; };
	operator Type*() const { return ( ptr == 0 ) ? 0 : ptr->p; };

private:
	class CPointer {
	public:
		Type* p;
		CPointer( Type* _p = 0 ) : p( _p ), nRef( 1 ) {};
		void IncRef() { InterlockedIncrement(&nRef); }
		void DecRef() {
			presume( 0 < nRef );
			if( InterlockedDecrement(&nRef) == 0 ) {
				delete this;
			}
		}
	private:
		long volatile nRef;		
		~CPointer() { 
			delete p;
		};		
	};
	CPointer* ptr; 
};