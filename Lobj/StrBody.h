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
	//���������� �� ����� ������
	explicit CStrBody( int bufSize = 1 ) : nRef(1), bufferSize(bufSize), length(0)
	{
		assert( 0 < bufferSize );
		buffer = new CharType[bufferSize];
		buffer[0] = 0;
	}
	//�����������, needAllocBuffer - ����� �� �������� ������, ���� ����������� ������������
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

	//�������� �� ��������������
	bool IsUnique() const { return (nRef == 1); }
	//���������� ���������� ������ �� �����
	void IncRef() { InterlockedIncrement(&nRef); }
	//���������� ���������� ������ �� �����
	void DecRef() {
		presume( 0 < nRef );
		if(InterlockedDecrement(&nRef) == 0) {
			delete this;
		}
	}

	CharType *buffer;//�����
	long volatile nRef;//������� ������
	int bufferSize;//����� ������
	int length;//����� ������	
};

