#pragma once
#include "GLES2/gl2.h"
#include "elloop/inc.h"
#include <string>

NS_BEGIN(elloop);

typedef GLuint ShaderId;
typedef GLuint ProgramId;

class ShaderHelper
{
public:
    static const ShaderId compileVertexShader(const std::string &shaderSrc)
    {
        return compileShader(shaderSrc, GL_VERTEX_SHADER);
    }

    static const ShaderId compileFragmentShader(const std::string &shaderSrc)
    {
        return compileShader(shaderSrc, GL_FRAGMENT_SHADER);
    }

    static ProgramId linkProgram(ShaderId _vertexShaderId, 
                                 ShaderId _fragShaderId);
private:
    static ShaderId compileShader(const std::string &shaderSrc, 
                                  GLenum shaderType);
};

NS_END(elloop);