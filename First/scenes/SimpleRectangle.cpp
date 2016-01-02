#include "scenes/SimpleRectangle.h"
#include "app_control/ELDirector.h"
#include "math/ELGeometry.h"

NS_BEGIN(elloop);

void SimpleRectangle::render()
{
    using CELL::matrix4;
    using CELL::ortho;

    //glClearColor(1, 1, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto director = Director::getInstance();
    Size s = director->getFrameSize();
    float width =  s._width;
    float height = s._height;

    glViewport(0, 0, width, height);

    begin();

    matrix4 screenProj = ortho<float>(0, width, height, 0, -100.0f, 100);
    GLfloat x = 100;
    GLfloat y = 100;
    GLfloat w = 100;
    GLfloat h = 100;
    GLfloat ary[] = {
        x, y,
        x + w, y,
        x, y + h,
        x + w, y + h,
    };
    glUniformMatrix4fv(mvp_, 1, false, screenProj.data());
    glUniform4f(color_, 1, 0, 0, 0.5);
    glVertexAttribPointer(position_, 2, GL_FLOAT, false, 2 * sizeof(GLfloat), ary);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    end();
}

void SimpleRectangle::begin()
{
    glUseProgram(programId_);
    glEnableVertexAttribArray(position_);
}

void SimpleRectangle::end()
{
    glDisableVertexAttribArray(position_);
    glUseProgram(0);
}

bool SimpleRectangle::init()
{
    valid_ = ShaderProgram::initWithFile(vsFileName_, fsFileName_);
    if (valid_)
    {
        position_ = glGetAttribLocation(programId_, "_position");
        color_ = glGetUniformLocation(programId_, "_color");
        mvp_ = glGetUniformLocation(programId_, "_mvp");
    }
    return valid_;
}

SimpleRectangle* SimpleRectangle::create()
{
    auto * self = new SimpleRectangle();
    if (self && self->init())
    {
        self->autorelease();
        return self;
    }
    return nullptr;
}

NS_END(elloop);