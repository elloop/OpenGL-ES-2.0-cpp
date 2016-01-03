#include "scenes/RotateRectangle.h"
#include "app_control/ELDirector.h"
#include "math/ELGeometry.h"

NS_BEGIN(elloop);

using namespace CELL;

struct Vertex
{
    float2      pos;
    Rgba4Byte   color;
};

void RotateRectangle::render()
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

    Vertex ary[] = 
    {
        float2{ x, y },         Rgba4Byte(255, 0, 0, 1),    
        float2{ x, y + h },     Rgba4Byte(0, 255, 0, 1),    
        float2{ x + w, y },     Rgba4Byte(0, 0, 255, 1),    
        float2{ x + w, y + h }, Rgba4Byte(255, 255, 255, 1),
    };

    matrix4 matTrans;
    matTrans.translate(-150, -150, 0);

    static float delta = 0;
    matrix4 matRotate;
    matRotate.rotateZ(delta);
    delta += 0.5;

    matrix4 matTransback;
    matTransback.translate(150, 150, 0);

    matrix4 matROrigin = matTransback * (matRotate * matTrans);

    screenProj = screenProj * matROrigin;

    glUniformMatrix4fv(mvp_, 1, false, screenProj.data());
    glVertexAttribPointer(position_, 2, GL_FLOAT, false, sizeof( Vertex ), ary);
    glVertexAttribPointer(color_, 4, GL_UNSIGNED_BYTE, true, sizeof( Vertex ), &( ary[0].color ));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    end();
}

void RotateRectangle::begin()
{
    glUseProgram(programId_);
    glEnableVertexAttribArray(position_);
    glEnableVertexAttribArray(color_);
}

void RotateRectangle::end()
{
    glDisableVertexAttribArray(color_);
    glDisableVertexAttribArray(position_);
    glUseProgram(0);
}

bool RotateRectangle::init()
{
    valid_ = ShaderProgram::initWithFile(vsFileName_, fsFileName_);
    if ( valid_ )
    {
        position_ = glGetAttribLocation(programId_, "_position");
        color_ = glGetAttribLocation(programId_, "_color");
        mvp_ = glGetUniformLocation(programId_, "_mvp");
    }
    return valid_;
}

RotateRectangle* RotateRectangle::create()
{
    auto * self = new RotateRectangle();
    if ( self && self->init() )
    {
        self->autorelease();
        return self;
    }
    return nullptr;
}

NS_END(elloop);