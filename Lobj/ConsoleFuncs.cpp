#include <windows.h>

#include "ConsoleFuncs.h"

void GotoXY( int x, int y )
{
	HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
	COORD cord;
	cord.X = (SHORT)x;
	cord.Y = (SHORT)y;
	SetConsoleCursorPosition( hConsole, cord );
}

void SetColor( WORD attr ) 
{
	HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
	SetConsoleTextAttribute( hConsole, attr );
}
