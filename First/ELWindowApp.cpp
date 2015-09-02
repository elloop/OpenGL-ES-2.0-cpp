#include "ELWindowApp.h"
#include "GLES2/gl2.h"
#include <stdexcept>
USING_NS_ELLOOP;

int ELWindowApp::run() {
    _hwnd = CreateWindow(_T("ELWindow"), _T("ELWindow"), 
        WS_OVERLAPPEDWINDOW, 0, 0, _width, _height, 0, 
        0, _hInstance, this);
    if (0 == _hwnd) {
        return 0;
    }
    // update window
    UpdateWindow(_hwnd);

    ShowWindow(_hwnd, SW_NORMAL);

    if (!initOpenGLES()) {
        return false;
    }

    _shader = ShaderProgram_p2c4::create();
    if (_shader) {
        _shader->retain();
    }

    MSG msg = {0};

    while (msg.message != WM_QUIT) {
        if (WM_CLOSE == msg.message || WM_DESTROY == msg.message) {
            break;
        }
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {

            // clean autorelease objects.
            PoolManager::getInstance()->recycle();


            clock_t before = clock();
            double renderInterval = (before - _lastRender) / CLOCKS_PER_SEC;

            render();
            clock_t end = clock();
            double renderSeconds = (end - before) / CLOCKS_PER_SEC;

            _lastRender = before;

            eglSwapBuffers(_display, _surface);
        }
    }

    destroyOpenGLES();

    PoolManager::getInstance()->purge();

    return 0;
}

LRESULT CALLBACK ELWindowApp::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    ELWindowApp* app = (ELWindowApp*)GetWindowLong(hWnd, GWL_USERDATA);
    if (app != nullptr) {
        return app->onEvent(hWnd, msg, wParam, lParam);
    }
    if (WM_CREATE == msg) {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        SetWindowLong(hWnd, GWL_USERDATA, (DWORD_PTR)pCreate->lpCreateParams);
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK ELWindowApp::onEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
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


ELWindowApp::ELWindowApp(HINSTANCE instance):
        _hInstance(instance),
        _width(400),
        _height(600),
        _shader(nullptr)
{
    // 1. register a window class.
    WNDCLASSEX wndClass;
    memset(&wndClass, 0, sizeof(wndClass));
    wndClass.cbSize         = sizeof(wndClass);
    wndClass.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndClass.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndClass.hCursor        = LoadCursor(_hInstance, IDC_ARROW);
    wndClass.lpszClassName  = _T("ELWindow");
    wndClass.lpfnWndProc    = wndProc;
    wndClass.hIcon          = 0;
    wndClass.lpszMenuName   = 0;
    RegisterClassEx(&wndClass);
}

ELWindowApp::~ELWindowApp() {
    UnregisterClass(_T("ELWindow"), _hInstance);
    _shader->release();
}

bool ELWindowApp::initOpenGLES() {
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

    EGLint attr[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE};
    _context = eglCreateContext(_display, _config, 0, attr);

    if (eglMakeCurrent(_display, _surface, _surface, _context) == EGL_FALSE) {
        return false;
    }

    eglQuerySurface(_display, _surface, EGL_WIDTH, &_width);
    eglQuerySurface(_display, _surface, EGL_HEIGHT, &_height);
    return true;
}

void ELWindowApp::destroyOpenGLES() {
    if (_display != EGL_NO_DISPLAY) {
        eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (_context != EGL_NO_CONTEXT) {
            eglDestroyContext(_display, _context);
        }
        if (_surface != EGL_NO_SURFACE) {
            eglDestroySurface(_display, _surface);
        }
        eglTerminate(_display);
    }
    _display = EGL_NO_DISPLAY;
    _context = EGL_NO_CONTEXT;
    _surface = EGL_NO_SURFACE;
}

void ELWindowApp::render() {
    //glClearColor(1, 1, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, _width, _height);

    if (_shader) {
        _shader->begin();
        CELL::matrix4 screenProj = CELL::ortho<float>(0, float(_width), float(_height), 0, -100.0f, 100);
        GLfloat x = 100;
        GLfloat y = 100;
        GLfloat w = 100;
        GLfloat h = 100;
        GLfloat ary [] = {
            x, y, 
            x+w, y,
            x, y + h,
            x+w, y+h,
            //x + 3.0 * w/2, y + 1.0/2*h,
        };
        size_t  len = sizeof ary / sizeof ary[0];
        glUniformMatrix4fv(_shader->_mvp, 1, false, screenProj.data());
        glUniform4f(_shader->_color, 1, 0, 0, 0.5);
        glVertexAttribPointer(_shader->_position, 2, GL_FLOAT, false, 2*sizeof(GLfloat), ary);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        _shader->end();
    }

}


