#include "ELShaderProgram.h"
#include "util/ELShaderHelper.h"
#include "util/ELTextReader.h"

NS_BEGIN(elloop);

bool ShaderProgram::initWithFile(const std::string & vsFileName,
                                 const std::string& fsFileName)
{
    using elloop::TextReader;
    using elloop::ShaderHelper;

    bool error(false);
    do
    {
        vertexShaderSrc_ = TextReader::readFromFile(vsFileName);
        error = vertexShaderSrc_.empty();
        BREAK_IF(error);

        vertexShaderId_ = ShaderHelper::compileVertexShader(vertexShaderSrc_);
        error = vertexShaderId_ == 0;
        BREAK_IF(error);

        fragShaderSrc_ = TextReader::readFromFile(fsFileName);
        error = fragShaderSrc_.empty();
        BREAK_IF(error);

        fragShaderId_ = ShaderHelper::compileFragmentShader(fragShaderSrc_);
        error = fragShaderId_ == 0;
        BREAK_IF(error);

        programId_ = ShaderHelper::linkProgram(vertexShaderId_, fragShaderId_);
        error = programId_ == 0;
        BREAK_IF(error);

    }
    while (false);

    return !error;
}

bool ShaderProgram::validateProgram()
{
    glValidateProgram(programId_);
    GLint validateStatus(0);
    glGetProgramiv(programId_, GL_VALIDATE_STATUS, &validateStatus);
    return (validateStatus == GL_TRUE);
}

NS_END(elloop);
