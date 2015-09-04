#pragma once
#include <string>
#include "elloop/inc.h"
#include "windows.h"
#include "tchar.h"
#include "gltest.h"
#include "ELRef.h"
#include "math/ELGeometry.h"

NS_BEGIN(elloop);

class WindowEventListener {
public:
    virtual LRESULT CALLBACK onEvent(HWND hWnd, UINT msg,
                                     WPARAM wParam, LPARAM lParam) {
        return S_OK;
    }
};

class OpenGLView : public Ref {
public:
    static OpenGLView*      create(HINSTANCE instance, LPCTSTR winName=_T(""));
    bool                    show();
    
    bool                    initOpenGLES();
    void                    destroyOpenGLES();
    void                    swapBuffer();

    void setWindowEventListener(WindowEventListener* listener) {
        _windowListener = listener;
    }
    WindowEventListener* getWindowEventListener() const {
        return _windowListener;
    }

    float           width() const { return _width; }
    void            setWidth(float val) { _width = val; }
    float           height() const { return _height; }
    void            setHeight(float val) { _height = val; }
    Size            frameSize() {
        return Size(_width, _height);
    }

protected:
    OpenGLView();
    ~OpenGLView();
    bool initWithInstance(HINSTANCE instance, LPCTSTR winName);
    static LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, 
                                    WPARAM wParam, LPARAM lParam);
private:
    WindowCallback          _windowCallback;
    HWND                    _hwnd;
    HINSTANCE               _hInstance;
    int                     _width;
    int                     _height;
    EGLConfig               _config;
    EGLSurface              _surface;
    EGLContext              _context;
    EGLDisplay              _display;
    WindowEventListener*    _windowListener;
};

NS_END(elloop);
