#pragma once
#include <string>
#include "elloop/inc.h"
#include "windows.h"
#include "tchar.h"
#include "gl_include.h"
#include "basic/ELRef.h"
#include "math/ELGeometry.h"

NS_BEGIN(elloop);

class WindowEventListener
{
public:
    virtual LRESULT CALLBACK onEvent(HWND hWnd, UINT msg,
        WPARAM wParam, LPARAM lParam)
    {
        return S_OK;
    }
};

class OpenGLView : public Ref
{
public:
    static OpenGLView*      create(HINSTANCE instance, LPCTSTR winName = _T(""));
    bool                    show();

    bool                    initOpenGLES();
    void                    destroyOpenGLES();
    void                    swapBuffer();

    void setWindowEventListener(WindowEventListener* listener)
    {
        windowListener_ = listener;
    }
    WindowEventListener* getWindowEventListener() const
    {
        return windowListener_;
    }

    float   width() const { return width_; }
    void    setWidth(float val) { width_ = val; }
    float   height() const { return height_; }
    void    setHeight(float val) { height_ = val; }
    Size    frameSize() const { return Size(width_, height_); }

protected:
    OpenGLView();
    ~OpenGLView();
    bool initWithInstance(HINSTANCE instance, LPCTSTR winName);
    static LRESULT CALLBACK wndProc(HWND hWnd, UINT msg,
        WPARAM wParam, LPARAM lParam);
private:
    WindowCallback          windowCallback_;
    HWND                    hwnd_;
    HINSTANCE               hInstance_;
    int                     width_;
    int                     height_;
    EGLConfig               config_;
    EGLSurface              surface_;
    EGLContext              context_;
    EGLDisplay              display_;
    WindowEventListener*    windowListener_;
};

NS_END(elloop);
