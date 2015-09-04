#pragma once

#include "elloop/inc.h"
#include "elloop/Singleton.h"
#include "math/ELGeometry.h"

NS_BEGIN(elloop);

class OpenGLView;

class Director : public Singleton<Director> {
public:
    void            setGLView(OpenGLView* glView);
    OpenGLView*     getGLView() const { return _glView;}
    void            mainLoop();
    void            drawScenes();
    void            end();
    Size            getFrameSize() const;
protected:
    Director();
    ~Director();
    friend Singleton<Director>;
private:
    OpenGLView*     _glView;
};

NS_END(elloop);
