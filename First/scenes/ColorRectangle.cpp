#include "scenes/ColorRectangle.h"
#include "app_control/ELDirector.h"
#include "math/ELGeometry.h"

NS_BEGIN(elloop);

void ColorRectangle::render()
{

    using namespace CELL;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto director = Director::getInstance();
    Size s = director->getFrameSize();
    float width = s._width;
    float height = s._height;

    glViewport(0, 0, width, height);

    begin();

    matrix4 screenProj = ortho<float>(0, width, height, 0, -100.0f, 100);
    GLfloat x = 100;
    GLfloat y = 100;
    GLfloat w = 100;
    GLfloat h = 100;
    float2 ary[] = {
        { x, y },
        { x + w, y },
        { x, y + h },
        { x + w, y + h },
    };


    glUniformMatrix4fv(_mvp, 1, false, screenProj.data());
    glUniform4f(_color, 1, 0, 0, 0.5);
    glVertexAttribPointer(_position, 2, GL_FLOAT, false, 2 * sizeof( GLfloat ), ary);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    end();
}

void ColorRectangle::begin()
{
    glUseProgram(programId_);
    glEnableVertexAttribArray(_position);
}

void ColorRectangle::end()
{
    glDisableVertexAttribArray(_position);
    glUseProgram(0);
}

bool ColorRectangle::init()
{
    valid_ = ShaderProgram::initWithFile(vsFileName_, fsFileName_);
    if ( valid_ )
    {
        _position = glGetAttribLocation(programId_, "_position");
        _color = glGetUniformLocation(programId_, "_color");
        _mvp = glGetUniformLocation(programId_, "_mvp");
    }
    return valid_;
}

ColorRectangle* ColorRectangle::create()
{
    auto * self = new ColorRectangle();
    if ( self && self->init() )
    {
        self->autorelease();
        return self;
    }
    return nullptr;
}

NS_END(elloop);