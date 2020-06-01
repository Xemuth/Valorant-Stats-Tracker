#include "VST.h"

#define IMAGECLASS VSTImg
#define  IMAGEFILE <ValorantStatsTracker/VST.iml>
#include <Draw/iml.h>

#define TOPICFILE <ValorantStatsTracker/app.tpp/all.i>
#include <Core/topic_group.h>

void VST::Configure()
{
	StopKeyHook = true;
	VSTOption config(cfg);
	
	config.RoundEndedChange.WhenAction = ([&]{
		config.HookAKey(&config.key_endRound, config.RoundEndedInput);
	});
	config.KillChange.WhenAction = ([&]{
		config.HookAKey(&config.key_kill, config.KillInput);
	});
	config.HSChange.WhenAction = ([&]{
		config.HookAKey(&config.headshot_kill, config.HSinput);
	});
	config.GSChange.WhenAction = ([&]{
		config.HookAKey(&config.game_started, config.GameStartedInput);
	});
	config.GEChange.WhenAction = ([&]{
		config.HookAKey(&config.game_ended, config.GameEndedInput);
	});
	int exitMode = config.Execute();
	if(exitMode == 1 ){
		//Then we save
		cfg.key_kill = config.key_kill;
		cfg.headshot_kill = config.headshot_kill;
		cfg.key_endRound = config.key_endRound;
		cfg.game_started = config.game_started;
		cfg.game_ended = config.game_ended;
		String cfgfile = ConfigFile();
		if(!StoreToFile(cfg, cfgfile))
			Exclamation("Error saving configuration file!");
	}
	StopKeyHook = false;
}

void VST::About()
{
	WithAboutDialogLayout<TopWindow> dlg;
	dlg.text = GetTopic("topic://VST/app/about$en-us").text;
	CtrlLayoutOK(dlg, "About");
	dlg.CenterScreen();
	dlg.Run();
}

void VST::SettingsMenu(Bar& bar)
{
	bar.Add(t_("Configuration"), THISBACK(Configure)).Help(t_("Change settings"));
	bar.Add(t_("Exit"), THISBACK(Exit)).Help(t_("Exit the application"));
}

void VST::HelpMenu(Bar& bar)
{
	bar.Add(t_("About"), THISBACK(About)) .Help(t_("About VST"));
}

void VST::MainMenu(Bar& bar)
{
	bar.Add(t_("Settings"), THISBACK(SettingsMenu));
	bar.Add(t_("Help"), THISBACK(HelpMenu));
}

void VST::TrayMenu(Bar& bar)
{
	bar.Add(t_("Show VST"), THISBACK(dummyFunc));
	bar.Add(t_("Ignore eyes"), THISBACK(dummyFunc)).Check(true);
	bar.Add(t_("About.."), THISBACK(About));
	bar.Add(t_("Exit"), THISBACK(Exit));
}

void VST::LoadCfg(){
	String cfgfile = ConfigFile();
    if(FileExists(cfgfile) && !LoadFromFile(cfg, cfgfile))
        Exclamation("Error loading configuration file!");
    if(!StoreToFile(cfg, cfgfile))
        Exclamation("Error updating configuration file!");
}

void VST::SetupHookKey(){
	printf("Enabling Key hooking\n");
	if (!(_hook = SetWindowsHookEx(WH_KEYBOARD_LL, VST::HookCallback, NULL, 0))){
		MessageBox(NULL, "Failed to install keyboard hook ! VST will now close.", "Error", MB_ICONERROR);
		Exit();
	}
}

void VST::ClearHookKey(){
	printf("Disabling Key hooking\n");
	UnhookWindowsHookEx(_hook);
}

LRESULT __stdcall VST::HookCallback(int nCode, WPARAM wParam, LPARAM lParam){
	KBDLLHOOKSTRUCT kbdStruct;
	if (nCode >= 0){
		// the action is valid: HC_ACTION.
		if (wParam == WM_KEYDOWN){
			// lParam is the pointer to the struct containing the data needed, so cast and assign it to kdbStruct.
			kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
			// a key (non-system) is pressed.
			if(parent)parent->ProcessEvent(kbdStruct.vkCode);
		}
	}
	// call the next hook in the hook chain. This is nessecary or your hook chain will break and the hook stops
	return CallNextHookEx(_hook, nCode, wParam, lParam);
}

void VST::ProcessEvent(DWORD key){
	if(key == cfg.key_kill){
		printf("Kill occured\n");
	}else if(key == cfg.headshot_kill){
		printf("Headshot kill occured\n");
	}else if(key == cfg.key_endRound){
		printf("EndRound occured\n");
	}else if(key == cfg.game_started){
		printf("Game started occured\n");
	}else if(key == cfg.game_ended){
		printf("Game ended occured\n");
	}
}

void VST::Paint(Draw& w){
	TopWindow::Paint(w);
	if(!StopKeyHook){
		if(!ValorantIsStarted){
			if(StateChanged){
				ClearHookKey();
				StateChanged = false;
			}
		}else{
			if(StateChanged){
				SetupHookKey();
				StateChanged = false;
			}
		}
	}else{
		if(!StateChanged){
			ClearHookKey();
			StateChanged = true;
		}
	}
}

void VST::RoutineValorantThreadChecker(){
	printf("Valorant thread Checker start\n");
	bool lastValue = false;
	for(;;){
		if(Thread::IsShutdownThreads())break;
		lastValue = ValorantIsStarted;
		if(::FindWindow(NULL, "valorant")) {
		//	ValorantIsStarted =true;
		}else{
		//	ValorantIsStarted = false;
		}
		if(lastValue != ValorantIsStarted)StateChanged = true;
		Sleep(100);
	}
	printf("Valorant thread Checker have ended\n");
}

void VST::Exit()
{
	Thread::ShutdownThreads();
	ClearHookKey();
	Break();
	trayicon.Break();
}

VST::VST()
{
	CtrlLayout(*this, t_("Valorant Stats Tracker"));
	menu.Set(THISBACK(MainMenu));
	CenterScreen();
	Icon(VSTImg::icon());
	hide  <<= THISBACK(HideWin);
	ChangeValorant.WhenAction = ([&]{
		ValorantIsStarted = !ValorantIsStarted;
		StateChanged = true;
	});

	trayicon.WhenBar = THISBACK(TrayMenu);
	trayicon.WhenLeftDouble = THISBACK(dummyFunc);
	trayicon.Icon(VSTImg::trayIcon());
	trayicon.Tip(t_("Valorant Stats Tracker"));
	
	parent = this;
	
	//First we load the file
	LoadCfg();
	//Then we setup hotkey
	ValorantChecker.Run([&]{ RoutineValorantThreadChecker();});
}

GUI_APP_MAIN
{
#ifdef PLATFORM_WIN32
	static const char unique_name[] = "Valorant Stats Tracker";
	if(::FindWindow(NULL, unique_name)) {Exclamation(t_("VST is already running."));return;	}
	TopWindow singlechk;
	singlechk.SetRect(-1, -1, 1, 1);
	singlechk.Hide();
	singlechk.Title(unique_name);
	singlechk.Open();
#endif
	VST().Run();
}