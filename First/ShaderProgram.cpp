#include "ShaderProgram.h"
#include "ShaderHelper.h"
#include "TextReader.h"
#include "macros.h"
NS_BEGIN(elloop);

void ShaderProgram::begin() {

}

bool ShaderProgram::initWithFile(const std::string & vsFileName, const std::string& fsFileName) {
    using elloop::TextReader;
    using elloop::ShaderHelper;

    bool error(false);
    do {
        _vertexShaderSrc = TextReader::readFromFile(vsFileName);
        error = _vertexShaderSrc.empty();
        BREAK_IF(error);

        _vertexShaderId = ShaderHelper::compileVertexShader(_vertexShaderSrc.c_str());
        error = _vertexShaderId == 0;
        BREAK_IF(error);

        _fragShaderSrc = TextReader::readFromFile(fsFileName);
        error = _fragShaderSrc.empty();
        BREAK_IF(error);

        _fragShaderId = ShaderHelper::compileFragmentShader(_fragShaderSrc.c_str());
        error = _fragShaderId == 0;
        BREAK_IF(error);

        _programId = ShaderHelper::linkProgram(_vertexShaderId, _fragShaderId);
        error = _programId == 0;
        BREAK_IF(error);

    } while (false);
   
    return !error;
}

void ShaderProgram::end() {

}

bool ShaderProgram::validateProgram() {
    glValidateProgram(_programId);
    GLint validateStatus(0);
    glGetProgramiv(_programId, GL_VALIDATE_STATUS, &validateStatus);
    return (validateStatus == GL_TRUE);
}


void ShaderProgram_p2c4::begin() {
    glUseProgram(_programId);
    glEnableVertexAttribArray(_position);
}

void ShaderProgram_p2c4::end() {
    glDisableVertexAttribArray(_position);
    glUseProgram(0);
}


/************************************************************************/
/* implementation of       ShaderProgram_p2c4                                                               */
/************************************************************************/

const std::string ShaderProgram_p2c4::sc_VertexShaderName = "shaders/p2c4_vertex_shader.glsl";
const std::string ShaderProgram_p2c4::sc_FragmentShaderName = "shaders/p2c4_fragment_shader.glsl";

bool ShaderProgram_p2c4::init() {
    _valid = ShaderProgram::initWithFile(sc_VertexShaderName, sc_FragmentShaderName);
    if (_valid) {
        _position   = glGetAttribLocation(_programId, "_position");
        _color      = glGetUniformLocation(_programId, "_color");
        _mvp        = glGetUniformLocation(_programId, "_mvp");
    }
    return _valid;
}

ShaderProgram_p2c4* ShaderProgram_p2c4::create() {
    auto * self = new ShaderProgram_p2c4();
    if (self && self->init()) {
        self->autorelease();
        return self;
    }
    return nullptr;
}




NS_END(elloop);