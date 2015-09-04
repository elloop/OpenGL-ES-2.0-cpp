#include "ELWindowApp.h"
#include <Windows.h>
#include <tchar.h>

#include "Application.h"

int CALLBACK _tWinMain(
	HINSTANCE	hInstance, 
	HINSTANCE	hPrevInstance, 
	LPWSTR		lpCmdLine, 
	int			nShowCmd) 
{
    using elloop::Application;

    Application::getInstance()->init();
    int ret = Application::getInstance()->run();
    return ret;


    /*  ELWindowApp app(hInstance);
    app.setWidth(500);
    app.setHeight(500);
    app.run();*/


    //MessageBox(nullptr, TEXT("hello"), TEXT("hello2"), 0);
}