#define _WIN32_WINNT 0x0501

#include "WinMsgName.h"

#include <CommCtrl.h>

CString Get_WM_XXX_Str( UINT uMsg );
CString Get_EM_XXX_Str( UINT uMsg );
CString Get_BM_XXX_Str( UINT uMsg );
CString Get_WH_XXX_Str( UINT uMsg );
CString Get_NM_XXX_Str( UINT uMsg );
CString Get_LVN_XXX_Str( UINT uMsg );

CString getWinMsgStr( UINT uMsg )
{
	CString (*pfaGet_XX_XXX_Str[])(UINT) = { Get_WM_XXX_Str, Get_EM_XXX_Str, 
		Get_BM_XXX_Str, Get_WH_XXX_Str, Get_NM_XXX_Str, Get_LVN_XXX_Str, 0 };

	for( int i = 0; pfaGet_XX_XXX_Str[i] != 0; i++ ) {
		CString msgStr = (pfaGet_XX_XXX_Str[i])( uMsg );
		if( !msgStr.IsEmpty() ) {
			return msgStr;
		}
	}
	return T("???  ") + Int2Str( uMsg ) +  T("  UNDEFINED MESSAGE ???");
}

CString getWinMsgStrFormated( UINT uMsg, int minStrLen = 20 )
{
	CString res = getWinMsgStr( uMsg );
	while( res.Length() < minStrLen ) {
		res += T(" ");
	}
	return res;
}

CStr<char> GetWinMsgNameA( UINT uMsg )
{
	return GetStrA( getWinMsgStrFormated( uMsg ) );	
}

