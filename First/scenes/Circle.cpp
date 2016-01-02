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

    const int Delta = 1;
    const int PointNeeded = 360 / Delta + 1 + 1;
    Vertex ary2[PointNeeded] = { 0 };

    float radius = 50;
    float pi = 3.1415926;
    float centerX = 100;
    float centerY = 100; 
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

    glUniformMatrix4fv(mvp_, 1, false, screenProj.data());
    glVertexAttribPointer(position_, 2, GL_FLOAT, false, sizeof( Vertex ), ary2);
    glVertexAttribPointer(color_, 4, GL_UNSIGNED_BYTE, true, sizeof( Vertex ), &( ary2[0].color ));
    glDrawArrays(GL_TRIANGLE_FAN, 0, PointNeeded);

    end();
}

void Circle::begin()
{
    glUseProgram(programId_);
    glEnableVertexAttribArray(position_);
    glEnableVertexAttribArray(color_);
}

void Circle::end()
{
    glDisableVertexAttribArray(color_);
    glDisableVertexAttribArray(position_);
    glUseProgram(0);
}

bool Circle::init()
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