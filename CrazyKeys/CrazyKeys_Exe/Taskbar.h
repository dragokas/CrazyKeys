#pragma once
#include <windows.h>
#include "Vector.h"

enum THookState;

class CTaskbar {
public:
	CTaskbar( HINSTANCE hInstance, HWND hWndMain ) : hInst( hInstance ), hWnd( hWndMain ), hIcons( 3, 0 ) {};
	~CTaskbar();

	bool Init();//загружает иконки и делает сообщение от таскбара

	UINT GetTaskbarWM() const { return taskbarMsg; };//сообщение от таскбара
	void SetIconColor( THookState color );//ставит цвет иконки
	void ShowMenu( THookState hookState );//показывает меню

private:	
	void fillNotifyIconData( NOTIFYICONDATA& tnid, THookState color );//заполняет структ. данными

	HINSTANCE hInst;
	UINT taskbarMsg;//сообщение нажатия на таскбар иконку
	HWND hWnd;//окно ловящее сообщение от таскбара
	CVector<HICON> hIcons;//иконки
	THookState currentIconColor;//текущий цвет иконки

	DISALLOW_EVIL_CONSTRUCTORS( CTaskbar );
};
