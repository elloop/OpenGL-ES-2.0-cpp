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
    OpenGLView*     getGLView() const { return glView_;}
    void            mainLoop();
    void            end();
    
    void            drawScenes();
    void            pushScene(Drawable* drawable);
    void            runWithScene(Drawable* drawable);

    Size            getFrameSize() const;
protected:
    Director();
    ~Director();
    void            clearSceneStack();
    friend Singleton<Director>;
private:
    OpenGLView*             glView_;
    Drawable*               currentScene_;
    std::stack<Drawable*>   sceneStack_;
};

NS_END(elloop);
