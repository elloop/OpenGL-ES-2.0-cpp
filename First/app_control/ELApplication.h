#include <windows.h>
#include "elloop/inc.h"
#include "ELOpenGLView.h"
#include "ELApplicationProtocol.h"

NS_BEGIN(elloop);

class Director;

class Application : 
    public ApplicationProtocol,
    public WindowEventListener

{
public:
    static Application* getInstance();
    bool                init();
    int                 run();
    LRESULT CALLBACK    onEvent(HWND hWnd, UINT msg, 
                                WPARAM wParam, LPARAM lParam) override;
protected:
    Application();
    static Application* instance_;
    HINSTANCE           hInstance_;
};

NS_END(elloop);

