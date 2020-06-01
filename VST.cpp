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
		bool ReloadBDD = false;
		bool CheckAPI =false;
		
		cfg.key_kill = config.key_kill;
		cfg.headshot_kill = config.headshot_kill;
		cfg.key_endRound = config.key_endRound;
		cfg.game_started = config.game_started;
		cfg.game_ended = config.game_ended;
		
		if(cfg.DB_Location.Compare(config.DB_Location) !=0) ReloadBDD=true;
		cfg.DB_Location = config.DB_Location;
		
		if(cfg.API_KEY.Compare(config.API_KEY) !=0) CheckAPI=true;
		cfg.API_KEY = config.API_KEY;
		
		cfg.LocalSave = config.LocalSave;
		cfg.SendDataOnline = config.SendDataOnline;
		cfg.typeSend = config.typeSend;
		
		String cfgfile = ConfigFile();
		if(!StoreToFile(cfg, cfgfile))
			Exclamation("Error saving configuration file!");
	}
	StopKeyHook = false;
}

void VST::About()
{
	WithAboutDialogLayout<TopWindow> dlg;
	dlg.text = GetTopic("topic://ValorantStatsTracker/app/about$en-us").text;
	dlg.text2 = GetTopic("topic://ValorantStatsTracker/app/about2$en-us").text;
	dlg.image.SetImage( RescaleFilter(VSTImg::icon(), 70, 70, 0));
	dlg.image2.SetImage( RescaleFilter(VSTImg::Upp(), 70, 70, 0));
	CtrlLayoutOK(dlg, "About");
	dlg.CenterScreen();
	dlg.Run();
}

void VST::MainMenu(Bar& bar)
{
	bar.Add(t_("Configuration"), THISBACK(Configure)).Help(t_("Change settings"));
	bar.Add(t_("About"), THISBACK(About)) .Help(t_("About VST"));
	bar.Add(t_("Exit"), THISBACK(Exit)).Help(t_("Exit the application"));
}
void VST::TrayMenu(Bar& bar)
{
	bar.Add(t_("Configuration"), THISBACK(Configure)).Help(t_("Change settings"));
	bar.Add(t_("About"), THISBACK(About)) .Help(t_("About VST"));
	bar.Add(t_("Exit"), THISBACK(Exit)).Help(t_("Exit the application"));
}

void VST::LoadCfg(){
	String cfgfile = ConfigFile();
    if(FileExists(cfgfile) && !LoadFromFile(cfg, cfgfile)){
        Exclamation("Error loading configuration file!");
		ToolStatus = "Configuration loading failled";
    }else{
		ToolStatus = "Default configuration loaded";
    }
    /*if(!StoreToFile(cfg, cfgfile))
        Exclamation("Error updating configuration file!");*/
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
				ValorantFinder = "Valorant is not found";
				KeyHookStatus = "KeyHook is disabled";
				StateChanged = false;
			}
		}else{
			if(StateChanged){
				SetupHookKey();
				ValorantFinder = "Valorant is launch";
				KeyHookStatus = "KeyHook is enabled";
				StateChanged = false;
			}
		}
	}else{
		if(!StateChanged){
			ClearHookKey();
			KeyHookStatus = "KeyHook is disabled during configuration";
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
		if(CheckForProcess(targetProcessName)) {
			ValorantIsStarted =true;
		}else{
			ValorantIsStarted = false;
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

void VST::PrepareArray(){
	array.AddColumn("No");
	array.AddColumn("Date");
	array.AddColumn("Kill");
	array.AddColumn("Headshot");
	array.AddColumn("Total Round");
	
	//array.WhenLeftDouble = [=] { DoubleClick(); };
	array.ColumnWidths("30 90 60 60 70");

	int k =0, hs =0, round =0, games =0;
	int k_ =0, hs_ =0, round_ =0;

	for(int i = 0; i < 20; i++){
		k_ = rand() % 30;
		hs_ = rand() % k_;
		round_ = 13 +( rand() % 12);
		games++;
		k+= k_;
		hs+= hs_;
		round+= round_;
		array.Add(i, GetSysDate() ,k_ , hs_,round_);
	}
	
	TKill = AsString(k);
	Kill = AsString(k_);
	THS = AsString(hs);
	HS = AsString(hs_);
	TRP = AsString(round);
	RP = AsString(round_);
	TGP = AsString(games);
	
	
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
	PrepareArray();
	
	DataPersistenceStatus = (BddLoaded)? "Database is loaded":"No database loaded";
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