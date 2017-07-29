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
        float2{ x, y },         Rgba4Byte(255, 0, 0, 255),
        float2{ x, y + h },     Rgba4Byte(0, 255, 0, 255),
        float2{ x + w, y },     Rgba4Byte(0, 0, 255, 255),
        float2{ x + w, y + h }, Rgba4Byte(255, 255, 255, 255),
    };

#define USE_MATRIX  1
#if USE_MATRIX == 1
    matrix4 matTransToOrigin;
    matTransToOrigin.translate(-150, -150, 0); // move center to origin.

    static float delta = 0;
    matrix4 matRotate;
    matRotate.rotateZ(delta);
    delta += 1;

    matrix4 matTransback;
    matTransback.translate(150, 150, 0);

    matrix4 matRotateSelf = matTransback * (matRotate * matTransToOrigin);

    screenProj = screenProj * matRotateSelf;
#else
    static float delta = 0;
    delta += 1;
    // rotate according to axis Z, (0, 0).
    quaternion quat = angleAxis(delta, float3(0, 0, 1));
    matrix4 matMVP = makeTransform(float3(0, 0, 0), float3(1, 1, 1), quat);
    screenProj = screenProj * matMVP;
#endif

    glUniformMatrix4fv(_mvp, 1, false, screenProj.data());
    glVertexAttribPointer(_position, 2, GL_FLOAT, false, sizeof( Vertex ), ary);
    glVertexAttribPointer(_color, 4, GL_UNSIGNED_BYTE, true, sizeof( Vertex ), &( ary[0].color ));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    end();
}

void RotateRectangle::begin()
{
    glUseProgram(_programId);
    glEnableVertexAttribArray(_position);
    glEnableVertexAttribArray(_color);
}

void RotateRectangle::end()
{
    glDisableVertexAttribArray(_color);
    glDisableVertexAttribArray(_position);
    glUseProgram(0);
}

bool RotateRectangle::init()
{
    _valid = ShaderProgram::initWithFile(_vsFileName, _fsFileName);
    if ( _valid )
    {
        _position = glGetAttribLocation(_programId, "_position");
        _color = glGetAttribLocation(_programId, "_color");
        _mvp = glGetUniformLocation(_programId, "_mvp");
    }
    return _valid;
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