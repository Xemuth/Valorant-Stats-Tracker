#ifndef _ValorantStatsTracker_KeyMap_h_
#define _ValorantStatsTracker_KeyMap_h_
#include <Core/Core.h>
struct WinKey{
	Upp::String KeyName ="";
	int KeyCode;
	
	WinKey(const Upp::String& KN, int code){
		KeyName = KN;
		KeyCode = code;
	}
};
namespace Upp{
const Upp::ArrayMap<dword,WinKey> AllKeys{
		{K_BACK ,WinKey("BACK", VK_BACK )},
		{K_BACKSPACE ,WinKey("BACKSPACE", VK_BACK )},
		{K_TAB ,WinKey("TAB", 9)},
		{K_SPACE ,WinKey("SPACE", 32)},
		{K_RETURN ,WinKey("RETURN", 13)},
		{K_ENTER ,WinKey("ENTER", K_RETURN)},
		{K_SHIFT_KEY ,WinKey("SHIFT_KEY", VK_SHIFT )},
		{K_CTRL_KEY ,WinKey("CTRL_KEY", VK_CONTROL )},
		{K_ALT_KEY ,WinKey("ALT_KEY", VK_MENU )},
		{K_CAPSLOCK ,WinKey("CAPSLOCK", VK_CAPITAL )},
		{K_ESCAPE ,WinKey("ESCAPE", VK_ESCAPE )},
		{K_PRIOR ,WinKey("PRIOR", VK_PRIOR )},
		{K_PAGEUP ,WinKey("PAGEUP", VK_PRIOR )},
		{K_NEXT ,WinKey("NEXT", VK_NEXT )},
		{K_PAGEDOWN ,WinKey("PAGEDOWN", VK_NEXT )},
		{K_END ,WinKey("END", VK_END )},
		{K_HOME ,WinKey("HOME", VK_HOME )},
		{K_LEFT ,WinKey("LEFT", VK_LEFT )},
		{K_UP ,WinKey("UP", VK_UP )},
		{K_RIGHT ,WinKey("RIGHT", VK_RIGHT )},
		{K_DOWN ,WinKey("DOWN", VK_DOWN )},
		{K_INSERT ,WinKey("INSERT", VK_INSERT )},
		{K_DELETE ,WinKey("DELETE", VK_DELETE )},
		{K_NUMPAD0 ,WinKey("NUMPAD0", VK_NUMPAD0 )},
		{K_NUMPAD1 ,WinKey("NUMPAD1", VK_NUMPAD1 )},
		{K_NUMPAD2 ,WinKey("NUMPAD2", VK_NUMPAD2 )},
		{K_NUMPAD3 ,WinKey("NUMPAD3", VK_NUMPAD3 )},
		{K_NUMPAD4 ,WinKey("NUMPAD4", VK_NUMPAD4 )},
		{K_NUMPAD5 ,WinKey("NUMPAD5", VK_NUMPAD5 )},
		{K_NUMPAD6 ,WinKey("NUMPAD6", VK_NUMPAD6 )},
		{K_NUMPAD7 ,WinKey("NUMPAD7", VK_NUMPAD7 )},
		{K_NUMPAD8 ,WinKey("NUMPAD8", VK_NUMPAD8 )},
		{K_NUMPAD9 ,WinKey("NUMPAD9", VK_NUMPAD9 )},
		{K_MULTIPLY ,WinKey("MULTIPLY", VK_MULTIPLY )},
		{K_ADD ,WinKey("ADD", VK_ADD )},
		{K_SEPARATOR ,WinKey("SEPARATOR", VK_SEPARATOR )},
		{K_SUBTRACT ,WinKey("SUBTRACT", VK_SUBTRACT )},
		{K_DECIMAL ,WinKey("DECIMAL", VK_DECIMAL )},
		{K_DIVIDE ,WinKey("DIVIDE", VK_DIVIDE )},
		{K_SCROLL ,WinKey("SCROLL", VK_SCROLL )},
		{K_PLUS ,WinKey("PLUS", VK_OEM_PLUS )},
		{K_MINUS ,WinKey("MINUS", VK_OEM_MINUS )},
		{K_COMMA ,WinKey("COMMA", VK_OEM_COMMA )},
		{K_PERIOD ,WinKey("PERIOD", VK_OEM_PERIOD )},
		{K_SEMICOLON ,WinKey("SEMICOLON", VK_OEM_1 )},
		{K_SLASH ,WinKey("SLASH", VK_OEM_2 )},
		{K_GRAVE ,WinKey("GRAVE", VK_OEM_3 )},
		{K_LBRACKET ,WinKey("LBRACKET", VK_OEM_4 )},
		{K_BACKSLASH ,WinKey("BACKSLASH", VK_OEM_5 )},
		{K_RBRACKET ,WinKey("RBRACKET", VK_OEM_6 )},
		{K_QUOTEDBL ,WinKey("QUOTEDBL", VK_OEM_7 )},
		{K_F1 ,WinKey("F1", VK_F1 )},
		{K_F2 ,WinKey("F2", VK_F2 )},
		{K_F3 ,WinKey("F3", VK_F3 )},
		{K_F4 ,WinKey("F4", VK_F4 )},
		{K_F5 ,WinKey("F5", VK_F5 )},
		{K_F6 ,WinKey("F6", VK_F6 )},
		{K_F7 ,WinKey("F7", VK_F7 )},
		{K_F8 ,WinKey("F8", VK_F8 )},
		{K_F9 ,WinKey("F9", VK_F9 )},
		{K_F10 ,WinKey("F10", VK_F10 )},
		{K_F11 ,WinKey("F11", VK_F11 )},
		{K_F12 ,WinKey("F12", VK_F12 )},
		{K_A ,WinKey("A", 'A' )},
		{K_B ,WinKey("B", 'B' )},
		{K_C ,WinKey("C", 'C' )},
		{K_D ,WinKey("D", 'D' )},
		{K_E ,WinKey("E", 'E' )},
		{K_F ,WinKey("F", 'F' )},
		{K_G ,WinKey("G", 'G' )},
		{K_H ,WinKey("H", 'H' )},
		{K_I ,WinKey("I", 'I' )},
		{K_J ,WinKey("J", 'J' )},
		{K_K ,WinKey("K", 'K' )},
		{K_L ,WinKey("L", 'L' )},
		{K_M ,WinKey("M", 'M' )},
		{K_N ,WinKey("N", 'N' )},
		{K_O ,WinKey("O", 'O' )},
		{K_P ,WinKey("P", 'P' )},
		{K_Q ,WinKey("Q", 'Q' )},
		{K_R ,WinKey("R", 'R' )},
		{K_S ,WinKey("S", 'S' )},
		{K_T ,WinKey("T", 'T' )},
		{K_U ,WinKey("U", 'U' )},
		{K_V ,WinKey("V", 'V' )},
		{K_W ,WinKey("W", 'W' )},
		{K_X ,WinKey("X", 'X' )},
		{K_Y ,WinKey("Y", 'Y' )},
		{K_Z ,WinKey("Z", 'Z' )},
		{K_0 ,WinKey("0", '0' )},
		{K_1 ,WinKey("1", '1' )},
		{K_2 ,WinKey("2", '2' )},
		{K_3 ,WinKey("3", '3' )},
		{K_4 ,WinKey("4", '4' )},
		{K_5 ,WinKey("5", '5' )},
		{K_6 ,WinKey("6", '6' )},
		{K_7 ,WinKey("7", '7' )},
		{K_8 ,WinKey("8", '8' )},
		{K_9 ,WinKey("9", '9' )}
	};
}






#endif
