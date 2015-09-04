#pragma once

#include "elloop/inc.h"
#include "elloop/Singleton.h"

NS_BEGIN(elloop);

class OpenGLView;

class Director : public Singleton<Director> {
public:
    void setGLView(OpenGLView* glView);
protected:
    Director();
    friend Singleton<Director>;
private:
    OpenGLView* _glView;
};

NS_END(elloop);
