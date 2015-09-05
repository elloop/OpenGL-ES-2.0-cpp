#pragma once

#include "elloop/inc.h"
#include "elloop/Singleton.h"
#include "math/ELGeometry.h"
#include <stack>

NS_BEGIN(elloop);

class OpenGLView;
class Drawable;

class Director : public Singleton<Director> {
public:
    void            setGLView(OpenGLView* glView);
    OpenGLView*     getGLView() const { return _glView;}
    void            mainLoop();
    void            end();
    
    void            drawScenes();
    void            runWithScene(Drawable* drawable);

    Size            getFrameSize() const;
protected:
    Director();
    ~Director();
    void            clearSceneStack();
    friend Singleton<Director>;
private:
    OpenGLView*             _glView;
    Drawable*               _currentScene;
    std::stack<Drawable*>   _sceneStack;
};

NS_END(elloop);
