#include "ELDirector.h"
#include "ELOpenGLView.h"
#include "scenes/ELDrawable.h"

NS_BEGIN(elloop);


Director::Director()
: _glView(nullptr)
, _currentScene(nullptr)
{}

void Director::setGLView(OpenGLView* glView)
{
    assert(glView);
    glView->retain();
    if (_glView)
    {
        _glView->release();
    }
    _glView = glView;
}

void Director::mainLoop()
{

    drawScenes();

    if (_glView)
    {
        _glView->swapBuffer();
    }
}

void Director::end()
{
    delete _instance;
}

Director::~Director()
{
    if (_glView)
    {
        _glView->release();
        _glView = nullptr;
    }

    clearSceneStack();
    _currentScene = nullptr;
}

void Director::drawScenes()
{
    if (_currentScene)
    {
        _currentScene->render();
    }
}

Size Director::getFrameSize() const
{
    return (_glView == nullptr) ? Size() : _glView->frameSize();
}

void Director::runWithScene(Drawable* drawable)
{
    /*assert(drawable);
    drawable->retain();
    sceneStack_.push(drawable);
    currentScene_ = drawable;*/
}

void Director::pushScene(Drawable* drawable)
{
    assert(drawable);
    // judge if drawable is the current top?
    if (!_sceneStack.empty() && drawable == _sceneStack.top()) 
    {
        return;
    }

    drawable->retain();
    _sceneStack.push(drawable);
    _currentScene = drawable;
}

void Director::clearSceneStack()
{
    Drawable* temp(nullptr);
    while (!_sceneStack.empty())
    {
        temp = _sceneStack.top();
        temp->release();
        _sceneStack.pop();
    }
}



NS_END(elloop);