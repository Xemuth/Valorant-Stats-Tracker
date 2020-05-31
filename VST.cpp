#include "VST.h"

#define IMAGECLASS VSTImg
#define  IMAGEFILE <ValorantStatsTracker/VST.iml>
#include <Draw/iml.h>

#define TOPICFILE <ValorantStatsTracker/app.tpp/all.i>
#include <Core/topic_group.h>

BOOL GetMessageWithTimeout(MSG *msg, UINT to);

void VST::Configure()
{
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
	UnregisterHotKey(0,END_ROUND);
	UnregisterHotKey(0,KILL);
	UnregisterHotKey(0,HEADSHOT_KILL);
	RegisterHotKey(0, END_ROUND, MOD_NOREPEAT, cfg.key_endRound);
	RegisterHotKey(0, KILL, MOD_NOREPEAT, cfg.key_kill);
	RegisterHotKey(0, HEADSHOT_KILL, MOD_NOREPEAT, cfg.headshot_kill);
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
				}
		}
	}
}

void VST::RoutineKeyboardListener(){
	MSG msg;
	SetupHotKey();
	printf("Keyboard listener thread have been started\n");
	for(;;){
		if(Thread::IsShutdownThreads())break;
		if(reloadConfig){
			SetupHotKey();
			reloadConfig = false;
		}
		ProcessEvent(msg);
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