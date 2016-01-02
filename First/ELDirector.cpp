#include "ELDirector.h"
#include "ELOpenGLView.h"
#include "ELDrawable.h"

NS_BEGIN(elloop);


Director::Director() 
: _glView(nullptr)
, _currentScene(nullptr)
{}

void Director::setGLView(OpenGLView* glView)
{
    assert(glView);
    glView->retain();
    if ( _glView )
    {
        _glView->release();
    }
    _glView = glView;
}

void Director::mainLoop()
{

    drawScenes();

    if ( _glView )
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
    if ( _glView )
    {
        _glView->release();
        _glView = nullptr;
    }

    clearSceneStack();
    _currentScene = nullptr;
}

void Director::drawScenes()
{
    if ( _currentScene )
    {
        _currentScene->render();
    }
}

Size Director::getFrameSize() const
{
    return ( _glView == nullptr ) ? Size() : _glView->frameSize();
}

void Director::runWithScene(Drawable* drawable)
{
    assert(drawable);
    drawable->retain();
    _sceneStack.push(drawable);
    if ( _currentScene )
    {
        _currentScene->release();
    }
    _currentScene = drawable;
}

void Director::clearSceneStack()
{
    Drawable* temp(nullptr);
    while ( !_sceneStack.empty() )
    {
        temp = _sceneStack.top();
        temp->release();
        _sceneStack.pop();
    }
}

NS_END(elloop);