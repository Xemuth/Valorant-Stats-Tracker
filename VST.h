#ifndef _VST_VST_h_
#define _VST_VST_h_
#include <CtrlLib/CtrlLib.h>
#include "KeyMap.h"
using namespace Upp;

#define LAYOUTFILE <ValorantStatsTracker/VST.lay>
#include <CtrlCore/lay.h>


/***
	Our config file is binary stored, it contain username/password hotkey config 
	window state
***/
struct VSTconfig {
	VSTconfig(){
		API_KEY="";
		
		key_endRound = VK_INSERT; // 0x2D
		key_kill = VK_HOME; // 0x24
		headshot_kill = VK_PRIOR; // 0x21
		game_started = VK_DELETE; // 0x2E
		game_ended = VK_END; // 0x23
	}
	
	Upp::String API_KEY ="";
	
	uint8_t key_endRound;
	uint8_t key_kill;
	uint8_t headshot_kill;
	uint8_t game_started;
	uint8_t game_ended;

	void Serialize(Stream& stream){
		stream % API_KEY % key_endRound % key_kill % headshot_kill % game_started % game_ended;
	}
};


class KeyChange : public WithInputChangeLayout<TopWindow>{
	dword * CharRegistered = nullptr;
	bool Key(dword key,int count){
		Cout() << (long) key << "," << count  << EOL;
		if(CharRegistered &&  *CharRegistered == 0) *CharRegistered = key;
		Close();
		return true;
	}
public:
	typedef KeyChange CLASSNAME;
	KeyChange(dword& _buffer){CharRegistered = &_buffer; CtrlLayout(*this, t_("Change key"));}
};

class VST : public WithVSTLayout<TopWindow> {
	enum{END_ROUND = 1, KILL = 2, HEADSHOT_KILL = 3, GAME_STARTED = 4, GAME_ENDED = 5};

	TrayIcon trayicon;
	WithConfigurationLayout<TopWindow> config;
	VSTconfig cfg;
	
	
	/*
	 Thread and boolean used to drive listener
	*/
	Thread keyboardListener;
	bool reloadConfig = false; //if set to true, keyboardListener will reload cfg and set it to false
	bool StopKeyListening = false; //when set to true, thread will stop key listening and will disable key event until it is changed to true
	bool KeyHaveBeenStop = true;
	bool ValorantIsStarted = false;
	
/*
	config function
*/
	void LoadCfg();
	void SetupHotKey();
	void ClearHotKey();
	void ProcessEvent(MSG& msg);
	void RoutineKeyboardListener();
/*
	Logic function
*/
	void Configure();
	void About();
	void Start();
	void Exit();

	void dummyFunc(){}
	void HideWin() { Hide(); }
/*
	All menu
*/
	void MainMenu(Bar& menu);
	void SettingsMenu(Bar& bar);
	void HelpMenu(Bar& bar);
	void TrayMenu(Bar& bar);

public:
	typedef VST CLASSNAME;

	VST();
};

#endif
