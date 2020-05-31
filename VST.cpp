#include "VST.h"

#define IMAGECLASS VSTImg
#define  IMAGEFILE <ValorantStatsTracker/VST.iml>
#include <Draw/iml.h>

#define TOPICFILE <ValorantStatsTracker/app.tpp/all.i>
#include <Core/topic_group.h>

BOOL GetMessageWithTimeout(MSG *msg, UINT to);

void VST::Configure()
{
	config.RoundEndedChange.WhenAction = ([&]{
		StopKeyListening =true; //Setting this to true ensure the key listening wont occure with KeyChange action
		dword test =0;
		KeyChange key(test);
		key.Execute();
		bool found = false;
		if(AllKeys.Find(test) > -1){
			Upp::Cout() << "Key selected for Round ended is : " <<  test << ", Is Name for Windows is : " <<  AllKeys.Get(test).KeyName <<  "\n";
		}
		StopKeyListening = false; //Reenabling hotkey
	});
	/*config.KillChange
	config.HSChange
	config.GSChange
	config.GEChange*/
	config.Execute();
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

void VST::SetupHotKey(){
	ClearHotKey();
	RegisterHotKey(0, END_ROUND, MOD_NOREPEAT, cfg.key_endRound);
	RegisterHotKey(0, KILL, MOD_NOREPEAT, cfg.key_kill);
	RegisterHotKey(0, HEADSHOT_KILL, MOD_NOREPEAT, cfg.headshot_kill);
	RegisterHotKey(0, GAME_STARTED, MOD_NOREPEAT, cfg.game_started);
	RegisterHotKey(0, GAME_ENDED, MOD_NOREPEAT, cfg.game_ended);
}

void VST::ClearHotKey(){
	UnregisterHotKey(0,END_ROUND);
	UnregisterHotKey(0,KILL);
	UnregisterHotKey(0,HEADSHOT_KILL);
	UnregisterHotKey(0,GAME_STARTED);
	UnregisterHotKey(0,GAME_ENDED);
}

void VST::ProcessEvent(MSG& msg){
	if(GetMessageWithTimeout(&msg, 100)){
		PeekMessage(&msg, 0, 0, 0, 0x0001);
		switch(msg.message){
			case WM_HOTKEY:
				switch(msg.wParam){
					case END_ROUND:
						printf("Round ended\n");
						break;
					case KILL:
						printf("Kill registered\n");
						break;
					case HEADSHOT_KILL:
						printf("headshot registered\n");
						break;
					case GAME_STARTED:
						printf("game started\n");
						break;
					case GAME_ENDED:
						printf("game ended\n");
						break;
				}
		}
	}
}

void VST::RoutineKeyboardListener(){
	MSG msg;
	printf("Keyboard listener thread have been started\n");
	int cptCheckValorant = 999;
	for(;;){
		if(Thread::IsShutdownThreads())break;
		if(cptCheckValorant > 100){
			//here we check for valorant and ensure valorant it is up
			if(::FindWindow(NULL, "valorant")) {
			//	ValorantIsStarted =true;
			}else{
			//	ValorantIsStarted = false;
			}
			cptCheckValorant=0;
			Cout() << "Checking for Valorant : " << ValorantIsStarted <<"\n";
		}
		if(ValorantIsStarted){
			if(!StopKeyListening){
				if(reloadConfig || KeyHaveBeenStop){
					printf("Loading HotKey\n");
					SetupHotKey();
					KeyHaveBeenStop = false;
					reloadConfig = false;
				}
				ProcessEvent(msg);
			}else{
				if(!KeyHaveBeenStop){
					printf("Removing Hotkey\n");
					ClearHotKey();
					KeyHaveBeenStop = true;
				}
				Sleep(100);
			}
		}else{
			if(!KeyHaveBeenStop){
				printf("Removing Hotkey\n");
				ClearHotKey();
				KeyHaveBeenStop = true;
			}
			Sleep(100);
		}
		cptCheckValorant++;
	}
	printf("Keyboard listener thread have ended\n");
}


void VST::Start()
{
	//First we load the file
	LoadCfg();
	//Then we setup hotkey
	keyboardListener.Run([&]{ RoutineKeyboardListener();});
}

void VST::Exit()
{
	Thread::ShutdownThreads();
	Break();
	trayicon.Break();
}

VST::VST()
{
	CtrlLayoutOKCancel(config, t_("Configuration"));
	CtrlLayout(*this, t_("Valorant Stats Tracker"));
	menu.Set(THISBACK(MainMenu));
	CenterScreen();
	Icon(VSTImg::icon());
	hide  <<= THISBACK(HideWin);
	ChangeValorant.WhenAction = ([&]{
		ValorantIsStarted = !ValorantIsStarted;
	});

	trayicon.WhenBar = THISBACK(TrayMenu);
	trayicon.WhenLeftDouble = THISBACK(dummyFunc);
	trayicon.Icon(VSTImg::trayIcon());
	trayicon.Tip(t_("Valorant Stats Tracker"));
	Start();
}

GUI_APP_MAIN
{
#ifdef PLATFORM_WIN32
	static const char unique_name[] = "VST";
	if(::FindWindow(NULL, unique_name)) {
		Exclamation(t_("VST is already running."));
		return;
	}
	TopWindow singlechk;
	singlechk.SetRect(-1, -1, 1, 1);
	singlechk.Hide();
	singlechk.Title(unique_name);
	singlechk.Open();
#endif

	VST().Run();
}

///GET message implementation but with a timeout
//https://stackoverflow.com/questions/10866311/getmessage-with-a-timeout 
//Thanks to rodrigo https://stackoverflow.com/users/865874/rodrigo
BOOL GetMessageWithTimeout(MSG *msg, UINT to)
{
    BOOL res;
    UINT_PTR timerId = SetTimer(NULL, NULL, to, NULL);
    res = GetMessage(msg,0,0,0);
    KillTimer(NULL, timerId);
    if (!res)
        return FALSE;
    if (msg->message == WM_TIMER && msg->hwnd == NULL && msg->wParam == timerId)
        return FALSE; //TIMEOUT! You could call SetLastError() or something...
    return TRUE;
}
