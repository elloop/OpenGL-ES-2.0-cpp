#include "scenes/Circle.h"
#include "app_control/ELDirector.h"
#include "math/ELGeometry.h"
#include <cmath>

NS_BEGIN(elloop);


void Circle::render()
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

    // angle between every two consecutive triangles.
    const int Delta = 1;
    // total points = triangle's count + 1 + center
    const int PointNeeded = 360 / Delta + 1 + 1;
    Vertex ary2[PointNeeded] = { 0 };

    float radius = 50;
    float pi = 3.1415926;
    float centerX = 100;
    float centerY = 100; 

    // center location
    ary2[0].pos.x = centerX;
    ary2[0].pos.y = centerY;
    ary2[0].color = {0, 255, 0, 255};

    static unsigned char r = rand() % 255;
    static unsigned char g = rand() % 255;
    static unsigned char b = rand() % 255;
    for ( int i = 1; i < PointNeeded; ++i )
    {
        ary2[i].pos.x = centerX + radius * cos(i*Delta / 180.f * pi);
        ary2[i].pos.y = centerY + radius * sin(i*Delta / 180.f * pi);
        ary2[i].color = Rgba4Byte(r, g, b, 255);
    }

    glUniformMatrix4fv(_mvp, 1, false, screenProj.data());
    glVertexAttribPointer(_position, 2, GL_FLOAT, false, sizeof( Vertex ), ary2);
    glVertexAttribPointer(_color, 4, GL_UNSIGNED_BYTE, true, sizeof( Vertex ), &( ary2[0].color ));
    glDrawArrays(GL_TRIANGLE_FAN, 0, PointNeeded);

    end();
}

void Circle::begin()
{
    glUseProgram(_programId);
    glEnableVertexAttribArray(_position);
    glEnableVertexAttribArray(_color);
}

void Circle::end()
{
    glDisableVertexAttribArray(_color);
    glDisableVertexAttribArray(_position);
    glUseProgram(0);
}

bool Circle::init()
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

Circle* Circle::create()
{
    auto * self = new Circle();
    if ( self && self->init() )
    {
        self->autorelease();
        return self;
    }
    return nullptr;
}

NS_END(elloop);