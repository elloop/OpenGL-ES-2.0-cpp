#include "ELDirector.h"
#include "ELOpenGLView.h"

NS_BEGIN(elloop);


Director::Director() : _glView(nullptr) {

}

void Director::setGLView(OpenGLView* glView) {
    assert(glView);
    glView->retain();
    if (_glView) {
        _glView->release();
    }
    _glView = glView;
}

void Director::mainLoop() {

    drawScenes();

    if (_glView) {
        _glView->swapBuffer();
    }
}

void Director::end() {
    delete _instance;
}

Director::~Director() {
    if (_glView) {
        _glView->release();
    }
}

void Director::drawScenes() {
    //glClearColor(1, 1, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, _width, _height);

    if ( _shader ) {
        _shader->begin();
        CELL::matrix4 screenProj = CELL::ortho<float>(0, float(_width), float(_height), 0, -100.0f, 100);
        GLfloat x = 100;
        GLfloat y = 100;
        GLfloat w = 100;
        GLfloat h = 100;
        GLfloat ary[] = {
            x, y,
            x + w, y,
            x, y + h,
            x + w, y + h,
            //x + 3.0 * w/2, y + 1.0/2*h,
        };
        size_t  len = sizeof ary / sizeof ary[0];
        glUniformMatrix4fv(_shader->_mvp, 1, false, screenProj.data());
        glUniform4f(_shader->_color, 1, 0, 0, 0.5);
        glVertexAttribPointer(_shader->_position, 2, GL_FLOAT, false, 2 * sizeof( GLfloat ), ary);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        _shader->end();
    }
}

Size Director::getFrameSize() {
    return ( _glView == nullptr ) ? Size() : _glView->frameSize();
}

NS_END(elloop);