#include "ELShaderProgram.h"
#include "ELShaderHelper.h"
#include "ELTextReader.h"
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






NS_END(elloop);