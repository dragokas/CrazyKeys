#pragma once
#include <windows.h>
#include "Vector.h"

enum THookState;

class CTaskbar {
public:
	CTaskbar( HINSTANCE hInstance, HWND hWndMain ) : hInst( hInstance ), hWnd( hWndMain ), hIcons( 3, 0 ) {};
	~CTaskbar();

	bool Init();//��������� ������ � ������ ��������� �� ��������

	UINT GetTaskbarWM() const { return taskbarMsg; };//��������� �� ��������
	void SetIconColor( THookState color );//������ ���� ������
	void ShowMenu( THookState hookState );//���������� ����

private:	
	void fillNotifyIconData( NOTIFYICONDATA& tnid, THookState color );//��������� ������. �������

	HINSTANCE hInst;
	UINT taskbarMsg;//��������� ������� �� ������� ������
	HWND hWnd;//���� ������� ��������� �� ��������
	CVector<HICON> hIcons;//������
	THookState currentIconColor;//������� ���� ������

	DISALLOW_EVIL_CONSTRUCTORS( CTaskbar );
};
