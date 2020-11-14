#pragma once
#include <windows.h>

enum THookState;
//указатели на функции в длл-ке
typedef bool ( *TDF_HookSetState ) ( THookState state, HHOOK hNextHook, HWND hWnd, UINT crazyKeysMsg );
typedef LRESULT ( CALLBACK  *TDF_HookProcFunc ) ( int nCode, WPARAM wParam, LPARAM lParam );

class CЕхеHookManager {
public:
	CЕхеHookManager( HWND hWndMain );
	~CЕхеHookManager();

	bool SetHookState( THookState newState );
	THookState GetHookState() const { return hookState; };
	void OnHookStateIsChanged( THookState newState ) { hookState = newState; };

	UINT GetCrazyKeysMsg() const { return crazyKeysMsg; }

private:
	bool loadDll();//загружает длл
	void unloadDll();//выгружает длл

	bool setWndHook();//ставит хук функцию в системную цепь
	void removeWndHook();//отсоединяет хук функцию от системы

	bool isDllLoaded;//загружена ли длл
	bool isWndHookSet;//стоит ли ловушка в системе
	THookState hookState;//состояние хука

	HMODULE hModule;//модуль длл-ки
	HHOOK hHook;//хук следующего по очереди для обработки в дллки

	TDF_HookSetState fHookSetState;//выставляет состояние хука в длл-ке
	TDF_HookProcFunc fHookProc;//функция хука для системы

	HWND hWnd;//окно которое нужно длл-ке, чтобы слать в него сообщения
	UINT crazyKeysMsg;//сообщение которые длл-ка будет слать
};
