#include "ELOpenGLView.h"
#include <tchar.h>

NS_BEGIN(elloop);

OpenGLView::OpenGLView() : 
    _height(600), 
    _width(400), 
    _hwnd(nullptr),
    _hInstance(nullptr),
    _config(nullptr),
    _surface(nullptr),
    _context(nullptr),
    _display(nullptr),
    _windowListener(nullptr)
{

}

bool OpenGLView::initWithInstance(HINSTANCE instance, LPCTSTR winName) {
    WNDCLASSEX wndClass;
    memset(&wndClass, 0, sizeof( wndClass ));
    wndClass.cbSize         = sizeof( wndClass );
    wndClass.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndClass.hbrBackground  = (HBRUSH) GetStockObject(BLACK_BRUSH);
    wndClass.hCursor        = LoadCursor(_hInstance, IDC_ARROW);
    wndClass.lpszClassName  = winName;
    wndClass.lpfnWndProc    = wndProc;
    wndClass.hIcon          = 0;
    wndClass.lpszMenuName   = 0;
    RegisterClassEx(&wndClass);

    _hwnd = CreateWindow(winName, winName,WS_OVERLAPPEDWINDOW, 0, 0, 
                         _width, _height, 0, 0, _hInstance, this);

    if ( !initOpenGLES() ) {
        return false;
    }

    return (_hwnd != 0);
}

bool OpenGLView::show() {
    
    UpdateWindow(_hwnd);

    ShowWindow(_hwnd, SW_NORMAL);

   /* if ( !initOpenGLES() ) {
        return false;
    }*/

    return true;
}

OpenGLView* OpenGLView::create(HINSTANCE instance, LPCTSTR winName) {
    auto ret = new (std::nothrow) OpenGLView();
    if (ret && ret->initWithInstance(instance, winName)) {
        ret->autorelease();
        return ret;
    }
    return nullptr;
}

LRESULT CALLBACK OpenGLView::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    auto glView = (OpenGLView*)GetWindowLong(hWnd, GWL_USERDATA);
    if (glView && glView->getWindowEventListener()) {
        return (glView->getWindowEventListener()->onEvent(hWnd, msg, wParam, lParam));
    }

    if ( WM_CREATE == msg ) {
        CREATESTRUCT* pCreate = (CREATESTRUCT*) lParam;
        SetWindowLong(hWnd, GWL_USERDATA, (DWORD_PTR) pCreate->lpCreateParams);
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

bool OpenGLView::initOpenGLES() {
    const EGLint attirbs[] =
    {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
        EGL_NONE
    };

    EGLint format(0);
    EGLint numConfigs(0);
    EGLint major;
    EGLint minor;

    _display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(_display, &major, &minor);

    eglChooseConfig(_display, attirbs, &_config, 1, &numConfigs);

    eglGetConfigAttrib(_display, _config, EGL_NATIVE_VISUAL_ID, &format);

    _surface = eglCreateWindowSurface(_display, _config, _hwnd, nullptr);

    EGLint attr[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };
    _context = eglCreateContext(_display, _config, 0, attr);

    if ( eglMakeCurrent(_display, _surface, _surface, _context) == EGL_FALSE ) {
        return false;
    }

    eglQuerySurface(_display, _surface, EGL_WIDTH, &_width);
    eglQuerySurface(_display, _surface, EGL_HEIGHT, &_height);
    return true;
}

void OpenGLView::destroyOpenGLES() {
    if ( _display != EGL_NO_DISPLAY ) {
        eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if ( _context != EGL_NO_CONTEXT ) {
            eglDestroyContext(_display, _context);
        }
        if ( _surface != EGL_NO_SURFACE ) {
            eglDestroySurface(_display, _surface);
        }
        eglTerminate(_display);
    }
    _display = EGL_NO_DISPLAY;
    _context = EGL_NO_CONTEXT;
    _surface = EGL_NO_SURFACE;
}

void OpenGLView::swapBuffer() {
    eglSwapBuffers(_display, _surface);
}

OpenGLView::~OpenGLView() {
    destroyOpenGLES();
}



NS_END(elloop);