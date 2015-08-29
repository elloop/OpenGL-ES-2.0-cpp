#include "ELWindowApp.h"
#include <Windows.h>
#include <tchar.h>

int CALLBACK _tWinMain(
	HINSTANCE	hInstance, 
	HINSTANCE	hPrevInstance, 
	LPWSTR		lpCmdLine, 
	int			nShowCmd) 
{
    ELWindowApp app(hInstance);
    app.setWidth(500);
    app.setHeight(500);
    app.run();
	return 0;
}