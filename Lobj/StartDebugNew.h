//ставит отладочный вариант new
#pragma once

#ifdef _DEBUG
#include "DebugNew.h"
#define new new( __FILE__, __LINE__ )
#define delete TraceDelete( __FILE__, __LINE__ ), delete
#endif


