#include "app_control/ELAppDelegate.h"
#include <Windows.h>
#include <tchar.h>
#include <iostream>

int CALLBACK _tWinMain(
    HINSTANCE	hInstance,
    HINSTANCE	hPrevInstance,
    LPWSTR		lpCmdLine,
    int			nShowCmd)
{
    using elloop::AppDelegate;
    using elloop::Application;

    AppDelegate app;
    int ret = Application::getInstance()->run();
    return ret;
}