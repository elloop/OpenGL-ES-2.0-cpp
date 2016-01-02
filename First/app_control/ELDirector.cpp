#include "ELDirector.h"
#include "ELOpenGLView.h"
#include "scenes/ELDrawable.h"

NS_BEGIN(elloop);


Director::Director()
: glView_(nullptr)
, currentScene_(nullptr)
{}

void Director::setGLView(OpenGLView* glView)
{
    assert(glView);
    glView->retain();
    if (glView_)
    {
        glView_->release();
    }
    glView_ = glView;
}

void Director::mainLoop()
{

    drawScenes();

    if (glView_)
    {
        glView_->swapBuffer();
    }
}

void Director::end()
{
    delete _instance;
}

Director::~Director()
{
    if (glView_)
    {
        glView_->release();
        glView_ = nullptr;
    }

    clearSceneStack();
    currentScene_ = nullptr;
}

void Director::drawScenes()
{
    if (currentScene_)
    {
        currentScene_->render();
    }
}

Size Director::getFrameSize() const
{
    return (glView_ == nullptr) ? Size() : glView_->frameSize();
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
    if (!sceneStack_.empty() && drawable == sceneStack_.top()) 
    {
        return;
    }

    drawable->retain();
    sceneStack_.push(drawable);
    currentScene_ = drawable;
}

void Director::clearSceneStack()
{
    Drawable* temp(nullptr);
    while (!sceneStack_.empty())
    {
        temp = sceneStack_.top();
        temp->release();
        sceneStack_.pop();
    }
}



NS_END(elloop);