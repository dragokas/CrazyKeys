#pragma once

enum THookState{ 
	HS_On = 0, HS_Green = HS_On,//включен
	HS_Pause = 1, HS_Yellow = HS_Pause,//на паузе
	HS_Off = 2, HS_Red = HS_Off,//выгружен/выключен
	HS_Count = 3,//всего состояний
	HS_Undefined = 4, HS_NoIcon = HS_Undefined//неопределёное состояние
};

#define WM_FROM_DLL_CHANGE_ICON 0//отсылается из длл, когда надо сменить иконку

const int MY_KEY_MESSAGE = 7;

const wchar_t CrazyKeysProgramName[] = L"CrazyKeys";
const wchar_t HookDllFileName[] = L"CrazyKeys_Dll.dll";
const wchar_t CfgCangeLangFileName[] = L"changeLanguageSequence.cfg";
const wchar_t CfgDefaultRulesFileName[] = L"CrazyKeysDefaultRules.cfg";
const wchar_t HelpFileName[] = L"CrazyKeysHelp.hlp";

const short DefaultDelayTime = 555;
const short DefaultSleepTime = 33;
const short DefaultTimerTime = 22;

const wchar_t spaceBetweenLanguages[] = L" -->> ";

const unsigned char scanCodes[4][15] = {//тут только кнопки которые квадратные
	{0x29, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x2B, 0},
	{      0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B,    0, 0},
	{      0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x2B,    0, 0},
	{      0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,    0,    0,    0, 0}
};

const wchar_t VKeyNames[256][100] = {
/*00-07*/L"No key", L"Left mouse", L"Right mouse", L"Control-break", L"Middle mouse", L"X1 mouse", L"X2 mouse", L"Undefined",
/*08-0F*/L"Backspace", L"Tab", L"Reserved", L"Reserved", L"Clear", L"Enter", L"Undefined", L"Undefined", 
/*10-17*/L"Shift", L"Ctrl", L"Alt", L"Pause", L"Capital", L"Kana/Hangul", L"Undefined", L"Junja", 
/*18-1F*/L"Final", L"Hanja/Kanji", L"Undefined", L"Esc", L"Convert", L"Nonconvert", L"Accept", L"Mode Change", 
/*20-27*/L"Spacebar", L"Page Up", L"Page Down", L"End", L"Home", L"Left arrow", L"Up arrow", L"Right arrow", 
/*28-2F*/L"Down Arrow", L"Select", L"Print", L"Execute", L"Print Screen", L"Ins", L"Del", L"Help", 
/*30-37*/L"0", L"1", L"2", L"3", L"4", L"5", L"6", L"7", 
/*38-3F*/L"8", L"9", L"Undefined", L"Undefined", L"Undefined", L"Undefined", L"Undefined", L"Undefined", 
/*40-47*/L"Undefined", L"A", L"B", L"C", L"D", L"E", L"F", L"G", 
/*48-4F*/L"H", L"I", L"J", L"K", L"L", L"M", L"N", L"O", 
/*50-57*/L"P", L"Q", L"R", L"S", L"T", L"U", L"V", L"W", 
/*58-5F*/L"X", L"Y", L"Z", L"Left Wnd", L"Right Wnd", L"Applications", L"Reserved", L"Sleep", 
/*60-67*/L"Num 0", L"Num 1", L"Num 2", L"Num 3", L"Num 4", L"Num 5", L"Num 6", L"Num 7", 
/*68-6F*/L"Num 8", L"Num 9", L"Multiply", L"Add", L"Separator", L"Subtract", L"Decimal", L"Divide", 
/*70-77*/L"F1", L"F2", L"F3", L"F4", L"F5", L"F6", L"F7", L"F8", 
/*78-7F*/L"F9", L"F10", L"F11", L"F12", L"F13", L"F14", L"F15", L"F16",
/*80-87*/L"F17", L"F18", L"F19", L"F20", L"F21", L"F22", L"F23", L"F24", 
/*88-8F*/L"Unassigned", L"Unassigned", L"Unassigned", L"Unassigned", L"Unassigned", L"Unassigned", L"Unassigned", L"Unassigned", 
/*90-97*/L"Num Lock", L"Scroll Lock", L"OEM Specific", L"OEM Specific", L"OEM Specific", L"OEM Specific", L"OEM Specific", L"Unassigned", 
/*98-9F*/L"Unassigned", L"Unassigned", L"Unassigned", L"Unassigned", L"Unassigned", L"Unassigned", L"Unassigned", L"Unassigned", 
/*A0-A7*/L"Left shift", L"Right shift", L"Left control", L"Right control", L"Left menu", L"Right menu", L"Browser Back", L"Browser Forward", 
/*A8-AF*/L"Browser Refresh", L"Browser Stop", L"Browser Search", L"Browser Favorites", L"Browser Start", L"Volume Mute", L"Volume Down", L"Volume Up", 
/*B0-B7*/L"Next Track", L"Previous Track", L"Stop Media", L"Play/Pause Media",  L"Start Mail", L"Select Media", L"Start Application 1", L"Start Application 2",
/*B8-BF*/L"Reserved", L"Reserved", L"OEM_1 ; :", L"OEM_PLUS +", L"OEM_COMMA ,", L"OEM_MINUS -", L"OEM_PERIOD .", L"OEM_2 /?", 
/*C0-C7*/L"OEM_3  ~", L"Reserved", L"Reserved", L"Reserved", L"Reserved", L"Reserved", L"Reserved", L"Reserved", 
/*C8-CF*/L"Reserved", L"Reserved", L"Reserved", L"Reserved", L"Reserved", L"Reserved", L"Reserved", L"Reserved", 
/*D0-D7*/L"Reserved", L"Reserved", L"Reserved", L"Reserved", L"Reserved", L"Reserved", L"Reserved", L"Reserved", 
/*D8-DF*/L"Unassigned", L"Unassigned", L"Unassigned", L"OEM_4  [{", L"OEM_5  \\|", L"OEM_6  ] }", L"OEM_7  ' \"", L"OEM_8", 
/*E0-E7*/L"Reserved", L"OEM Specific", L"OEM_102", L"OEM Specific", L"OEM Specific", L"Process", L"OEM Specific", L"Packet", 
/*E8-EF*/L"Unassigned", L"OEM Specific", L"OEM Specific", L"OEM Specific", L"OEM Specific", L"OEM Specific", L"OEM Specific", L"OEM Specific", 
/*F0-F7*/L"OEM Specific", L"OEM Specific", L"OEM Specific", L"OEM Specific", L"OEM Specific", L"OEM Specific", L"Attn", L"CrSel", 
/*F8-FF*/L"ExSel", L"Erase", L"Play", L"Zoom", L"Reserved", L"PA1", L"Clear"
};

const int VKeyGoodness[256] = {
//	      0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
/*00-0F*/ 2, 2, 2, 0, 2, 1, 1, 1, 2, 2, 2, 2, 1, 2, 1, 1,
/*10-1F*/ 2, 2, 2, 0, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 
/*20-2F*/ 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2,
/*30-3F*/ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1,
/*40-4F*/ 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
/*50-5F*/ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
/*60-6F*/ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
/*70-7F*/ 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*80-8F*/ 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 
/*90-9F*/ 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
/*A0-AF*/ 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1,
/*B0-BF*/ 1, 1, 1, 1, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,
/*C0-CF*/ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
/*D0-DF*/ 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 2, 2, 2, 2, 1,
/*E0-EF*/ 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1,
/*F0-FF*/ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1,
};




