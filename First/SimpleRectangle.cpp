#include "SimpleRectangle.h"
#include "ELDirector.h"
#include "math/ELGeometry.h"

NS_BEGIN(elloop);

const std::string SimpleRectangle::sc_VertexShaderName = "shaders/p2c4_vertex_shader.glsl";
const std::string SimpleRectangle::sc_FragmentShaderName = "shaders/p2c4_fragment_shader.glsl";

void SimpleRectangle::render() {
    //glClearColor(1, 1, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto director = Director::getInstance();
    Size s = director->getFrameSize();
    float width = s._width;
    float height = s._height;

    glViewport(0, 0, width, height);

    begin();

    CELL::matrix4 screenProj = CELL::ortho<float>(0, width, height, 0, -100.0f, 100);
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
    glUniformMatrix4fv(_mvp, 1, false, screenProj.data());
    glUniform4f(_color, 1, 0, 0, 0.5);
    glVertexAttribPointer(_position, 2, GL_FLOAT, false, 2 * sizeof( GLfloat ), ary);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
    end();
}

void SimpleRectangle::begin() {
    glUseProgram(_programId);
    glEnableVertexAttribArray(_position);
}

void SimpleRectangle::end() {
    glDisableVertexAttribArray(_position);
    glUseProgram(0);
}

bool SimpleRectangle::init() {
    _valid = ShaderProgram::initWithFile(sc_VertexShaderName, sc_FragmentShaderName);
    if ( _valid ) {
        _position   = glGetAttribLocation(  _programId, "_position");
        _color      = glGetUniformLocation( _programId, "_color");
        _mvp        = glGetUniformLocation( _programId, "_mvp");
    }
    return _valid;
}

SimpleRectangle* SimpleRectangle::create() {
    auto * self = new SimpleRectangle();
    if ( self && self->init() ) {
        self->autorelease();
        return self;
    }
    return nullptr;
}

NS_END(elloop);