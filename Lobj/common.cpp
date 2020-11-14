#include "common.h"

void MemCpy( void* target, const void* source, size_t count )
{
	if( target == source || count == 0 ) {
		return;
	}
	if( ( source < target && (char*)target < (const char*)(source) + count ) ) {
		//копируем с права на лево, т.к. есть нахлёст
		char* t = (char*)target + count - 1;
		const char* s = (const char*)source + count - 1;
		for( ; target <= t; s--, t-- ) {
			*t = *s;
		}
	} else {//копируем с лева на право, обычно
		char* t = (char*)target;
		const char* end = t + count;
		const char* s = (const char*)source;
		for( ; t < end; s++, t++ ) {
			*t = *s;
		}
	}
}

bool MemCmp( const void* p1, const void* p2, size_t count )
{
	if( count == 0 ) {
		return true;
	}
	const char* pCh1 = (const char*)p1;
	const char* pCh2 = (const char*)p2;
	count++;
	while( ( 0 < --count ) && *pCh1++ == *pCh2++ );
	return ( count == 0 );
}