CStr<wchar_t> GetWinMsgNameW( UINT uMsg )
{
	return GetStrW( getWinMsgStrFormated( uMsg ) );
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//#include <WinUser.h>
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CString Get_WM_XXX_Str( UINT uMsg )
{
	switch(uMsg) {
		case WM_NULL: return T("WM_NULL");
		case WM_CREATE: return T("WM_CREATE");
		case WM_DESTROY: return T("WM_DESTROY");
		case WM_MOVE: return T("WM_MOVE");
		case WM_SIZE: return T("WM_SIZE");
		case WM_ACTIVATE: return T("WM_ACTIVATE");
		case WM_SETFOCUS: return T("WM_SETFOCUS");
		case WM_KILLFOCUS: return T("WM_KILLFOCUS");
		case WM_ENABLE: return T("WM_ENABLE");
		case WM_SETREDRAW: return T("WM_SETREDRAW");
		case WM_SETTEXT: return T("WM_SETTEXT");
		case WM_GETTEXT: return T("WM_GETTEXT");
		case WM_GETTEXTLENGTH: return T("WM_GETTEXTLENGTH");
		case WM_PAINT: return T("WM_PAINT");
		case WM_CLOSE: return T("WM_CLOSE");
		case WM_QUERYENDSESSION: return T("WM_QUERYENDSESSION");
		case WM_QUERYOPEN: return T("WM_QUERYOPEN");
		case WM_ENDSESSION: return T("WM_ENDSESSION");
		case WM_QUIT: return T("WM_QUIT");
		case WM_ERASEBKGND: return T("WM_ERASEBKGND");
		case WM_SYSCOLORCHANGE: return T("WM_SYSCOLORCHANGE");
		case WM_SHOWWINDOW: return T("WM_SHOWWINDOW");
//		case WM_WININICHANGE: return T("WM_WININICHANGE");
		case WM_SETTINGCHANGE: return T("WM_SETTINGCHANGE");
		case WM_DEVMODECHANGE: return T("WM_DEVMODECHANGE");
		case WM_ACTIVATEAPP: return T("WM_ACTIVATEAPP");
		case WM_FONTCHANGE: return T("WM_FONTCHANGE");
		case WM_TIMECHANGE: return T("WM_TIMECHANGE");
		case WM_CANCELMODE: return T("WM_CANCELMODE");
		case WM_SETCURSOR: return T("WM_SETCURSOR");
		case WM_MOUSEACTIVATE: return T("WM_MOUSEACTIVATE");
		case WM_CHILDACTIVATE: return T("WM_CHILDACTIVATE");
		case WM_QUEUESYNC: return T("WM_QUEUESYNC");
		case WM_GETMINMAXINFO: return T("WM_GETMINMAXINFO");
		case WM_PAINTICON: return T("WM_PAINTICON");
		case WM_ICONERASEBKGND: return T("WM_ICONERASEBKGND");
		case WM_NEXTDLGCTL: return T("WM_NEXTDLGCTL");
		case WM_SPOOLERSTATUS: return T("WM_SPOOLERSTATUS");
		case WM_DRAWITEM: return T("WM_DRAWITEM");
		case WM_MEASUREITEM: return T("WM_MEASUREITEM");
		case WM_DELETEITEM: return T("WM_DELETEITEM");
		case WM_VKEYTOITEM: return T("WM_VKEYTOITEM");
		case WM_CHARTOITEM: return T("WM_CHARTOITEM");
		case WM_SETFONT: return T("WM_SETFONT");
		case WM_GETFONT: return T("WM_GETFONT");
		case WM_SETHOTKEY: return T("WM_SETHOTKEY");
		case WM_GETHOTKEY: return T("WM_GETHOTKEY");
		case WM_QUERYDRAGICON: return T("WM_QUERYDRAGICON");
		case WM_COMPAREITEM: return T("WM_COMPAREITEM");
		case WM_GETOBJECT: return T("WM_GETOBJECT");
		case WM_COMPACTING: return T("WM_COMPACTING");
		case WM_COMMNOTIFY: return T("WM_COMMNOTIFY");
		case WM_WINDOWPOSCHANGING: return T("WM_WINDOWPOSCHANGING");
		case WM_WINDOWPOSCHANGED: return T("WM_WINDOWPOSCHANGED");
		case WM_POWER: return T("WM_POWER");
		case WM_COPYDATA: return T("WM_COPYDATA");
		case WM_CANCELJOURNAL: return T("WM_CANCELJOURNAL");
		case WM_NOTIFY: return T("WM_NOTIFY");
		case WM_INPUTLANGCHANGEREQUEST: return T("WM_INPUTLANGCHANGEREQUEST");
		case WM_INPUTLANGCHANGE: return T("WM_INPUTLANGCHANGE");
		case WM_TCARD: return T("WM_TCARD");
		case WM_HELP: return T("WM_HELP");
		case WM_USERCHANGED: return T("WM_USERCHANGED");
		case WM_NOTIFYFORMAT: return T("WM_NOTIFYFORMAT");
		case WM_CONTEXTMENU: return T("WM_CONTEXTMENU");
		case WM_STYLECHANGING: return T("WM_STYLECHANGING");
		case WM_STYLECHANGED: return T("WM_STYLECHANGED");
		case WM_DISPLAYCHANGE: return T("WM_DISPLAYCHANGE");
		case WM_GETICON: return T("WM_GETICON");
		case WM_SETICON: return T("WM_SETICON");
		case WM_NCCREATE: return T("WM_NCCREATE");
		case WM_NCDESTROY: return T("WM_NCDESTROY");
		case WM_NCCALCSIZE: return T("WM_NCCALCSIZE");
		case WM_NCHITTEST: return T("WM_NCHITTEST");
		case WM_NCPAINT: return T("WM_NCPAINT");
		case WM_NCACTIVATE: return T("WM_NCACTIVATE");
		case WM_GETDLGCODE: return T("WM_GETDLGCODE");
		case WM_SYNCPAINT: return T("WM_SYNCPAINT");
		case WM_NCMOUSEMOVE: return T("WM_NCMOUSEMOVE");
		case WM_NCLBUTTONDOWN: return T("WM_NCLBUTTONDOWN");
		case WM_NCLBUTTONUP: return T("WM_NCLBUTTONUP");
		case WM_NCLBUTTONDBLCLK: return T("WM_NCLBUTTONDBLCLK");
		case WM_NCRBUTTONDOWN: return T("WM_NCRBUTTONDOWN");
		case WM_NCRBUTTONUP: return T("WM_NCRBUTTONUP");
		case WM_NCRBUTTONDBLCLK: return T("WM_NCRBUTTONDBLCLK");
		case WM_NCMBUTTONDOWN: return T("WM_NCMBUTTONDOWN");
		case WM_NCMBUTTONUP: return T("WM_NCMBUTTONUP");
		case WM_NCMBUTTONDBLCLK: return T("WM_NCMBUTTONDBLCLK");
		case WM_NCXBUTTONDOWN: return T("WM_NCXBUTTONDOWN");
		case WM_NCXBUTTONUP: return T("WM_NCXBUTTONUP");
		case WM_NCXBUTTONDBLCLK: return T("WM_NCXBUTTONDBLCLK");
		case WM_INPUT: return T("WM_INPUT");
//		case WM_KEYFIRST: return T("WM_KEYFIRST");
		case WM_KEYDOWN: return T("WM_KEYDOWN");
		case WM_KEYUP: return T("WM_KEYUP");
		case WM_CHAR: return T("WM_CHAR");
		case WM_DEADCHAR: return T("WM_DEADCHAR");
		case WM_SYSKEYDOWN: return T("WM_SYSKEYDOWN");
		case WM_SYSKEYUP: return T("WM_SYSKEYUP");
		case WM_SYSCHAR: return T("WM_SYSCHAR");
		case WM_SYSDEADCHAR: return T("WM_SYSDEADCHAR");
		case WM_UNICHAR: return T("WM_UNICHAR");
//		case WM_KEYLAST: return T("WM_KEYLAST");
//		case WM_KEYLAST: return T("WM_KEYLAST");
		case WM_IME_STARTCOMPOSITION: return T("WM_IME_STARTCOMPOSITION");
		case WM_IME_ENDCOMPOSITION: return T("WM_IME_ENDCOMPOSITION");
		case WM_IME_COMPOSITION: return T("WM_IME_COMPOSITION");
//		case WM_IME_KEYLAST: return T("WM_IME_KEYLAST");
		case WM_INITDIALOG: return T("WM_INITDIALOG");
		case WM_COMMAND: return T("WM_COMMAND");
		case WM_SYSCOMMAND: return T("WM_SYSCOMMAND");
		case WM_TIMER: return T("WM_TIMER");
		case WM_HSCROLL: return T("WM_HSCROLL");
		case WM_VSCROLL: return T("WM_VSCROLL");
		case WM_INITMENU: return T("WM_INITMENU");
		case WM_INITMENUPOPUP: return T("WM_INITMENUPOPUP");
		case WM_MENUSELECT: return T("WM_MENUSELECT");
		case WM_MENUCHAR: return T("WM_MENUCHAR");
		case WM_ENTERIDLE: return T("WM_ENTERIDLE");
		case WM_MENURBUTTONUP: return T("WM_MENURBUTTONUP");
		case WM_MENUDRAG: return T("WM_MENUDRAG");
		case WM_MENUGETOBJECT: return T("WM_MENUGETOBJECT");
		case WM_UNINITMENUPOPUP: return T("WM_UNINITMENUPOPUP");
		case WM_MENUCOMMAND: return T("WM_MENUCOMMAND");
		case WM_CHANGEUISTATE: return T("WM_CHANGEUISTATE");
		case WM_UPDATEUISTATE: return T("WM_UPDATEUISTATE");
		case WM_QUERYUISTATE: return T("WM_QUERYUISTATE");
		case WM_CTLCOLORMSGBOX: return T("WM_CTLCOLORMSGBOX");
		case WM_CTLCOLOREDIT: return T("WM_CTLCOLOREDIT");
		case WM_CTLCOLORLISTBOX: return T("WM_CTLCOLORLISTBOX");
		case WM_CTLCOLORBTN: return T("WM_CTLCOLORBTN");
		case WM_CTLCOLORDLG: return T("WM_CTLCOLORDLG");
		case WM_CTLCOLORSCROLLBAR: return T("WM_CTLCOLORSCROLLBAR");
		case WM_CTLCOLORSTATIC: return T("WM_CTLCOLORSTATIC");
//		case WM_MOUSEFIRST: return T("WM_MOUSEFIRST");
		case WM_MOUSEMOVE: return T("WM_MOUSEMOVE");
		case WM_LBUTTONDOWN: return T("WM_LBUTTONDOWN");
		case WM_LBUTTONUP: return T("WM_LBUTTONUP");
		case WM_LBUTTONDBLCLK: return T("WM_LBUTTONDBLCLK");
		case WM_RBUTTONDOWN: return T("WM_RBUTTONDOWN");
		case WM_RBUTTONUP: return T("WM_RBUTTONUP");
		case WM_RBUTTONDBLCLK: return T("WM_RBUTTONDBLCLK");
		case WM_MBUTTONDOWN: return T("WM_MBUTTONDOWN");
		case WM_MBUTTONUP: return T("WM_MBUTTONUP");
		case WM_MBUTTONDBLCLK: return T("WM_MBUTTONDBLCLK");
		case WM_MOUSEWHEEL: return T("WM_MOUSEWHEEL");
		case WM_XBUTTONDOWN: return T("WM_XBUTTONDOWN");
		case WM_XBUTTONUP: return T("WM_XBUTTONUP");
		case WM_XBUTTONDBLCLK: return T("WM_XBUTTONDBLCLK");
//		case WM_MOUSELAST: return T("WM_MOUSELAST");
//		case WM_MOUSELAST: return T("WM_MOUSELAST");
//		case WM_MOUSELAST: return T("WM_MOUSELAST");
		case WM_PARENTNOTIFY: return T("WM_PARENTNOTIFY");
		case WM_ENTERMENULOOP: return T("WM_ENTERMENULOOP");
		case WM_EXITMENULOOP: return T("WM_EXITMENULOOP");
		case WM_NEXTMENU: return T("WM_NEXTMENU");
		case WM_SIZING: return T("WM_SIZING");
		case WM_CAPTURECHANGED: return T("WM_CAPTURECHANGED");
		case WM_MOVING: return T("WM_MOVING");
		case WM_POWERBROADCAST: return T("WM_POWERBROADCAST");
		case WM_DEVICECHANGE: return T("WM_DEVICECHANGE");
		case WM_MDICREATE: return T("WM_MDICREATE");
		case WM_MDIDESTROY: return T("WM_MDIDESTROY");
		case WM_MDIACTIVATE: return T("WM_MDIACTIVATE");
		case WM_MDIRESTORE: return T("WM_MDIRESTORE");
		case WM_MDINEXT: return T("WM_MDINEXT");
		case WM_MDIMAXIMIZE: return T("WM_MDIMAXIMIZE");
		case WM_MDITILE: return T("WM_MDITILE");
		case WM_MDICASCADE: return T("WM_MDICASCADE");
		case WM_MDIICONARRANGE: return T("WM_MDIICONARRANGE");
		case WM_MDIGETACTIVE: return T("WM_MDIGETACTIVE");
		case WM_MDISETMENU: return T("WM_MDISETMENU");
		case WM_ENTERSIZEMOVE: return T("WM_ENTERSIZEMOVE");
		case WM_EXITSIZEMOVE: return T("WM_EXITSIZEMOVE");
		case WM_DROPFILES: return T("WM_DROPFILES");
		case WM_MDIREFRESHMENU: return T("WM_MDIREFRESHMENU");
		case WM_IME_SETCONTEXT: return T("WM_IME_SETCONTEXT");
		case WM_IME_NOTIFY: return T("WM_IME_NOTIFY");
		case WM_IME_CONTROL: return T("WM_IME_CONTROL");
		case WM_IME_COMPOSITIONFULL: return T("WM_IME_COMPOSITIONFULL");
		case WM_IME_SELECT: return T("WM_IME_SELECT");
		case WM_IME_CHAR: return T("WM_IME_CHAR");
		case WM_IME_REQUEST: return T("WM_IME_REQUEST");
		case WM_IME_KEYDOWN: return T("WM_IME_KEYDOWN");
		case WM_IME_KEYUP: return T("WM_IME_KEYUP");
		case WM_MOUSEHOVER: return T("WM_MOUSEHOVER");
		case WM_MOUSELEAVE: return T("WM_MOUSELEAVE");
		case WM_NCMOUSEHOVER: return T("WM_NCMOUSEHOVER");
		case WM_NCMOUSELEAVE: return T("WM_NCMOUSELEAVE");
		case WM_WTSSESSION_CHANGE: return T("WM_WTSSESSION_CHANGE");
		case WM_TABLET_FIRST: return T("WM_TABLET_FIRST");
		case WM_TABLET_LAST: return T("WM_TABLET_LAST");
		case WM_CUT: return T("WM_CUT");
		case WM_COPY: return T("WM_COPY");
		case WM_PASTE: return T("WM_PASTE");
		case WM_CLEAR: return T("WM_CLEAR");
		case WM_UNDO: return T("WM_UNDO");
		case WM_RENDERFORMAT: return T("WM_RENDERFORMAT");
		case WM_RENDERALLFORMATS: return T("WM_RENDERALLFORMATS");
		case WM_DESTROYCLIPBOARD: return T("WM_DESTROYCLIPBOARD");
		case WM_DRAWCLIPBOARD: return T("WM_DRAWCLIPBOARD");
		case WM_PAINTCLIPBOARD: return T("WM_PAINTCLIPBOARD");
		case WM_VSCROLLCLIPBOARD: return T("WM_VSCROLLCLIPBOARD");
		case WM_SIZECLIPBOARD: return T("WM_SIZECLIPBOARD");
		case WM_ASKCBFORMATNAME: return T("WM_ASKCBFORMATNAME");
		case WM_CHANGECBCHAIN: return T("WM_CHANGECBCHAIN");
		case WM_HSCROLLCLIPBOARD: return T("WM_HSCROLLCLIPBOARD");
		case WM_QUERYNEWPALETTE: return T("WM_QUERYNEWPALETTE");
		case WM_PALETTEISCHANGING: return T("WM_PALETTEISCHANGING");
		case WM_PALETTECHANGED: return T("WM_PALETTECHANGED");
		case WM_HOTKEY: return T("WM_HOTKEY");
		case WM_PRINT: return T("WM_PRINT");
		case WM_PRINTCLIENT: return T("WM_PRINTCLIENT");
		case WM_APPCOMMAND: return T("WM_APPCOMMAND");
		case WM_THEMECHANGED: return T("WM_THEMECHANGED");
		case WM_HANDHELDFIRST: return T("WM_HANDHELDFIRST");
		case WM_HANDHELDLAST: return T("WM_HANDHELDLAST");
		case WM_AFXFIRST: return T("WM_AFXFIRST");
		case WM_AFXLAST: return T("WM_AFXLAST");
		case WM_PENWINFIRST: return T("WM_PENWINFIRST");
		case WM_PENWINLAST: return T("WM_PENWINLAST");
		case WM_APP: return T("WM_APP");
		case WM_USER: return T("WM_USER");
	}
	return T("");
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CString Get_EM_XXX_Str( UINT uMsg )
{
	switch(uMsg) {
		case EM_GETSEL: return T("EM_GETSEL");
		case EM_SETSEL: return T("EM_SETSEL");
		case EM_GETRECT: return T("EM_GETRECT");
		case EM_SETRECT: return T("EM_SETRECT");
		case EM_SETRECTNP: return T("EM_SETRECTNP");
		case EM_SCROLL: return T("EM_SCROLL");
		case EM_LINESCROLL: return T("EM_LINESCROLL");
		case EM_SCROLLCARET: return T("EM_SCROLLCARET");
		case EM_GETMODIFY: return T("EM_GETMODIFY");
		case EM_SETMODIFY: return T("EM_SETMODIFY");
		case EM_GETLINECOUNT: return T("EM_GETLINECOUNT");
		case EM_LINEINDEX: return T("EM_LINEINDEX");
		case EM_SETHANDLE: return T("EM_SETHANDLE");
		case EM_GETHANDLE: return T("EM_GETHANDLE");
		case EM_GETTHUMB: return T("EM_GETTHUMB");
		case EM_LINELENGTH: return T("EM_LINELENGTH");
		case EM_REPLACESEL: return T("EM_REPLACESEL");
		case EM_GETLINE: return T("EM_GETLINE");
		case EM_LIMITTEXT: return T("EM_LIMITTEXT");
		case EM_CANUNDO: return T("EM_CANUNDO");
		case EM_UNDO: return T("EM_UNDO");
		case EM_FMTLINES: return T("EM_FMTLINES");
		case EM_LINEFROMCHAR: return T("EM_LINEFROMCHAR");
		case EM_SETTABSTOPS: return T("EM_SETTABSTOPS");
		case EM_SETPASSWORDCHAR: return T("EM_SETPASSWORDCHAR");
		case EM_EMPTYUNDOBUFFER: return T("EM_EMPTYUNDOBUFFER");
		case EM_GETFIRSTVISIBLELINE: return T("EM_GETFIRSTVISIBLELINE");
		case EM_SETREADONLY: return T("EM_SETREADONLY");
		case EM_SETWORDBREAKPROC: return T("EM_SETWORDBREAKPROC");
		case EM_GETWORDBREAKPROC: return T("EM_GETWORDBREAKPROC");
		case EM_GETPASSWORDCHAR: return T("EM_GETPASSWORDCHAR");
		case EM_SETMARGINS: return T("EM_SETMARGINS");
		case EM_GETMARGINS: return T("EM_GETMARGINS");
//		case EM_SETLIMITTEXT: return T("EM_SETLIMITTEXT");
		case EM_GETLIMITTEXT: return T("EM_GETLIMITTEXT");
		case EM_POSFROMCHAR: return T("EM_POSFROMCHAR");
		case EM_CHARFROMPOS: return T("EM_CHARFROMPOS");
		case EM_SETIMESTATUS: return T("EM_SETIMESTATUS");
		case EM_GETIMESTATUS: return T("EM_GETIMESTATUS");
	}
	return T("");
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CString Get_BM_XXX_Str( UINT uMsg )
{
	switch(uMsg) {
		case BM_GETCHECK: return T("BM_GETCHECK");
		case BM_SETCHECK: return T("BM_SETCHECK");
		case BM_GETSTATE: return T("BM_GETSTATE");
		case BM_SETSTATE: return T("BM_SETSTATE");
		case BM_SETSTYLE: return T("BM_SETSTYLE");
		case BM_CLICK: return T("BM_CLICK");
		case BM_GETIMAGE: return T("BM_GETIMAGE");
		case BM_SETIMAGE: return T("BM_SETIMAGE");
	}
	return T("");
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CString Get_WH_XXX_Str( UINT uMsg )
{
	switch(uMsg) {
//		case WH_MIN: return T("WH_MIN");
		case WH_MSGFILTER: return T("WH_MSGFILTER");
		case WH_JOURNALRECORD: return T("WH_JOURNALRECORD");
		case WH_JOURNALPLAYBACK: return T("WH_JOURNALPLAYBACK");
		case WH_KEYBOARD: return T("WH_KEYBOARD");
		case WH_GETMESSAGE: return T("WH_GETMESSAGE");
		case WH_CALLWNDPROC: return T("WH_CALLWNDPROC");
		case WH_CBT: return T("WH_CBT");
		case WH_SYSMSGFILTER: return T("WH_SYSMSGFILTER");
		case WH_MOUSE: return T("WH_MOUSE");
//		case WH_HARDWARE: return T("WH_HARDWARE");
		case WH_DEBUG: return T("WH_DEBUG");
		case WH_SHELL: return T("WH_SHELL");
		case WH_FOREGROUNDIDLE: return T("WH_FOREGROUNDIDLE");
		case WH_CALLWNDPROCRET: return T("WH_CALLWNDPROCRET");
		case WH_KEYBOARD_LL: return T("WH_KEYBOARD_LL");
		case WH_MOUSE_LL: return T("WH_MOUSE_LL");
//		case WH_MAX: return T("WH_MAX");
//		case WH_MINHOOK: return T("WH_MINHOOK");
//		case WH_MAXHOOK: return T("WH_MAXHOOK");
	}
	return T("");
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//#include <CommCtrl.h>
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CString Get_NM_XXX_Str( UINT uMsg )
{
	switch(uMsg) {
		case NM_OUTOFMEMORY: return T("NM_OUTOFMEMORY");
		case NM_CLICK: return T("NM_CLICK");
		case NM_DBLCLK: return T("NM_DBLCLK");
		case NM_RETURN: return T("NM_RETURN");
		case NM_RCLICK: return T("NM_RCLICK");
		case NM_RDBLCLK: return T("NM_RDBLCLK");
		case NM_SETFOCUS: return T("NM_SETFOCUS");
		case NM_KILLFOCUS: return T("NM_KILLFOCUS");
		case NM_CUSTOMDRAW: return T("NM_CUSTOMDRAW");
		case NM_HOVER: return T("NM_HOVER");
		case NM_NCHITTEST: return T("NM_NCHITTEST");
		case NM_KEYDOWN: return T("NM_KEYDOWN");
		case NM_RELEASEDCAPTURE: return T("NM_RELEASEDCAPTURE");
		case NM_SETCURSOR: return T("NM_SETCURSOR");
		case NM_CHAR: return T("NM_CHAR");
		case NM_TOOLTIPSCREATED: return T("NM_TOOLTIPSCREATED");
		case NM_LDOWN: return T("NM_LDOWN");
		case NM_RDOWN: return T("NM_RDOWN");
		case NM_THEMECHANGED: return T("NM_THEMECHANGED");
		case NM_FIRST: return T("NM_FIRST");
		case NM_LAST: return T("NM_LAST");
//		case NM_UPDOWN: return T("NM_UPDOWN");
//		case NM_LISTVIEW: return T("NM_LISTVIEW");
//		case NM_CACHEHINT: return T("NM_CACHEHINT");
//		case NM_FINDITEMA: return T("NM_FINDITEMA");
//		case NM_FINDITEMW: return T("NM_FINDITEMW");
//		case NM_FINDITEM: return T("NM_FINDITEM");
//		case NM_FINDITEM: return T("NM_FINDITEM");
//		case NM_ODSTATECHANGE: return T("NM_ODSTATECHANGE");
//		case NM_TREEVIEWW: return T("NM_TREEVIEWW");
//		case NM_TREEVIEWA: return T("NM_TREEVIEWA");
//		case NM_TREEVIEW: return T("NM_TREEVIEW");
	}
	return T("");
}

CString Get_LVN_XXX_Str( UINT uMsg )
{
	switch(uMsg) {
//		case LVN_FIRST: return T("LVN_FIRST");
//		case LVN_LAST: return T("LVN_LAST");
		case LVN_ITEMCHANGING: return T("LVN_ITEMCHANGING");
		case LVN_ITEMCHANGED: return T("LVN_ITEMCHANGED");
		case LVN_INSERTITEM: return T("LVN_INSERTITEM");
		case LVN_DELETEITEM: return T("LVN_DELETEITEM");
		case LVN_DELETEALLITEMS: return T("LVN_DELETEALLITEMS");
		case LVN_BEGINLABELEDITA: return T("LVN_BEGINLABELEDITA");
		case LVN_BEGINLABELEDITW: return T("LVN_BEGINLABELEDITW");
		case LVN_ENDLABELEDITA: return T("LVN_ENDLABELEDITA");
		case LVN_ENDLABELEDITW: return T("LVN_ENDLABELEDITW");
		case LVN_COLUMNCLICK: return T("LVN_COLUMNCLICK");
		case LVN_BEGINDRAG: return T("LVN_BEGINDRAG");
		case LVN_BEGINRDRAG: return T("LVN_BEGINRDRAG");
		case LVN_ODCACHEHINT: return T("LVN_ODCACHEHINT");
		case LVN_ODFINDITEMA: return T("LVN_ODFINDITEMA");
		case LVN_ODFINDITEMW: return T("LVN_ODFINDITEMW");
		case LVN_ITEMACTIVATE: return T("LVN_ITEMACTIVATE");
		case LVN_ODSTATECHANGED: return T("LVN_ODSTATECHANGED");
//		case LVN_ODFINDITEM: return T("LVN_ODFINDITEM");
//		case LVN_ODFINDITEM: return T("LVN_ODFINDITEM");
		case LVN_HOTTRACK: return T("LVN_HOTTRACK");
		case LVN_GETDISPINFOA: return T("LVN_GETDISPINFOA");
		case LVN_GETDISPINFOW: return T("LVN_GETDISPINFOW");
		case LVN_SETDISPINFOA: return T("LVN_SETDISPINFOA");
		case LVN_SETDISPINFOW: return T("LVN_SETDISPINFOW");
//		case LVN_BEGINLABELEDIT: return T("LVN_BEGINLABELEDIT");
//		case LVN_ENDLABELEDIT: return T("LVN_ENDLABELEDIT");
//		case LVN_GETDISPINFO: return T("LVN_GETDISPINFO");
//		case LVN_SETDISPINFO: return T("LVN_SETDISPINFO");
//		case LVN_BEGINLABELEDIT: return T("LVN_BEGINLABELEDIT");
//		case LVN_ENDLABELEDIT: return T("LVN_ENDLABELEDIT");
//		case LVN_GETDISPINFO: return T("LVN_GETDISPINFO");
//		case LVN_SETDISPINFO: return T("LVN_SETDISPINFO");
		case LVN_KEYDOWN: return T("LVN_KEYDOWN");
		case LVN_MARQUEEBEGIN: return T("LVN_MARQUEEBEGIN");
		case LVN_GETINFOTIPA: return T("LVN_GETINFOTIPA");
		case LVN_GETINFOTIPW: return T("LVN_GETINFOTIPW");
//		case LVN_GETINFOTIP: return T("LVN_GETINFOTIP");
//		case LVN_GETINFOTIP: return T("LVN_GETINFOTIP");
		case LVN_BEGINSCROLL: return T("LVN_BEGINSCROLL");
		case LVN_ENDSCROLL: return T("LVN_ENDSCROLL");
	}
	return T("");
}