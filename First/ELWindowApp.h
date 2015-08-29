#include <Windows.h>
#include <tchar.h>
#include "EGL/egl.h"
#include "gltest.h"

class ELWindowApp
{

public:
	ELWindowApp(HINSTANCE instance);
	~ELWindowApp();

    static LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);
    int run();

    float width() const { return _width; }
    void setWidth(float val) { _width = val; }
    float height() const { return _height; }
    void setHeight(float val) { _height = val; }
    LRESULT CALLBACK onEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    // init es 2.0
    bool initOpenGLES();
    void destroyOpenGLES();
    void render();
private:
	HWND        _hwnd;
    HINSTANCE   _hInstance;
    int         _width;
    int         _height;

    EGLConfig   _config;
    EGLSurface  _surface;
    EGLContext  _context;
    EGLDisplay  _display;

    elloop::ShaderProgram* _shader;
};
