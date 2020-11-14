#pragma once

#ifdef _DEBUG

void * operator new (size_t size, const char* fName, int nLine );
void * operator new[] (size_t size, const char* fName, int nLine );

//на случай исключений из конструктора
void operator delete (void *p, const char* fName, int nLine );
void operator delete[] (void *p, const char* fName, int nLine );

/*
void operator delete (void *p);
void operator delete[] (void *p);
*/

void TraceDelete( const char* fName, int nLine );

#endif