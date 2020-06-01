#ifndef _VST_VST_h_
#define _VST_VST_h_
#include <CtrlLib/CtrlLib.h>
#include "KeyMap.h"
using namespace Upp;

#define LAYOUTFILE <ValorantStatsTracker/VST.lay>
#include <CtrlCore/lay.h>

HHOOK _hook;
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

class VSTOption : public WithConfigurationLayout<TopWindow>{
public:
	uint8_t key_endRound;
	uint8_t key_kill;
	uint8_t headshot_kill;
	uint8_t game_started;
	uint8_t game_ended;
	typedef VSTOption CLASSNAME;
	
	VSTOption(VSTconfig& conf){
		CtrlLayoutOKCancel(*this, t_("Configuration"));
		key_endRound = conf.key_endRound;
		key_kill = conf.key_kill;
		headshot_kill = conf.headshot_kill;
		game_started = conf.game_started;
		game_ended = conf.game_ended;
		
		struct KeyCodeAndPtr{
			Ctrl* labelPtr = nullptr;
			uint8_t code;
			KeyCodeAndPtr(uint8_t c, Ctrl& ct){
				code = c;
				labelPtr = &ct;
			}
		};

		Upp::Array<KeyCodeAndPtr> KeyToFound;
		KeyToFound.Add(KeyCodeAndPtr(key_endRound,RoundEndedInput));
		KeyToFound.Add(KeyCodeAndPtr(key_kill,KillInput));
		KeyToFound.Add(KeyCodeAndPtr(headshot_kill,HSinput));
		KeyToFound.Add(KeyCodeAndPtr(game_started,GameStartedInput));
		KeyToFound.Add(KeyCodeAndPtr(game_ended,GameEndedInput));
		
		for(int e = 0; e < AllKeys.GetCount(); e++){
			if(KeyToFound.GetCount() == 0) break;
			const WinKey& key  = AllKeys[e];
			int cptFound = 0;
			bool found = false;
			for(KeyCodeAndPtr& k : KeyToFound){
				if(k.code == key.KeyCode){
					if(k.labelPtr) ((Label&)(*(k.labelPtr))) = key.KeyName;
					found=true;
					break;
				}
				cptFound++;
			}
			if(found){
				KeyToFound.Remove(cptFound);
			}
		}
		if(KeyToFound.GetCount()> 0){
			Exclamation("Error during resolution of HotKey, have you changed your config file by hand ? We advise you to delete it and relaunch the tool.");
		}
	}
	
	bool HookAKey(uint8_t* ValueToFill, Label& label){
		dword buff =0;
		KeyChange key(buff);
		key.Execute();
		if(AllKeys.Find(buff) > -1){
			const WinKey& key = AllKeys.Get(buff);
			*ValueToFill = (uint8_t)key.KeyCode;
			label = key.KeyName;
			return true;
		}else{
			label = "ERROR";
			return false;
		}
	}
};

class VST : public WithVSTLayout<TopWindow> {
	TrayIcon trayicon;
	VSTconfig cfg;
/*
	Thread and boolean used to drive valorant checker
*/
	Thread ValorantChecker;
	bool ValorantIsStarted = false;
	bool StateChanged = false;
	bool StopKeyHook = false; // when set to true, it stop hook no matter what happen
	void RoutineValorantThreadChecker();
/*
	config function
*/
	void LoadCfg();
/*
	keyboard listener data
*/
	static LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam);
	inline static VST* parent; //used to have pointer and process event
	void SetupHookKey();
	void ClearHookKey();
	void ProcessEvent(DWORD key);
	
/*
	Logic function
*/
	void Configure();
	void About();
	void Exit();
	void dummyFunc(){}
	void HideWin() { Hide(); }
	void Paint(Draw& w);
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
