#ifndef _VST_VST_h_
#define _VST_VST_h_
#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <ValorantStatsTracker/VST.lay>
#include <CtrlCore/lay.h>


/***
	Our config file is binary stored, it contain username/password hotkey config 
	window state
***/
struct VSTconfig {
	VSTconfig(){
		username = "";
		password = "";
	
		key_endRound = VK_INSERT; // 0x2D
		key_kill = VK_HOME; // 0x24
		headshot_kill = VK_PRIOR; // 0x21
	
		sendMode = 0; // O:Direct  1:Timeout
		sendTime = 2; //Time in minute, can't be 0
	}
	
	Upp::String username;
	Upp::String password;
	
	uint8_t key_endRound;
	uint8_t key_kill;
	uint8_t headshot_kill;
	
	uint8_t sendMode;
	uint8_t sendTime;
	
	void Serialize(Stream& stream){
		stream % username % password % key_endRound % key_kill % headshot_kill % sendMode % sendTime;
	}
};


class VST : public WithVSTLayout<TopWindow> {
	enum{END_ROUND = 1, KILL = 2, HEADSHOT_KILL = 3};
	Thread keyboardListener;
	bool reloadConfig = false; //if set to true, keyboardListener will reload cfg and set it to false
	
	TrayIcon     trayicon;
	WithConfigurationLayout<TopWindow> config;
	VSTconfig cfg;
/*
	config function
*/
	void LoadCfg();
	void SetupHotKey();
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
