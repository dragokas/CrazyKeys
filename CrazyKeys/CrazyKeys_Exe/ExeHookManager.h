#pragma once
#include <windows.h>

enum THookState;
//��������� �� ������� � ���-��
typedef bool ( *TDF_HookSetState ) ( THookState state, HHOOK hNextHook, HWND hWnd, UINT crazyKeysMsg );
typedef LRESULT ( CALLBACK  *TDF_HookProcFunc ) ( int nCode, WPARAM wParam, LPARAM lParam );

class C���HookManager {
public:
	C���HookManager( HWND hWndMain );
	~C���HookManager();

	bool SetHookState( THookState newState );
	THookState GetHookState() const { return hookState; };
	void OnHookStateIsChanged( THookState newState ) { hookState = newState; };

	UINT GetCrazyKeysMsg() const { return crazyKeysMsg; }

private:
	bool loadDll();//��������� ���
	void unloadDll();//��������� ���

	bool setWndHook();//������ ��� ������� � ��������� ����
	void removeWndHook();//����������� ��� ������� �� �������

	bool isDllLoaded;//��������� �� ���
	bool isWndHookSet;//����� �� ������� � �������
	THookState hookState;//��������� ����

	HMODULE hModule;//������ ���-��
	HHOOK hHook;//��� ���������� �� ������� ��� ��������� � �����

	TDF_HookSetState fHookSetState;//���������� ��������� ���� � ���-��
	TDF_HookProcFunc fHookProc;//������� ���� ��� �������

	HWND hWnd;//���� ������� ����� ���-��, ����� ����� � ���� ���������
	UINT crazyKeysMsg;//��������� ������� ���-�� ����� �����
};
