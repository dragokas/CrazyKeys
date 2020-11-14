#pragma once

#ifdef _UNICODE
void ShowAssertMessage( wchar_t* caption, wchar_t* exp, const char *file, int line);
#else
void ShowAssertMessage( char* caption, char* exp, const char *file, int line);
#endif

//assert
#ifdef _UNICODE
#define assert(exp) { if( !(exp) ) { ShowAssertMessage(L"assert", L###exp, __FILE__, __LINE__); } }
#define assertFalse { ShowAssertMessage(L"assert", L"assertFalse", __FILE__, __LINE__); }
#ifndef _DEBUG
#define presumeFalse {}
#endif
#else 
#define assert(exp) { if( !(exp) ) { ShowAssertMessage("assert", ###exp, __FILE__, __LINE__); } }
#define assertFalse { ShowAssertMessage("assert", "assertFalse", __FILE__, __LINE__); }
#ifndef _DEBUG
#define presumeFalse {}
#endif
#endif

//presume
#ifdef _DEBUG

#ifdef _UNICODE
#define presume(exp) { if( !(exp) ) { ShowAssertMessage(L"presume", L###exp, __FILE__, __LINE__); } }
#define presumeFalse { ShowAssertMessage(L"presume", L"presumeFalse", __FILE__, __LINE__); }
#else 
#define presume(exp) { if( !(exp) ) { ShowAssertMessage("presume", ###exp, __FILE__, __LINE__); } }
#define presumeFalse { ShowAssertMessage("presume", "presumeFalse", __FILE__, __LINE__); }
#endif

#else
#define presume(exp) { if( !(exp) ) {;} };
#endif

//FailShowError
#ifdef _UNICODE
void ShowLastErrorMessage( wchar_t* exp, const char *file, int line );
#define CheckError( exp, erValue ) { if( (exp) == erValue ) { ShowLastErrorMessage( L###exp, __FILE__, __LINE__); } }
#define CheckZero( exp ) { if( (exp) == 0 ) { ShowLastErrorMessage( L###exp, __FILE__, __LINE__); } }
#define CheckFalse( exp ) { if( !(exp) ) { ShowLastErrorMessage( L###exp, __FILE__, __LINE__); } }
#else
void ShowLastErrorMessage( char* exp, const char *file, int line );
#define CheckError( exp, erValue ) { if( (exp) == erValue ) { ShowLastErrorMessage( ###exp, __FILE__, __LINE__); } }
#define CheckZero( exp ) { if( (exp) == 0 ) { ShowLastErrorMessage( ###exp, __FILE__, __LINE__); } }
#define CheckFalse( exp ) { if( !(exp) ) { ShowLastErrorMessage( ###exp, __FILE__, __LINE__); } }
#endif


