#pragma once
#include <windows.h>
#include "Presume.h"
#include "common.h"

//#include "StartDebugNew.h"
#include "StopDebugNew.h"

template <typename CharType> class CStr;

template <typename CharType>
class CStrBody {

	friend class CStr<CharType>;

private:
	//констуктор по длине буфера
	explicit CStrBody( int bufSize = 1 ) : nRef(1), bufferSize(bufSize), length(0)
	{
		assert( 0 < bufferSize );
		buffer = new CharType[bufferSize];
		buffer[0] = 0;
	}
	//конструктор, needAllocBuffer - нужно ли выделять память, либо приаттачить существующую
	explicit CStrBody( const CharType* newBuffer, int newLength = -1, int newBuferSize = -1, bool needAllocBuffer = true ) 
		: nRef(1), length( newLength ), bufferSize( newBuferSize )
	{
		if( length == -1 ) {
			length = StrLen( newBuffer );
		}
		if( bufferSize == -1 ) {
			bufferSize = length + 1;
		}
		presume( 0 <= length && length < bufferSize );
		if( needAllocBuffer ) {
			buffer = new CharType[bufferSize];
			MemCpy( buffer, newBuffer, sizeof(CharType) * (length) );
			buffer[length] = 0;
		} else {
			buffer = const_cast<CharType*>(newBuffer);
		}
	}
    	
	~CStrBody() { delete[] buffer; };

	//проверка на единственность
	bool IsUnique() const { return (nRef == 1); }
	//увеличения количества ссылок на буфер
	void IncRef() { InterlockedIncrement(&nRef); }
	//уменьшение количества ссылок на буфер
	void DecRef() {
		presume( 0 < nRef );
		if(InterlockedDecrement(&nRef) == 0) {
			delete this;
		}
	}

	CharType *buffer;//буфер
	long volatile nRef;//счетчик ссылок
	int bufferSize;//длина буфера
	int length;//длина строки	
};

