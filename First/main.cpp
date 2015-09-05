#include "ELAppDelegate.h"
#include <Windows.h>
#include <tchar.h>


int CALLBACK _tWinMain(
	HINSTANCE	hInstance, 
	HINSTANCE	hPrevInstance, 
	LPWSTR		lpCmdLine, 
	int			nShowCmd) 
{
    using elloop::AppDelegate;

    AppDelegate app;
    int ret = elloop::Application::getInstance()->run();
    return ret;
}