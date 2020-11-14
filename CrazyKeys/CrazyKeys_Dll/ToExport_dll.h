#pragma once

enum THookState;

extern "C" __declspec(dllexport) bool SetHookState( THookState state, HHOOK hNextHook, HWND hWnd, UINT crazyKeysMsg );
__declspec(dllexport) LRESULT CALLBACK LowLevelKeyboardProcFunc( int nCode, WPARAM wParam, LPARAM lParam );


