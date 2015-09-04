#include "Director.h"

NS_BEGIN(elloop);


Director::Director() : _glView(nullptr) {

}

void Director::setGLView(OpenGLView* glView) {
    _glView = glView;
}

NS_END(elloop);