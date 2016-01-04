#include "ELOpenGLView.h"
#include <tchar.h>

NS_BEGIN(elloop);

OpenGLView::OpenGLView()
    : width_(800)
    , height_(600)
    , hwnd_(nullptr)
    , hInstance_(nullptr)
    , config_(nullptr)
    , surface_(nullptr)
    , context_(nullptr)
    , display_(nullptr)
    , windowListener_(nullptr)
{}

bool OpenGLView::initWithInstance(HINSTANCE instance, LPCTSTR winName)
{
    WNDCLASSEX wndClass;
    memset(&wndClass, 0, sizeof(wndClass));
    wndClass.cbSize = sizeof(wndClass);
    wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndClass.hCursor = LoadCursor(hInstance_, IDC_ARROW);
    wndClass.lpszClassName = winName;
    wndClass.lpfnWndProc = wndProc;
    wndClass.hIcon = 0;
    wndClass.lpszMenuName = 0;
    RegisterClassEx(&wndClass);

    hwnd_ = CreateWindow(winName, winName, WS_OVERLAPPEDWINDOW, 0, 0,
        width_, height_, 0, 0, hInstance_, this);

    if (!initOpenGLES())
    {
        return false;
    }

    return (hwnd_ != 0);
}

bool OpenGLView::show()
{

    UpdateWindow(hwnd_);

    ShowWindow(hwnd_, SW_NORMAL);

    /* if ( !initOpenGLES() ) {
         return false;
         }*/

    return true;
}

OpenGLView* OpenGLView::create(HINSTANCE instance, LPCTSTR winName)
{
    auto ret = new (std::nothrow) OpenGLView();
    if (ret && ret->initWithInstance(instance, winName))
    {
        ret->autorelease();
        return ret;
    }
    return nullptr;
}

LRESULT CALLBACK OpenGLView::wndProc(HWND hWnd,
    UINT msg, WPARAM wParam, LPARAM lParam)
{
    auto glView = (OpenGLView*)GetWindowLong(hWnd, GWL_USERDATA);
    if (glView && glView->getWindowEventListener())
    {
        return (glView->getWindowEventListener()->onEvent(
            hWnd, msg, wParam, lParam));
    }

    if (WM_CREATE == msg)
    {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        SetWindowLong(hWnd, GWL_USERDATA, (DWORD_PTR)pCreate->lpCreateParams);
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

bool OpenGLView::initOpenGLES()
{
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

    display_ = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display_, &major, &minor);

    eglChooseConfig(display_, attirbs, &config_, 1, &numConfigs);

    eglGetConfigAttrib(display_, config_, EGL_NATIVE_VISUAL_ID, &format);

    surface_ = eglCreateWindowSurface(display_, config_, hwnd_, nullptr);

    EGLint attr[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };
    context_ = eglCreateContext(display_, config_, 0, attr);

    if (eglMakeCurrent(display_, surface_, surface_, context_) == EGL_FALSE)
    {
        return false;
    }

    eglQuerySurface(display_, surface_, EGL_WIDTH, &width_);
    eglQuerySurface(display_, surface_, EGL_HEIGHT, &height_);
    return true;
}

void OpenGLView::destroyOpenGLES()
{
    if (display_ != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(display_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (context_ != EGL_NO_CONTEXT)
        {
            eglDestroyContext(display_, context_);
        }
        if (surface_ != EGL_NO_SURFACE)
        {
            eglDestroySurface(display_, surface_);
        }
        eglTerminate(display_);
    }
    display_ = EGL_NO_DISPLAY;
    context_ = EGL_NO_CONTEXT;
    surface_ = EGL_NO_SURFACE;
}

void OpenGLView::swapBuffer()
{
    eglSwapBuffers(display_, surface_);
}

OpenGLView::~OpenGLView()
{
    destroyOpenGLES();
}



NS_END(elloop);