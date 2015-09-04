#include "ELApplication.h"
#include <ctime>
#include "ELAutoReleasePool.h"
#include "ELDirector.h"
#include "ELOpenGLView.h"
NS_BEGIN(elloop);

Application* Application::_instance = nullptr;

bool Application::init() 
{
    auto director = Director::getInstance();
    auto glView = director->getGLView();
    if (!glView) {
        auto view = OpenGLView::create(_hInstance, _T("OpenGL ES 2.0"));
        assert(view);
        view->setWindowEventListener(this);
        director->setGLView(view);
    }
    return true;
}

int Application::run() 
{

    if (!applicationDidFinishLaunching()) {
        return -1;
    }

    auto director = Director::getInstance();
    if (!director->getGLView()) {
        return -1;
    }

    director->getGLView()->show();

    MSG msg = { 0 };

    while ( msg.message != WM_QUIT ) {
        if ( WM_CLOSE == msg.message || WM_DESTROY == msg.message ) {
            break;
        }
        if ( PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) ) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {

            // clean autorelease objects.
            PoolManager::getInstance()->recycle();

            director->mainLoop();
        }
    }

    director->end();
    PoolManager::getInstance()->purge();

    return 0;
}

Application::Application():
    _hInstance(nullptr)
{
    _hInstance = GetModuleHandle(nullptr);
    init();
    _instance = this;
}

LRESULT Application::onEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch ( msg ) {
    case WM_CREATE:
        MessageBox(nullptr, _T("hello"), _T("title"), 0);
        break;
    case WM_CLOSE:
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
        break;
    }
    return S_OK;
}

Application* Application::getInstance() {
    return _instance;
}

NS_END(elloop);

